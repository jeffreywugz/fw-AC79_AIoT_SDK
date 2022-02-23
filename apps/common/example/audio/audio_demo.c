#include "server/audio_server.h"
#include "system/includes.h"
#include "server/server_core.h"
#include "app_config.h"
#include "syscfg/syscfg_id.h"
#include "system/app_core.h"
#include "event/key_event.h"
#include "fs/fs.h"
#include "event/device_event.h"

#include "storage_device.h"
#include "reverb_deal.h"
#include "audio_music/audio_digital_vol.h"
#include "system/wait.h"

#include "generic/circular_buf.h"
#include "os/os_api.h"
#include <time.h>

#ifdef USE_AUDIO_DEMO

static int main_key_event_handler(struct key_event *key)
{
    switch (key->action) {
    case KEY_EVENT_CLICK:
        switch (key->value) {
        case KEY_MODE:
            audio_demo_mode_switch();
            break;
        default:
            return false;
        }
        break;
    case KEY_EVENT_LONG:
        break;
    default:
        return false;
    }

    return true;
}

static int main_dev_event_handler(struct device_event *event)
{
    switch (event->event) {
    case DEVICE_EVENT_IN:
        break;
    case DEVICE_EVENT_OUT:
        break;
    case DEVICE_EVENT_CHANGE:
        break;
    }
    return 0;
}

/*
 *  * 默认的系统事件处理函数
 *   * 当所有活动的app的事件处理函数都返回false时此函数会被调用
 *    */
void app_default_event_handler(struct sys_event *event)
{
    switch (event->type) {
    case SYS_KEY_EVENT:
        main_key_event_handler((struct key_event *)event->payload);
        break;
    case SYS_TOUCH_EVENT:
        break;
    case SYS_DEVICE_EVENT:
        main_dev_event_handler((struct device_event *)event->payload);
        break;
    default:
        break;
    }
}

struct audio_app_t {
    const char *tone_file_name;
    const char *app_name;
};

static const struct audio_app_t audio_app_table[] = {
#ifdef CONFIG_LOCAL_MUSIC_MODE_ENABLE
    {"FlashMusic.mp3", "local_music"},
#endif
#ifdef CONFIG_RECORDER_MODE_ENABLE
    {"Recorder.mp3", "recorder"},
#endif
#ifdef CONFIG_ASR_ALGORITHM_ENABLE
    {"AiSpeaker.mp3", "ai_speaker"},
#endif
};

static u8 mode_index = 0;

static void dec_server_event_handler(void *priv, int argc, int *argv)
{
    union audio_req r = {0};

    switch (argv[0]) {
    case AUDIO_SERVER_EVENT_ERR:
        log_i("tone: AUDIO_SERVER_EVENT_ERR\n");
    case AUDIO_SERVER_EVENT_END:
        log_i("tone: AUDIO_SERVER_EVENT_END\n");
        r.dec.cmd = AUDIO_DEC_STOP;
        server_request(priv, AUDIO_REQ_DEC, &r);
        server_close(priv); //priv是server_register_event_handler_to_task的priv参数
        fclose((FILE *)argv[1]); //argv[1]是解码开始时传递进去的文件句柄
        key_event_enable();
        //等提示音播完了再切换模式
        struct intent it;
        init_intent(&it);
        it.name = audio_app_table[mode_index++].app_name;
        start_app(&it);
        break;
    case AUDIO_SERVER_EVENT_CURR_TIME:
        log_i("play_time: %d\n", argv[1]);
        break;
    default:
        break;
    }
}

//播放提示音
static int app_play_tone_file(const char *path)
{
    int err = 0;
    union audio_req req = {0};

    log_d("play tone file : %s\n", path);

    FILE *file = fopen(path, "r");
    if (!file) {
        return -1;
    }

    void *dec_server = server_open("audio_server", "dec");
    if (!dec_server) {
        goto __err;
    }
    server_register_event_handler_to_task(dec_server, dec_server, dec_server_event_handler, "app_core");

    req.dec.cmd             = AUDIO_DEC_OPEN;
    req.dec.volume          = 50;
    req.dec.output_buf_len  = 4 * 1024;
    req.dec.file            = file;
    req.dec.sample_source   = "dac";

    syscfg_read(CFG_MUSIC_VOL, &req.dec.volume, sizeof(req.dec.volume));

    err = server_request(dec_server, AUDIO_REQ_DEC, &req);
    if (err) {
        goto __err;
    }

    req.dec.cmd = AUDIO_DEC_START;

    err = server_request(dec_server, AUDIO_REQ_DEC, &req);
    if (err) {
        goto __err;
    }

    key_event_disable();

    return 0;

__err:

    if (dec_server) {
        server_close(dec_server);
    }
    if (file) {
        fclose(file);
    }

    return err;
}

int audio_demo_mode_switch(void)
{
    if (mode_index >= ARRAY_SIZE(audio_app_table)) {
        mode_index = 0;
    }
    struct intent it;

    if (get_current_app()) {
        init_intent(&it);
        it.name = audio_app_table[mode_index].app_name;
        it.action = ACTION_STOP;    //退出当前模式
        start_app(&it);
    }

    char path[64];

    sprintf(path, "%s%s", CONFIG_VOICE_PROMPT_FILE_PATH, audio_app_table[mode_index].tone_file_name);

    return app_play_tone_file(path);
}

static void audio_demo_init(void)
{
    audio_demo_mode_switch();
    while (1) {
        os_time_dly(300);
    }
}

static int audio_demo_init_task(void)
{
    return thread_fork("audio_demo_init", 11, 1024, 0, 0, audio_demo_init, NULL);
}
late_initcall(audio_demo_init_task);





#ifdef CONFIG_LOCAL_MUSIC_MODE_ENABLE

#define CONFIG_STORE_VOLUME
#define VOLUME_STEP 5
#define MIN_VOLUME_VALUE	5
#define MAX_VOLUME_VALUE	100
#define INIT_VOLUME_VALUE   50

struct local_music_hdl {
    u8 local_play_all;	//1:全盘播放 0:播放目录
    char volume;
    u8 reverb_enable;
    u16 wait_sd;
    u16 wait_udisk;
    int play_time;
    int total_time;
    FILE *file;
    struct vfscan *fscan;
    struct vfscan *dir_list;
    struct server *dec_server;
    struct audio_dec_breakpoint local_bp;
    const char *local_path;
    void *digital_vol_hdl;
};

