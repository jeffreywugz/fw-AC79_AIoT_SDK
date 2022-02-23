#include "system/includes.h"
#include "server/audio_server.h"
#include "server/server_core.h"
#include "server/usb_syn_api.h"
#include "video/video_ioctl.h"
#include "event/device_event.h"
#include "app_config.h"

#if TCFG_USB_SLAVE_AUDIO_ENABLE

#include "device/usb_stack.h"
#include "device/uac_stream.h"

struct usb_audio_handle {
    struct video_buffer b;
    struct audio_format f;
    void *rec_dev;
    void *rec_priv;
    u8 *audio_rec_dma_buffer;
    u16 sys_event_id;
    u16 offset;
    OS_SEM rd_sem;
    void *play_dev;
    void *play_priv;
    u8 bindex;
    s8 play_start;
    void *sample_sync_buf;
    cbuffer_t sample_sync_cbuf;
    u8 *dacsyn_ptr;
    dac_usb_syn_ops *dacUSBsyn_ops;
};

static struct usb_audio_handle uac_handle;
#define __this      (&uac_handle)

static int usb_audio_mic_tx_handler(int priv, void *data, int len)
{
    int err = 0;
    struct video_buffer *b;

    if (!__this->rec_dev) {
        return 0;
    }

    b = &__this->b;
    b->noblock = 1;
    b->timeout = 0;
    b->index = __this->bindex;

    if (__this->offset == 0) {
        err = dev_ioctl(__this->rec_dev, AUDIOC_DQBUF, (u32)b);
        if (err || !b->len) {
            return 0;
        }
    }

    memcpy(data, (u8 *)b->baddr + __this->offset, len);
    __this->offset += len;

    if (__this->offset == b->len) {
        dev_ioctl(__this->rec_dev, AUDIOC_QBUF, (u32)b);
        __this->offset = 0;
    }

    return len;
}

static int usb_audio_mic_set_vol(u8 volume)
{
    if (__this->rec_dev) {
#ifdef CONFIG_ALL_ADC_CHANNEL_OPEN_ENABLE
        return dev_ioctl(__this->rec_dev, IOCTL_SET_VOLUME, (BIT(CONFIG_UAC_MIC_ADC_CHANNEL) << 8) | volume);
#else
        return dev_ioctl(__this->rec_dev, IOCTL_SET_VOLUME, volume);
#endif
    }

    return 0;
}

static int usb_audio_mic_close(void)
{
    if (__this->rec_dev) {
        dev_ioctl(__this->rec_dev, AUDIOC_STREAM_OFF, (u32)&__this->bindex);
        if (__this->offset) {
            dev_ioctl(__this->rec_dev, AUDIOC_QBUF, (u32)&__this->b);
            __this->offset = 0;
        }
        dev_close(__this->rec_dev);
        __this->rec_dev = NULL;
        set_uac_mic_tx_handler(NULL, NULL);
    }
    if (__this->audio_rec_dma_buffer) {
        free(__this->audio_rec_dma_buffer);
        __this->audio_rec_dma_buffer = NULL;
    }

    return 0;
}

static int usb_audio_mic_open(int value)
{
    int err = 0;
    void *mic_hdl = NULL;
    struct video_reqbufs breq = {0};

    if (__this->rec_dev) {
        return 0;
    }

    extern u16 uac_get_mic_vol(const usb_dev usb_id);
    extern u32 uac_get_mic_sameplerate(void *priv);
    u32 frame_len = ((uac_get_mic_sameplerate(NULL) * MIC_AUDIO_RES / 8 * MIC_CHANNEL) / 1000);
    frame_len += (uac_get_mic_sameplerate(NULL) % 1000 ? (MIC_AUDIO_RES / 8) * MIC_CHANNEL : 0);

    memset(&__this->f, 0, sizeof(struct audio_format));
    memset(&__this->b, 0, sizeof(struct video_buffer));

    mic_hdl = dev_open("audio", (void *)AUDIO_TYPE_ENC_MIC);
    if (!mic_hdl) {
        log_e("uac audio_open: err\n");
        return -EFAULT;
    }

    __this->audio_rec_dma_buffer = malloc(frame_len * 3 * 4);
    if (!__this->audio_rec_dma_buffer) {
        goto __err;
    }

    breq.buf  = __this->audio_rec_dma_buffer;
    breq.size = frame_len * 3 * 4;

    err = dev_ioctl(mic_hdl, AUDIOC_REQBUFS, (unsigned int)&breq);
    if (err) {
        goto __err;
    }

    __this->offset = 0;
#ifdef CONFIG_ALL_ADC_CHANNEL_OPEN_ENABLE
    __this->f.channel_bit_map = BIT(CONFIG_UAC_MIC_ADC_CHANNEL);
#endif
#if CONFIG_AUDIO_ENC_SAMPLE_SOURCE == AUDIO_ENC_SAMPLE_SOURCE_PLNK0
    __this->f.sample_source = "plnk0";
#elif CONFIG_AUDIO_ENC_SAMPLE_SOURCE == AUDIO_ENC_SAMPLE_SOURCE_PLNK1
    __this->f.sample_source = "plnk1";
#elif CONFIG_AUDIO_ENC_SAMPLE_SOURCE == AUDIO_ENC_SAMPLE_SOURCE_IIS0
    __this->f.sample_source = "iis0";
#elif CONFIG_AUDIO_ENC_SAMPLE_SOURCE == AUDIO_ENC_SAMPLE_SOURCE_IIS1
    __this->f.sample_source = "iis1";
#else
    __this->f.sample_source = "mic";
#endif
    __this->f.format = "pcm";
    __this->f.volume = uac_get_mic_vol(!!(value & BIT(31)));
    value &= ~BIT(31);
    __this->f.channel = value >> 24;
    __this->f.sample_rate = value & 0xffffff;
    __this->f.frame_len = frame_len * 3;

    log_d("uac channel : %d\n", __this->f.channel);
    log_d("uac sample_rate : %d\n", __this->f.sample_rate);
    log_d("uac volume : %d\n", __this->f.volume);
    log_d("uac frame_len : %d\n", __this->f.frame_len);

    err = dev_ioctl(mic_hdl, AUDIOC_SET_FMT, (unsigned int)&__this->f);
    if (err) {
        log_e("uac audio_set_fmt: err\n");
        goto __err;
    }

    err = dev_ioctl(mic_hdl, AUDIOC_STREAM_ON, (u32)&__this->bindex);
    if (err) {
        log_e(" uac audio rec stream on err\n");
        goto __err;
    }

    __this->rec_dev = mic_hdl;

    set_uac_mic_tx_handler(NULL, usb_audio_mic_tx_handler);

    log_i("uac audio_enc_start: suss\n");

    return 0;
__err:
    if (mic_hdl) {
        dev_close(mic_hdl);
    }
    if (__this->audio_rec_dma_buffer) {
        free(__this->audio_rec_dma_buffer);
        __this->audio_rec_dma_buffer = NULL;
    }

    return err;
}

static void speaker_rx_response(int priv, void *buf, int len)
{
    if (__this->play_start) {
        __this->dacUSBsyn_ops->run(__this->dacsyn_ptr, buf, len);
    }
}

static int uac_vfs_fread(void *file, void *data, u32 len)
{
    struct usb_audio_handle *hdl = (struct usb_audio_handle *)file;

    int rlen = 0;

    while (!rlen) {
        if (!hdl->play_start) {
            return -2;
        }

        rlen = cbuf_get_data_size(&hdl->sample_sync_cbuf);
        if (!rlen) {
            os_sem_pend(&hdl->rd_sem, 0);
        }
    }
    if (rlen > len) {
        rlen = len;
    }

    cbuf_read(&hdl->sample_sync_cbuf, data, rlen);

    return rlen;
}

static int uac_vfs_fclose(void *file)
{
    return 0;
}

static int uac_vfs_flen(void *file)
{
    return 0;
}

static const struct audio_vfs_ops uac_vfs_ops = {
    .fread  = uac_vfs_fread,
    .fclose = uac_vfs_fclose,
    .flen   = uac_vfs_flen,
};

static void dac_write_obuf(u8 *outbuf, u32 len)
{
    if (__this->play_start) {
        cbuf_write(&__this->sample_sync_cbuf, outbuf, len);
        os_sem_set(&__this->rd_sem, 0);
        os_sem_post(&__this->rd_sem);
    }
}

static u32 dac_get_obuf(void)
{
    return cbuf_get_data_size(&__this->sample_sync_cbuf);
}

static int usb_audio_sample_sync_open(struct usb_audio_handle *hdl, int sample_rate)
{
    int sync_buff_size = sample_rate * 4 * hdl->f.channel / 1000 * 30;
    hdl->sample_sync_buf = zalloc(sync_buff_size);
    if (!hdl->sample_sync_buf) {
        return -ENOMEM;
    }

    os_sem_create(&hdl->rd_sem, 0);

    speaker_funapi sf;
    sf.output = dac_write_obuf;
    sf.getlen = dac_get_obuf;

    hdl->dacUSBsyn_ops = get_dac_usbsyn_ops();
    ASSERT(hdl->dacUSBsyn_ops != NULL);
    hdl->dacsyn_ptr = (u8 *)malloc(hdl->dacUSBsyn_ops->need_buf());
    if (!hdl->dacsyn_ptr) {
        goto __err;
    }
    cbuf_init(&hdl->sample_sync_cbuf, hdl->sample_sync_buf, sync_buff_size);
    hdl->dacUSBsyn_ops->open(hdl->dacsyn_ptr, sync_buff_size / 2, &sf);
    set_uac_speaker_rx_handler(hdl->play_priv, speaker_rx_response);

    return 0;

__err:
    if (hdl->sample_sync_buf) {
        free(hdl->sample_sync_buf);
        hdl->sample_sync_buf = NULL;
    }

    return -EINVAL;
}