static struct local_music_hdl local_music_handler;

#define __this_local 	(&local_music_handler)

//获取断点数据
static int local_music_get_dec_breakpoint(struct audio_dec_breakpoint *bp)
{
    int err;
    union audio_req r = {0};

    bp->len = 0;
    r.dec.bp = bp;
    r.dec.cmd = AUDIO_DEC_GET_BREAKPOINT;

    if (bp->data) {
        free(bp->data);
        bp->data = NULL;
    }

    err = server_request(__this_local->dec_server, AUDIO_REQ_DEC, &r);
    if (err) {
        return err;
    }

    if (r.dec.status == AUDIO_DEC_STOP) {
        bp->len = 0;
        free(bp->data);
        bp->data = NULL;
        return -1;
    }
    /* put_buf(bp->data, bp->len); */

    return 0;
}

//设置音量大小
static int local_music_set_dec_volume(int step)
{
    union audio_req req = {0};

    int volume = __this_local->volume + step;
    if (volume < MIN_VOLUME_VALUE) {
        volume = MIN_VOLUME_VALUE;
    } else if (volume > MAX_VOLUME_VALUE) {
        volume = MAX_VOLUME_VALUE;
    }
    if (volume == __this_local->volume) {
        return -1;
    }
    __this_local->volume = volume;

    log_d("set_dec_volume: %d\n", volume);

    req.dec.cmd     = AUDIO_DEC_SET_VOLUME;
    req.dec.volume  = volume;
    server_request(__this_local->dec_server, AUDIO_REQ_DEC, &req);

#ifdef CONFIG_STORE_VOLUME
    syscfg_write(CFG_MUSIC_VOL, &__this_local->volume, sizeof(__this_local->volume));
#endif

    return 0;
}

//获取解码器状态
static int local_music_get_dec_status(void)
{
    union audio_req req = {0};

    req.dec.cmd     = AUDIO_DEC_GET_STATUS;
    server_request(__this_local->dec_server, AUDIO_REQ_DEC, &req);

    return req.dec.status;
}

#ifdef CONFIG_DEC_DIGITAL_VOLUME_ENABLE
static int audio_dec_data_callback(u8 *buf, u32 len, u32 sample_rate, u8 ch_num)
{
    u32 rdlen = 0, i = 0;

    while (i < len) {
        rdlen = i + 32 * 2 * ch_num > len ? len - i : 32 * 2 * ch_num;
        user_audio_digital_volume_run(__this_local->digital_vol_hdl, buf + i, rdlen, sample_rate, ch_num);
        i += rdlen;
    }

    return len;
}
#endif

//暂停/继续播放
static int local_music_dec_play_pause(void)
{
    union audio_req r = {0};

#ifdef CONFIG_DEC_ANALOG_VOLUME_ENABLE
    r.dec.attr = AUDIO_ATTR_FADE_INOUT;
#endif
    r.dec.cmd = AUDIO_DEC_PP;
    return server_request(__this_local->dec_server, AUDIO_REQ_DEC, &r);
}

//停止播放
static int local_music_dec_stop(void)
{
    int err = 0;
    union audio_req req = {0};

    if (!__this_local->file) {
        return 0;
    }

    log_i("local_music_dec_stop\n");

    req.dec.cmd = AUDIO_DEC_STOP;
    server_request(__this_local->dec_server, AUDIO_REQ_DEC, &req);

    int argv[2];
    argv[0] = AUDIO_SERVER_EVENT_END;
    argv[1] = (int)__this_local->file;
    server_event_handler_del(__this_local->dec_server, 2, argv);

    fclose(__this_local->file);
    __this_local->file = NULL;

    return 0;
}

//解码文件
static int local_music_dec_file(FILE *file)
{
    int err;
    union audio_req req = {0};

    log_i("local_music_dec_local_file\n");

    if (!file) {
        return -1;
    }

    local_music_dec_stop();

    req.dec.cmd             = AUDIO_DEC_OPEN;
    req.dec.volume          = __this_local->volume;
    req.dec.output_buf_len  = 6 * 1024;
    req.dec.file            = file;
    req.dec.channel         = 0;
    req.dec.sample_rate     = 0;
    req.dec.priority        = 1;
    req.dec.sample_source   = CONFIG_AUDIO_DEC_PLAY_SOURCE;
#if 0	//变声变调功能
    req.dec.speedV = 80; // >80是变快，<80是变慢，建议范围：30到130
    req.dec.pitchV = 32768; // >32768是音调变高，<32768音调变低，建议范围20000到50000
    req.dec.attr = AUDIO_ATTR_PS_EN;
#endif

#if TCFG_EQ_ENABLE && defined EQ_CORE_V1
    req.dec.attr |= AUDIO_ATTR_EQ_EN;
#if TCFG_LIMITER_ENABLE
    req.dec.attr |= AUDIO_ATTR_EQ32BIT_EN;
#endif
#if TCFG_DRC_ENABLE
    req.dec.attr |= AUDIO_ATTR_DRC_EN;
#endif
#endif

#if CONFIG_DEC_DECRYPT_ENABLE
    //播放加密文件
    extern const struct audio_vfs_ops *get_decrypt_vfs_ops(void);
    req.dec.vfs_ops = get_decrypt_vfs_ops();
    req.dec.attr |= AUDIO_ATTR_DECRYPT_DEC;
#endif

#ifdef CONFIG_DEC_DIGITAL_VOLUME_ENABLE
    if (!__this_local->digital_vol_hdl) {
        __this_local->digital_vol_hdl = user_audio_digital_volume_open(0, 31, 1);
    }
    if (__this_local->digital_vol_hdl) {
        user_audio_digital_volume_reset_fade(__this_local->digital_vol_hdl);
        user_audio_digital_volume_set(__this_local->digital_vol_hdl, 31);
        req.dec.dec_callback = audio_dec_data_callback;
    }
#endif

    err = server_request(__this_local->dec_server, AUDIO_REQ_DEC, &req);
    if (err) {
        log_e("audio_dec_open: err = %d\n", err);
        fclose(file);
        return err;
    }

    __this_local->play_time = req.dec.play_time;
    __this_local->total_time = req.dec.total_time; //获取播放总时长

    req.dec.cmd = AUDIO_DEC_START;

    err = server_request(__this_local->dec_server, AUDIO_REQ_DEC, &req);
    if (err) {
        log_e("audio_dec_start: err = %d\n", err);
        fclose(file);
        return err;
    }

    __this_local->file = file;

    log_i("play_music_file: suss\n");

    return 0;
}

//切换上一首或下一首
static int local_music_dec_switch_file(int fsel_mode)
{
    int i = 0;
    FILE *file = NULL;
    int len = 0;
    char name[16] = {0};

    log_i("local_music_dec_switch_file\n");

    if (!__this_local->fscan || !__this_local->fscan->file_number) {
        return -1;
    }

__retry:
    do {
        file = fselect(__this_local->fscan, fsel_mode, fsel_mode == FSEL_BY_NUMBER ? (CPU_RAND() % __this_local->fscan->file_number) + 1 : 0);
        if (file) {
            memset(name, 0, sizeof(name));
            len = fget_name(file, (u8 *)name, sizeof(name) - 1);
            if (len > 0) {
                log_i("play file name : %s\n", name);
            }
            ++i;
            break;
        }
        if (fsel_mode == FSEL_NEXT_FILE) {
            fsel_mode = FSEL_FIRST_FILE;
        } else if (fsel_mode == FSEL_PREV_FILE) {
            fsel_mode = FSEL_LAST_FILE;
        } else {
            break;
        }
    } while (i++ < __this_local->fscan->file_number);

    if (!file) {
        return -1;
    }

    if (0 != local_music_dec_file(file)) {
        if (fsel_mode == FSEL_FIRST_FILE) {
            fsel_mode = FSEL_NEXT_FILE;
        } else if (fsel_mode == FSEL_LAST_FILE) {
            fsel_mode = FSEL_PREV_FILE;
        }
        goto __retry;
    }

    return 0;
}

//切换文件夹
static int local_music_dec_switch_dir(int fsel_mode)
{
    int len = 0;
    int i = 0;
    char name[16] = {0};
    char path[128] = {0};
    FILE *dir = NULL;
    FILE *file = NULL;

    log_i("local_music_dec_switch_dir\n");

    if (!__this_local->local_path) {
        return -1;
    }

    if (__this_local->local_play_all && __this_local->local_path != CONFIG_MUSIC_PATH_FLASH) {
        //全盘搜索
        if (__this_local->fscan) {
            fscan_release(__this_local->fscan);
        }
#if CONFIG_DEC_DECRYPT_ENABLE
        __this_local->fscan = fscan(__this_local->local_path, "-r -tMP3WMAWAVM4AAMRAPEFLAAACSPXOPUDTSADPSMP -sn", 2);
#else
        __this_local->fscan = fscan(__this_local->local_path, "-r -tMP3WMAWAVM4AAMRAPEFLAAACSPXOPUDTSADP -sn", 2);
#endif
        if (!__this_local->fscan) {
            return -1;
        }
        return local_music_dec_switch_file(FSEL_FIRST_FILE);
    }

    //搜索文件夹
    if (!__this_local->dir_list) {
        __this_local->dir_list = fscan(__this_local->local_path, "-d -sn", 2);
        if (!__this_local->dir_list || __this_local->dir_list->file_number == 0) {
            log_w("no_music_dir_find\n");
            return -1;
        }
    }

    //选择文件夹
__again:
    do {
        dir = fselect(__this_local->dir_list, fsel_mode, 0);
        if (dir) {
            i++;
            break;
        }
        if (fsel_mode == FSEL_NEXT_FILE) {
            fsel_mode = FSEL_FIRST_FILE;
        } else if (fsel_mode == FSEL_PREV_FILE) {
            fsel_mode = FSEL_LAST_FILE;
        } else {
            log_w("fselect_dir_faild, create dir\n");
            return -1;
        }
    } while (i++ < __this_local->dir_list->file_number);

    if (!dir) {
        return -1;
    }

    len = fget_name(dir, (u8 *)name, sizeof(name) - 1);
    if (0 == len) {
        fclose(dir);
        if (fsel_mode == FSEL_FIRST_FILE) {
            fsel_mode = FSEL_NEXT_FILE;
        } else if (fsel_mode == FSEL_LAST_FILE) {
            fsel_mode = FSEL_PREV_FILE;
        }
        goto __again;
    }

    fclose(dir);

    if (__this_local->fscan) {
        fscan_release(__this_local->fscan);
    }

    fname_to_path(path, __this_local->local_path, name, len, 1, 0);

#if 0	//此处播放指定目录，用户填写的目录路径要注意中文编码问题，看不懂就直接用16进制把路径打印出来
    const char *user_dir = "";
    if (!file && strcmp(path, user_dir)) {
        log_i("dir name : %s\n", path);
        if (fsel_mode == FSEL_FIRST_FILE) {
            fsel_mode = FSEL_NEXT_FILE;
        } else if (fsel_mode == FSEL_LAST_FILE) {
            fsel_mode = FSEL_PREV_FILE;
        }
        goto __again;
    }
#endif

    log_i("fscan path : %s\n", path);

    /*搜索文件夹下的音频文件，按序号排序*/
#if CONFIG_DEC_DECRYPT_ENABLE
    __this_local->fscan = fscan(path, "-tMP3WMAWAVM4AAMRAPEFLAAACSPXOPUDTSADPSMP -sn", 2);
#else
    __this_local->fscan = fscan(path, "-tMP3WMAWAVM4AAMRAPEFLAAACSPXOPUDTSADP -sn", 2);
#endif

    if (!file) {
        if (!__this_local->fscan || !__this_local->fscan->file_number) {
            if (fsel_mode == FSEL_FIRST_FILE) {
                fsel_mode = FSEL_NEXT_FILE;
            } else if (fsel_mode == FSEL_LAST_FILE) {
                fsel_mode = FSEL_PREV_FILE;
            }
            goto __again;
        }
        local_music_dec_switch_file(FSEL_FIRST_FILE);
    }

    return 0;
}