static void usb_audio_sample_sync_close(struct usb_audio_handle *hdl)
{
    set_uac_speaker_rx_handler(NULL, NULL);
    if (hdl->dacsyn_ptr) {
        free(hdl->dacsyn_ptr);
        hdl->dacsyn_ptr = NULL;
    }
    hdl->dacUSBsyn_ops = NULL;

    if (hdl->sample_sync_buf) {
        free(hdl->sample_sync_buf);
        hdl->sample_sync_buf = NULL;
    }
}

static int usb_audio_speaker_open(int value)
{
    int err = 0;
    union audio_req req = {0};

    if (__this->play_dev) {
        return -1;
    }

    __this->play_dev = server_open("audio_server", "dec");
    if (!__this->play_dev) {
        return -1;
    }

    u16 vol = 0;
    extern void uac_get_cur_vol(const usb_dev usb_id, u16 * l_vol, u16 * r_vol);
    uac_get_cur_vol(!!(value & BIT(31)), &vol, NULL);

    value &= ~BIT(31);
    __this->play_priv = __this;
    __this->f.channel = value >> 24;
    __this->f.sample_rate = value & 0xffffff;

    req.dec.cmd             = AUDIO_DEC_OPEN;
    req.dec.volume          = vol;
    req.dec.output_buf_len  = 640 * 3;
    req.dec.channel         = __this->f.channel;
    req.dec.sample_rate     = __this->f.sample_rate;
    req.dec.priority        = 1;
    req.dec.vfs_ops         = &uac_vfs_ops;
    req.dec.dec_type 		= "pcm";
    req.dec.sample_source   = "dac";
    /* req.dec.attr            = AUDIO_ATTR_REAL_TIME; */
    req.dec.file 			= (FILE *)__this;

    err = server_request(__this->play_dev, AUDIO_REQ_DEC, &req);
    if (err) {
        goto __err1;
    }

    err = usb_audio_sample_sync_open(__this, __this->f.sample_rate);
    if (err) {
        goto __err1;
    }

    req.dec.cmd = AUDIO_DEC_START;
    err = server_request(__this->play_dev, AUDIO_REQ_DEC, &req);
    if (err) {
        goto __err1;
    }

    __this->play_start = 1;

    return 0;

__err1:
    if (__this->play_dev) {
        server_close(__this->play_dev);
        __this->play_dev = NULL;
    }

    return -1;
}

static int usb_audio_speaker_close(void)
{
    union audio_req req = {0};

    if (__this->play_dev) {
        __this->play_start = 0;
        os_sem_post(&__this->rd_sem);
        req.dec.cmd = AUDIO_DEC_STOP;
        server_request(__this->play_dev, AUDIO_REQ_DEC, &req);
        server_close(__this->play_dev);
        __this->play_dev = NULL;
    }

    usb_audio_sample_sync_close(__this);

    return 0;
}

static int usb_audio_speaker_set_vol(u8 volume)
{
    union audio_req req = {0};
    req.dec.cmd     = AUDIO_DEC_SET_VOLUME;
    req.dec.volume  = volume;
    return server_request(__this->play_dev, AUDIO_REQ_DEC, &req);
}

int usb_audio_event_handler(struct device_event *event)
{
    int value = event->value;

    switch (event->event) {
    case USB_AUDIO_MIC_OPEN:
        usb_audio_mic_open(value);
        break;
    case USB_AUDIO_MIC_CLOSE:
        usb_audio_mic_close();
        break;
    case USB_AUDIO_SET_MIC_VOL:
        log_i("USB_AUDIO_SET_MIC_VOL : %d\n", value);
        usb_audio_mic_set_vol(value);
        break;
    case USB_AUDIO_SET_PLAY_VOL:
        log_i("USB_AUDIO_SET_PLAY_VOL : %d\n", value & 0xff);
        usb_audio_speaker_set_vol(value & 0xff);
        break;
    case USB_AUDIO_PLAY_OPEN:
        log_i("USB_AUDIO_SET_PLAY_OPEN\n");
        usb_audio_speaker_open(value);
        break;
    case USB_AUDIO_PLAY_CLOSE:
        log_i("USB_AUDIO_SET_PLAY_CLOSE\n");
        usb_audio_speaker_close();
        break;
    }

    return true;
}

#endif