//快进快退,单位是秒,暂时只支持MP3格式
static int local_music_dec_seek(int seek_step)
{
    union audio_req r = {0};

    if (0 == seek_step) {
        return 0;
    }

    if (__this_local->total_time != 0 && __this_local->total_time != -1) {
        if (__this_local->play_time + seek_step <= 0 || __this_local->play_time + seek_step >= __this_local->total_time) {
            log_e("local music seek out of range\n");
            return -1;
        }
    }

    if (seek_step > 0) {
        r.dec.cmd = AUDIO_DEC_FF;
        r.dec.ff_fr_step = seek_step;
    } else {
        r.dec.cmd = AUDIO_DEC_FR;
        r.dec.ff_fr_step = -seek_step;
    }

    log_i("local music seek step : %d\n", seek_step);

    return server_request(__this_local->dec_server, AUDIO_REQ_DEC, &r);
}

//释放资源，切换播放源设备
static int local_music_switch_local_device(const char *path)
{
    log_i("local_music_switch_local_device\n");

    if (__this_local->dir_list) {
        fscan_release(__this_local->dir_list);
        __this_local->dir_list = NULL;
    }
    if (__this_local->fscan) {
        fscan_release(__this_local->fscan);
        __this_local->fscan = NULL;
    }
    if (__this_local->wait_sd) {
        wait_completion_del(__this_local->wait_sd);
        __this_local->wait_sd = 0;
    }
    if (__this_local->wait_udisk) {
        wait_completion_del(__this_local->wait_udisk);
        __this_local->wait_udisk = 0;
    }

    local_music_dec_stop();

    if (path == NULL) {
        return -1;
    }

    __this_local->local_path = path;

    local_music_dec_switch_dir(FSEL_FIRST_FILE);

    return 0;
}

static void local_dec_server_event_handler(void *priv, int argc, int *argv)
{
    switch (argv[0]) {
    case AUDIO_SERVER_EVENT_ERR:
        log_i("local_music: AUDIO_SERVER_EVENT_ERR\n");
    case AUDIO_SERVER_EVENT_END:
        log_i("local_music: AUDIO_SERVER_EVENT_END\n");
        local_music_dec_stop();
        local_music_dec_switch_file(FSEL_NEXT_FILE);
        break;
    case AUDIO_SERVER_EVENT_CURR_TIME:
        log_d("play_time: %d\n", argv[1]);
        __this_local->play_time = argv[1];
        break;
    }
}

static int local_music_mode_init(void)
{
    log_i("local_music_play_main\n");

    memset(__this_local, 0, sizeof(struct local_music_hdl));

#ifdef CONFIG_STORE_VOLUME
    if (syscfg_read(CFG_MUSIC_VOL, &__this_local->volume, sizeof(__this_local->volume)) < 0 ||
        __this_local->volume < MIN_VOLUME_VALUE || __this_local->volume > MAX_VOLUME_VALUE) {
        __this_local->volume = INIT_VOLUME_VALUE;
    }
#else
    __this_local->volume = INIT_VOLUME_VALUE;
#endif
    if (__this_local->volume < 0 || __this_local->volume > MAX_VOLUME_VALUE) {
        __this_local->volume = INIT_VOLUME_VALUE;
    }
    __this_local->local_play_all = 1;

    __this_local->dec_server = server_open("audio_server", "dec");
    if (!__this_local->dec_server) {
        return -1;
    }
    server_register_event_handler_to_task(__this_local->dec_server, NULL, local_dec_server_event_handler, "app_core");

    if (storage_device_ready()) {
        return local_music_switch_local_device(CONFIG_MUSIC_PATH_SD);
    } else {
        return local_music_switch_local_device(CONFIG_MUSIC_PATH_FLASH);
    }
}

static void local_music_mode_exit(void)
{
#if defined CONFIG_REVERB_MODE_ENABLE && defined CONFIG_AUDIO_MIX_ENABLE
    if (__this_local->reverb_enable) {
        echo_reverb_uninit();
    }
#endif
    local_music_switch_local_device(NULL);
    server_close(__this_local->dec_server);
    __this_local->dec_server = NULL;
}

static int local_music_key_click(struct key_event *key)
{
    int ret = true;

    switch (key->value) {
    case KEY_OK:
        local_music_dec_play_pause();
        break;
    case KEY_VOLUME_DEC:
        local_music_set_dec_volume(-VOLUME_STEP);
        break;
    case KEY_VOLUME_INC:
        local_music_set_dec_volume(VOLUME_STEP);
        break;
    default:
        ret = false;
        break;
    }

    return ret;
}

static int local_music_key_long(struct key_event *key)
{
    switch (key->value) {
    case KEY_OK:
        local_music_dec_switch_dir(FSEL_NEXT_FILE);
        break;
    case KEY_VOLUME_DEC:
        local_music_dec_switch_file(FSEL_PREV_FILE);
        break;
    case KEY_VOLUME_INC:
        local_music_dec_switch_file(FSEL_NEXT_FILE);
        break;
    case KEY_MODE:
#if defined CONFIG_REVERB_MODE_ENABLE && defined CONFIG_AUDIO_MIX_ENABLE
        if (__this_local->reverb_enable) {
            //关闭混响
            echo_reverb_uninit();
            __this_local->reverb_enable = 0;
        } else {
            //配置混响参数
            const struct __HOWLING_PARM_ howling_parm = {13, 20, 20, 300, 5, -50000/*-25000*/, 0, 16000, 1};
#if CONFIG_AUDIO_ENC_SAMPLE_SOURCE != AUDIO_ENC_SAMPLE_SOURCE_MIC
            echo_reverb_init(48000, 16000, BIT(CONFIG_AUDIO_ENC_SAMPLE_SOURCE + 3), 100, __this_local->volume, NULL, NULL, (void *)&howling_parm, NULL);
#else
            echo_reverb_init(48000, 16000, BIT(CONFIG_REVERB_ADC_CHANNEL), 100, __this_local->volume, NULL, NULL, (void *)&howling_parm, NULL);
#endif
            __this_local->reverb_enable = 1;
        }
#else
        if (storage_device_ready()) {
            local_music_switch_local_device(CONFIG_MUSIC_PATH_SD);
        }
#endif
        break;
    default:
        break;
    }

    return true;
}

static int local_music_key_event_handler(struct key_event *key)
{
    switch (key->action) {
    case KEY_EVENT_CLICK:
        return local_music_key_click(key);
    case KEY_EVENT_LONG:
        return local_music_key_long(key);
    default:
        break;
    }

    return true;
}

/*
 *设备响应函数
 */
static int local_music_device_event_handler(struct sys_event *sys_eve)
{
    struct device_event *device_eve = (struct device_event *)sys_eve->payload;
    /* SD卡插拔处理 */
    if (sys_eve->from == DEVICE_EVENT_FROM_SD) {
        switch (device_eve->event) {
        case DEVICE_EVENT_IN:
            //等待SD卡挂载完成才开始搜索文件
            __this_local->wait_sd = wait_completion(sdcard_storage_device_ready,
                                                    (int (*)(void *))local_music_switch_local_device,
                                                    CONFIG_MUSIC_PATH_SD, device_eve->arg);
            break;
        case DEVICE_EVENT_OUT:
            //SD卡拔出，释放资源
            local_music_switch_local_device(NULL);
            break;
        }
#if TCFG_UDISK_ENABLE
        /* U盘插拔处理 */
    } else if (sys_eve->from == DEVICE_EVENT_FROM_USB_HOST && !strncmp((const char *)device_eve->value, "udisk", 4)) {
        switch (device_eve->event) {
        case DEVICE_EVENT_IN:
            //等待U盘挂载完成才开始搜索文件
            __this_local->wait_udisk = wait_completion(udisk_storage_device_ready,
                                       (int (*)(void *))local_music_switch_local_device,
                                       CONFIG_MUSIC_PATH_UDISK, ((const char *)device_eve->value)[5] - '0');
            break;
        case DEVICE_EVENT_OUT:
            //U盘拔出，释放资源
            local_music_switch_local_device(NULL);
            break;
        }
#endif
    }

    return true;
}

static int local_music_event_handler(struct application *app, struct sys_event *event)
{
    switch (event->type) {
    case SYS_KEY_EVENT:
        return local_music_key_event_handler((struct key_event *)event->payload);
    case SYS_DEVICE_EVENT:
        return local_music_device_event_handler(event);
    default:
        return false;
    }
}

static int local_music_state_machine(struct application *app, enum app_state state, struct intent *it)
{
    switch (state) {
    case APP_STA_CREATE:
        break;
    case APP_STA_START:
        local_music_mode_init();
        break;
    case APP_STA_PAUSE:
        break;
    case APP_STA_RESUME:
        break;
    case APP_STA_STOP:
        local_music_mode_exit();
        break;
    case APP_STA_DESTROY:
        break;
    }

    return 0;
}

static const struct application_operation local_music_ops = {
    .state_machine  = local_music_state_machine,
    .event_handler 	= local_music_event_handler,
};

REGISTER_APPLICATION(local_music) = {
    .name 	= "local_music",
    .ops 	= &local_music_ops,
    .state  = APP_STA_DESTROY,
};

#endif



#ifdef CONFIG_RECORDER_MODE_ENABLE

#define CONFIG_STORE_VOLUME
#define VOLUME_STEP         5
#define GAIN_STEP           5
#define MIN_VOLUME_VALUE	5
#define MAX_VOLUME_VALUE	100
#define INIT_VOLUME_VALUE   50

struct recorder_hdl {
    FILE *fp;
    struct server *enc_server;
    struct server *dec_server;
    void *cache_buf;
    cbuffer_t save_cbuf;
    OS_SEM w_sem;
    OS_SEM r_sem;
    volatile u8 run_flag;
    u8 volume;
    u8 gain;
    u8 channel;
    u8 direct;
    const char *sample_source;
    int sample_rate;
};

static struct recorder_hdl recorder_handler;

#define __this_record (&recorder_handler)

//AUDIO ADC支持的采样率
static const u16 sample_rate_table[] = {
    8000,
    11025,
    12000,
    16000,
    22050,
    24000,
    32000,
    44100,
    48000,
};

//编码器输出PCM数据
static int recorder_vfs_fwrite(void *file, void *data, u32 len)
{
    cbuffer_t *cbuf = (cbuffer_t *)file;
    if (0 == cbuf_write(cbuf, data, len)) {
        //上层buf写不进去时清空一下，避免出现声音滞后的情况
        cbuf_clear(cbuf);
    }
    os_sem_set(&__this_record->r_sem, 0);
    os_sem_post(&__this_record->r_sem);
    //此回调返回0录音就会自动停止
    return len;
}

//解码器读取PCM数据
static int recorder_vfs_fread(void *file, void *data, u32 len)
{
    cbuffer_t *cbuf = (cbuffer_t *)file;
    u32 rlen;

    do {
        rlen = cbuf_get_data_size(cbuf);
        rlen = rlen > len ? len : rlen;
        if (cbuf_read(cbuf, data, rlen) > 0) {
            len = rlen;
            break;
        }
        //此处等待信号量是为了防止解码器因为读不到数而一直空转
        os_sem_pend(&__this_record->r_sem, 0);
        if (!__this_record->run_flag) {
            return 0;
        }
    } while (__this_record->run_flag);

    //返回成功读取的字节数
    return len;
}

static int recorder_vfs_fclose(void *file)
{
    return 0;
}

static int recorder_vfs_flen(void *file)
{
    return 0;
}

static const struct audio_vfs_ops recorder_vfs_ops = {
    .fwrite = recorder_vfs_fwrite,
    .fread  = recorder_vfs_fread,
    .fclose = recorder_vfs_fclose,
    .flen   = recorder_vfs_flen,
};

static int recorder_close(void)
{
    union audio_req req = {0};

    if (!__this_record->run_flag) {
        return 0;
    }

    log_d("----------recorder close----------\n");

    __this_record->run_flag = 0;

    os_sem_post(&__this_record->w_sem);
    os_sem_post(&__this_record->r_sem);

    if (__this_record->enc_server) {
        req.enc.cmd = AUDIO_ENC_CLOSE;
        server_request(__this_record->enc_server, AUDIO_REQ_ENC, &req);
    }

    if (__this_record->dec_server) {
        req.dec.cmd = AUDIO_DEC_STOP;
        server_request(__this_record->dec_server, AUDIO_REQ_DEC, &req);
    }

    if (__this_record->cache_buf) {
        free(__this_record->cache_buf);
        __this_record->cache_buf = NULL;
    }

    if (__this_record->fp) {
        fclose(__this_record->fp);
        __this_record->fp = NULL;
    }

    return 0;
}

static void enc_server_event_handler(void *priv, int argc, int *argv)
{
    switch (argv[0]) {
    case AUDIO_SERVER_EVENT_ERR:
    case AUDIO_SERVER_EVENT_END:
        recorder_close();
        break;
    case AUDIO_SERVER_EVENT_SPEAK_START:
        log_i("speak start ! \n");
        break;
    case AUDIO_SERVER_EVENT_SPEAK_STOP:
        log_i("speak stop ! \n");
        break;
    default:
        break;
    }
}

//将MIC的数字信号采集后推到DAC播放
//注意：如果需要播放两路MIC，DAC分别对应的是DACL和DACR，要留意芯片封装是否有DACR引脚出来，
//      而且要使能DAC的双通道输出，DAC如果采用差分输出方式也只会听到第一路MIC的声音
static int recorder_play_to_dac(int sample_rate, u8 channel)
{
    int err;
    union audio_req req = {0};

    log_d("----------recorder_play_to_dac----------\n");

    if (channel > 2) {
        channel = 2;
    }
    __this_record->cache_buf = malloc(sample_rate * channel); //上层缓冲buf缓冲0.5秒的数据，缓冲太大听感上会有延迟
    if (__this_record->cache_buf == NULL) {
        return -1;
    }
    cbuf_init(&__this_record->save_cbuf, __this_record->cache_buf, sample_rate * channel);

    os_sem_create(&__this_record->w_sem, 0);
    os_sem_create(&__this_record->r_sem, 0);

    __this_record->run_flag = 1;

    /****************打开解码DAC器*******************/
    req.dec.cmd             = AUDIO_DEC_OPEN;
    req.dec.volume          = __this_record->volume;
    req.dec.output_buf_len  = 4 * 1024;
    req.dec.channel         = channel;
    req.dec.sample_rate     = sample_rate;
    req.dec.vfs_ops         = &recorder_vfs_ops;
    req.dec.dec_type 		= "pcm";
    req.dec.sample_source   = CONFIG_AUDIO_DEC_PLAY_SOURCE;
    req.dec.file            = (FILE *)&__this_record->save_cbuf;
    /* req.dec.attr            = AUDIO_ATTR_LR_ADD; */          //左右声道数据合在一起,封装只有DACL但需要测试两个MIC时可以打开此功能

    err = server_request(__this_record->dec_server, AUDIO_REQ_DEC, &req);
    if (err) {
        goto __err;
    }

    req.dec.cmd = AUDIO_DEC_START;
    server_request(__this_record->dec_server, AUDIO_REQ_DEC, &req);

    /****************打开编码器*******************/
    memset(&req, 0, sizeof(union audio_req));

    //BIT(x)用来区分上层需要获取哪个通道的数据
    if (channel == 2) {
        req.enc.channel_bit_map = BIT(CONFIG_AUDIO_ADC_CHANNEL_L) | BIT(CONFIG_AUDIO_ADC_CHANNEL_R);
    } else {
        req.enc.channel_bit_map = BIT(CONFIG_AUDIO_ADC_CHANNEL_L);
    }
    req.enc.frame_size = sample_rate / 100 * 4 * channel;	//收集够多少字节PCM数据就回调一次fwrite
    req.enc.output_buf_len = req.enc.frame_size * 3; //底层缓冲buf至少设成3倍frame_size
    req.enc.cmd = AUDIO_ENC_OPEN;
    req.enc.channel = channel;
    req.enc.volume = __this_record->gain;
    req.enc.sample_rate = sample_rate;
    req.enc.format = "pcm";
    req.enc.sample_source = __this_record->sample_source;
    req.enc.vfs_ops = &recorder_vfs_ops;
    req.enc.file = (FILE *)&__this_record->save_cbuf;
    if (channel == 1 && !strcmp(__this_record->sample_source, "mic") && (sample_rate == 8000 || sample_rate == 16000)) {
        req.enc.use_vad = 1; //打开VAD断句功能
        req.enc.dns_enable = 1; //打开降噪功能
        req.enc.vad_auto_refresh = 1; //VAD自动刷新
    }

    err = server_request(__this_record->enc_server, AUDIO_REQ_ENC, &req);
    if (err) {
        goto __err1;
    }

    return 0;

__err1:
    req.dec.cmd = AUDIO_DEC_STOP;
    server_request(__this_record->dec_server, AUDIO_REQ_DEC, &req);

__err:
    if (__this_record->cache_buf) {
        free(__this_record->cache_buf);
        __this_record->cache_buf = NULL;
    }

    __this_record->run_flag = 0;

    return -1;
}

//MIC或者LINEIN模拟直通到DAC，不需要软件参与
static int audio_adc_analog_direct_to_dac(int sample_rate, u8 channel)
{
    union audio_req req = {0};

    log_d("----------audio_adc_analog_direct_to_dac----------\n");

    __this_record->run_flag = 1;

    req.enc.cmd = AUDIO_ENC_OPEN;
    req.enc.channel = channel;
    req.enc.volume = __this_record->gain;
    req.enc.format = "pcm";
    req.enc.sample_source = __this_record->sample_source;
    req.enc.sample_rate = sample_rate;
    req.enc.direct2dac = 1;
    req.enc.high_gain = 1;
    if (channel == 4) {
        req.enc.channel_bit_map = 0x0f;
    } else if (channel == 2) {
        req.enc.channel_bit_map = BIT(CONFIG_AUDIO_ADC_CHANNEL_L) | BIT(CONFIG_AUDIO_ADC_CHANNEL_R);
    } else {
        req.enc.channel_bit_map = BIT(CONFIG_AUDIO_ADC_CHANNEL_L);
    }

    return server_request(__this_record->enc_server, AUDIO_REQ_ENC, &req);
}

//录音文件到SD卡
static int recorder_to_file(int sample_rate, u8 channel)
{
    union audio_req req = {0};

    __this_record->run_flag = 1;
    __this_record->direct = 0;

    char time_str[64] = {0};
    char file_name[100] = {0};
    u8 dir_len = 0;
    struct tm timeinfo = {0};
    time_t timestamp = time(NULL) + 28800;
    localtime_r(&timestamp, &timeinfo);
    strcpy(time_str, CONFIG_ROOT_PATH"RECORDER/\\U");
    dir_len = strlen(time_str);
    strftime(time_str + dir_len, sizeof(time_str) - dir_len, "%Y-%m-%dT%H-%M-%S.wav", &timeinfo);
    log_i("recorder file name : %s\n", time_str);

    memcpy(file_name, time_str, dir_len);

    for (u8 i = 0; i < strlen(time_str) - dir_len; ++i) {
        file_name[dir_len + i * 2] = time_str[dir_len + i];
    }

    req.enc.cmd = AUDIO_ENC_OPEN;
    req.enc.channel = channel;
    req.enc.volume = __this_record->gain;
    req.enc.frame_size = 8192;
    req.enc.output_buf_len = req.enc.frame_size * 10;
    req.enc.sample_rate = sample_rate;
    req.enc.format = "wav";
    req.enc.sample_source = __this_record->sample_source;
    req.enc.msec = CONFIG_AUDIO_RECORDER_DURATION;
    req.enc.file = __this_record->fp = fopen(file_name, "w+");
    /* req.enc.sample_depth = 24; //IIS支持采集24bit深度 */
    if (channel == 4) {
        req.enc.channel_bit_map = 0x0f;
    } else if (channel == 2) {
        req.enc.channel_bit_map = BIT(CONFIG_AUDIO_ADC_CHANNEL_L) | BIT(CONFIG_AUDIO_ADC_CHANNEL_R);
    } else {
        req.enc.channel_bit_map = BIT(CONFIG_AUDIO_ADC_CHANNEL_L);
    }

    return server_request(__this_record->enc_server, AUDIO_REQ_ENC, &req);
}

static void recorder_play_pause(void)
{
    union audio_req req = {0};

    req.dec.cmd = AUDIO_DEC_PP;
    server_request(__this_record->dec_server, AUDIO_REQ_DEC, &req);

    req.enc.cmd = AUDIO_ENC_PP;
    server_request(__this_record->enc_server, AUDIO_REQ_ENC, &req);

    if (__this_record->cache_buf) {
        cbuf_clear(&__this_record->save_cbuf);
    }
}

//调整ADC的模拟增益
static int recorder_enc_gain_change(int step)
{
    union audio_req req = {0};

    int gain = __this_record->gain + step;
    if (gain < 0) {
        gain = 0;
    } else if (gain > 100) {
        gain = 100;
    }
    if (gain == __this_record->gain) {
        return -1;
    }
    __this_record->gain = gain;

    if (!__this_record->enc_server) {
        return -1;
    }

    log_d("set_enc_gain: %d\n", gain);

    req.enc.cmd     = AUDIO_ENC_SET_VOLUME;
    req.enc.volume  = gain;
    return server_request(__this_record->enc_server, AUDIO_REQ_ENC, &req);
}

//调整DAC的数字音量和模拟音量
static int recorder_dec_volume_change(int step)
{
    union audio_req req = {0};

    int volume = __this_record->volume + step;
    if (volume < MIN_VOLUME_VALUE) {
        volume = MIN_VOLUME_VALUE;
    } else if (volume > MAX_VOLUME_VALUE) {
        volume = MAX_VOLUME_VALUE;
    }
    if (volume == __this_record->volume) {
        return -1;
    }
    __this_record->volume = volume;

    if (!__this_record->dec_server) {
        return -1;
    }

    log_d("set_dec_volume: %d\n", volume);

#ifdef CONFIG_STORE_VOLUME
    syscfg_write(CFG_MUSIC_VOL, &__this_record->volume, sizeof(__this_record->volume));
#endif

    req.dec.cmd     = AUDIO_DEC_SET_VOLUME;
    req.dec.volume  = volume;
    return server_request(__this_record->dec_server, AUDIO_REQ_DEC, &req);
}

static int recorder_mode_init(void)
{
    log_i("recorder_play_main\n");

    memset(__this_record, 0, sizeof(struct recorder_hdl));

#ifdef CONFIG_STORE_VOLUME
    if (syscfg_read(CFG_MUSIC_VOL, &__this_record->volume, sizeof(__this_record->volume)) < 0 ||
        __this_record->volume < MIN_VOLUME_VALUE || __this_record->volume > MAX_VOLUME_VALUE) {
        __this_record->volume = INIT_VOLUME_VALUE;
    }
#else
    __this_record->volume = INIT_VOLUME_VALUE;
#endif
    if (__this_record->volume < 0 || __this_record->volume > MAX_VOLUME_VALUE) {
        __this_record->volume = INIT_VOLUME_VALUE;
    }

#if CONFIG_AUDIO_ENC_SAMPLE_SOURCE == AUDIO_ENC_SAMPLE_SOURCE_PLNK0
    __this_record->sample_source = "plnk0";
#elif CONFIG_AUDIO_ENC_SAMPLE_SOURCE == AUDIO_ENC_SAMPLE_SOURCE_PLNK1
    __this_record->sample_source = "plnk1";
#elif CONFIG_AUDIO_ENC_SAMPLE_SOURCE == AUDIO_ENC_SAMPLE_SOURCE_IIS0
    __this_record->sample_source = "iis0";
#elif CONFIG_AUDIO_ENC_SAMPLE_SOURCE == AUDIO_ENC_SAMPLE_SOURCE_IIS1
    __this_record->sample_source = "iis1";
#elif CONFIG_AUDIO_ENC_SAMPLE_SOURCE == AUDIO_ENC_SAMPLE_SOURCE_LINEIN
    __this_record->sample_source = "linein";
#else
    __this_record->sample_source = "mic";
#endif

    __this_record->channel = CONFIG_AUDIO_RECORDER_CHANNEL;
    __this_record->gain = CONFIG_AUDIO_ADC_GAIN;
    __this_record->sample_rate = CONFIG_AUDIO_RECORDER_SAMPLERATE;

    __this_record->enc_server = server_open("audio_server", "enc");
    server_register_event_handler_to_task(__this_record->enc_server, NULL, enc_server_event_handler, "app_core");

    __this_record->dec_server = server_open("audio_server", "dec");

    return recorder_play_to_dac(__this_record->sample_rate, __this_record->channel);
}

static void recorder_mode_exit(void)
{
    recorder_close();
    server_close(__this_record->dec_server);
    __this_record->dec_server = NULL;
    server_close(__this_record->enc_server);
    __this_record->enc_server = NULL;
}

static int recorder_key_click(struct key_event *key)
{
    int ret = true;

    switch (key->value) {
    case KEY_OK:
#if CONFIG_AUDIO_ENC_SAMPLE_SOURCE == AUDIO_ENC_SAMPLE_SOURCE_LINEIN || CONFIG_AUDIO_ENC_SAMPLE_SOURCE == AUDIO_ENC_SAMPLE_SOURCE_MIC
        if (__this_record->direct) {
            if (__this_record->run_flag) {
                recorder_close();
            } else {
                audio_adc_analog_direct_to_dac(__this_record->sample_rate, __this_record->channel);
            }
        } else {
            recorder_play_pause();
        }
#else
        recorder_play_pause();
#endif
        break;
    case KEY_VOLUME_DEC:
        recorder_dec_volume_change(-VOLUME_STEP);
        break;
    case KEY_VOLUME_INC:
        recorder_dec_volume_change(VOLUME_STEP);
        break;
    default:
        ret = false;
        break;
    }

    return ret;
}

static int recorder_key_long(struct key_event *key)
{
    switch (key->value) {
#if CONFIG_AUDIO_ENC_SAMPLE_SOURCE == AUDIO_ENC_SAMPLE_SOURCE_LINEIN || CONFIG_AUDIO_ENC_SAMPLE_SOURCE == AUDIO_ENC_SAMPLE_SOURCE_MIC
    case KEY_OK:
        recorder_close();
        if (__this_record->direct) {
            recorder_play_to_dac(__this_record->sample_rate, __this_record->channel);
        } else {
            audio_adc_analog_direct_to_dac(__this_record->sample_rate, __this_record->channel);
        }
        __this_record->direct = !__this_record->direct;
        break;
    case KEY_VOLUME_DEC:
        recorder_enc_gain_change(-GAIN_STEP);
        break;
    case KEY_VOLUME_INC:
        recorder_enc_gain_change(GAIN_STEP);
        break;
#endif
    case KEY_MODE:
        if (storage_device_ready()) {
            recorder_close();
            recorder_to_file(__this_record->sample_rate, __this_record->channel);
        }
        break;
    default:
        break;
    }

    return true;
}

static int recorder_key_event_handler(struct key_event *key)
{
    switch (key->action) {
    case KEY_EVENT_CLICK:
        return recorder_key_click(key);
    case KEY_EVENT_LONG:
        return recorder_key_long(key);
    default:
        break;
    }

    return true;
}

static int recorder_event_handler(struct application *app, struct sys_event *event)
{
    switch (event->type) {
    case SYS_KEY_EVENT:
        return recorder_key_event_handler((struct key_event *)event->payload);
    default:
        return false;
    }
}

static int recorder_state_machine(struct application *app, enum app_state state, struct intent *it)
{
    switch (state) {
    case APP_STA_CREATE:
        break;
    case APP_STA_START:
        recorder_mode_init();
        break;
    case APP_STA_PAUSE:
        break;
    case APP_STA_RESUME:
        break;
    case APP_STA_STOP:
        recorder_mode_exit();
        break;
    case APP_STA_DESTROY:
        break;
    }

    return 0;
}

static const struct application_operation recorder_ops = {
    .state_machine  = recorder_state_machine,
    .event_handler 	= recorder_event_handler,
};

REGISTER_APPLICATION(recorder) = {
    .name 	= "recorder",
    .ops 	= &recorder_ops,
    .state  = APP_STA_DESTROY,
};

#endif


#if defined CONFIG_ASR_ALGORITHM_ENABLE && CONFIG_ASR_ALGORITHM == AISP_ALGORITHM

extern int aisp_open(u16 sample_rate);
extern void aisp_suspend(void);
extern void aisp_resume(void);
extern int aisp_close(void);

static int ai_speaker_mode_init(void)
{
    r_printf("===================%s======%d======\n\r", __func__, __LINE__);
    log_i("ai_speaker_play_main\n");

    return aisp_open(16000);
}

static void ai_speaker_mode_exit(void)
{
    aisp_close();
}

static int ai_speaker_key_click(struct key_event *key)
{
    int ret = true;

    switch (key->value) {
    case KEY_OK:
        break;
    default:
        ret = false;
        break;
    }

    return ret;
}

static int ai_speaker_key_long(struct key_event *key)
{
    return true;
}

static int ai_speaker_key_event_handler(struct key_event *key)
{
    switch (key->action) {
    case KEY_EVENT_CLICK:
        return ai_speaker_key_click(key);
    case KEY_EVENT_LONG:
        return ai_speaker_key_long(key);
    default:
        break;
    }

    return true;
}

static int ai_speaker_event_handler(struct application *app, struct sys_event *event)
{
    switch (event->type) {
    case SYS_KEY_EVENT:
        return ai_speaker_key_event_handler((struct key_event *)event->payload);
    default:
        return false;
    }
}

static int ai_speaker_state_machine(struct application *app, enum app_state state, struct intent *it)
{
    switch (state) {
    case APP_STA_CREATE:
        break;
    case APP_STA_START:
        ai_speaker_mode_init();
        break;
    case APP_STA_PAUSE:
        break;
    case APP_STA_RESUME:
        break;
    case APP_STA_STOP:
        ai_speaker_mode_exit();
        break;
    case APP_STA_DESTROY:
        break;
    }

    return 0;
}

static const struct application_operation ai_speaker_ops = {
    .state_machine  = ai_speaker_state_machine,
    .event_handler 	= ai_speaker_event_handler,
};

REGISTER_APPLICATION(ai_speaker) = {
    .name 	= "ai_speaker",
    .ops 	= &ai_speaker_ops,
    .state  = APP_STA_DESTROY,
};

#endif


#endif
