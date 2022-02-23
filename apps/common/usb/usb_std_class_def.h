#ifndef __USB_STD_CLASS_DEF_H__
#define __USB_STD_CLASS_DEF_H__

#ifndef USB_MALLOC_ENABLE
#define USB_MALLOC_ENABLE           0
#endif
#ifndef USB_HOST_ASYNC
#define USB_HOST_ASYNC              1
#endif
#ifndef USB_H_MALLOC_ENABLE
#define USB_H_MALLOC_ENABLE         1
#endif
#ifndef USB_DEVICE_CLASS_CONFIG
#define USB_DEVICE_CLASS_CONFIG     (SPEAKER_CLASS|MIC_CLASS|HID_CLASS|MASSSTORAGE_CLASS)
#endif

///////////MassStorage Class
#ifndef MSD_BULK_EP_OUT
#define MSD_BULK_EP_OUT             1
#endif
#ifndef MSD_BULK_EP_IN
#define MSD_BULK_EP_IN              1
#endif
#ifndef MAXP_SIZE_BULKOUT
#define MAXP_SIZE_BULKOUT           64
#endif
#ifndef MAXP_SIZE_BULKIN
#define MAXP_SIZE_BULKIN            64
#endif
#ifndef MSD_STR_INDEX
#define MSD_STR_INDEX               7
#endif

///////////HID class
#ifndef HID_EP_IN
#define HID_EP_IN                   2
#endif
#ifndef HID_EP_OUT
#define HID_EP_OUT                  2
#endif
#ifndef MAXP_SIZE_HIDOUT
#define MAXP_SIZE_HIDOUT            64
#endif
#ifndef MAXP_SIZE_HIDIN
#define MAXP_SIZE_HIDIN             64
#endif

/////////////Audio Class
#ifndef UAC_ISO_INTERVAL_1_0
#define UAC_ISO_INTERVAL_1_0        1
#endif
#ifndef UAC_ISO_INTERVAL_2_0
#define UAC_ISO_INTERVAL_2_0        4
#endif
//speaker class
#ifndef SPK_AUDIO_RATE
#define SPK_AUDIO_RATE              48000
#endif
#ifndef SPK_AUDIO_RES
#define SPK_AUDIO_RES               16
#endif
#ifndef SPK_CHANNEL
#define SPK_CHANNEL                 2
#endif
#ifndef SPK_FRAME_LEN
#define SPK_FRAME_LEN               (((SPK_AUDIO_RATE) * SPK_AUDIO_RES / 8 * SPK_CHANNEL)/1000+4)
#endif
#ifndef SPK_PCM_Type
#define SPK_PCM_Type                (SPK_AUDIO_RES >> 4)                // 0=8 ,1=16
#endif
#ifndef SPK_AUDIO_TYPE
#define SPK_AUDIO_TYPE              (0x02 - SPK_PCM_Type)           // TYPE1_PCM16
#endif
#ifndef SPK_ISO_EP_OUT
#define SPK_ISO_EP_OUT              3
#endif
#ifndef SPEAKER_STR_INDEX
#define SPEAKER_STR_INDEX           5
#endif
#ifndef SPK_INPUT_TERMINAL_ID
#define SPK_INPUT_TERMINAL_ID       1
#endif
#ifndef SPK_FEATURE_UNIT_ID
#define SPK_FEATURE_UNIT_ID         2
#endif
#ifndef SPK_OUTPUT_TERMINAL_ID
#define SPK_OUTPUT_TERMINAL_ID      3
#endif
#ifndef SPK_SELECTOR_UNIT_ID
#define SPK_SELECTOR_UNIT_ID        8
#endif

/////////////Microphone Class
#ifndef MIC_SamplingFrequency
#define MIC_SamplingFrequency       1
#endif

#if MIC_SamplingFrequency   == 1
#define MIC_AUDIO_RATE              48000
#else
#define MIC_AUDIO_RATE              192000
#define MIC_AUDIO_RATE_1            44100
#define MIC_AUDIO_RATE_2            48000
#define MIC_AUDIO_RATE_4            96000
#endif

#ifndef MIC_AUDIO_RES
#define MIC_AUDIO_RES               16
#endif
#ifndef MIC_CHANNEL
#define MIC_CHANNEL                 1
#endif
#ifndef MIC_FRAME_LEN
#define MIC_FRAME_LEN               ((MIC_AUDIO_RATE * MIC_AUDIO_RES / 8 * MIC_CHANNEL)/1000)
#endif
#ifndef MIC_PCM_TYPE
#define MIC_PCM_TYPE                (MIC_AUDIO_RES >> 4)                // 0=8 ,1=16
#endif
#ifndef MIC_AUDIO_TYPE
#define MIC_AUDIO_TYPE              (0x02 - MIC_PCM_TYPE)
#endif
#ifndef MIC_ISO_EP_IN
#define MIC_ISO_EP_IN               3
#endif
#ifndef MIC_STR_INDEX
#define MIC_STR_INDEX               6
#endif
#ifndef MIC_INPUT_TERMINAL_ID
#define MIC_INPUT_TERMINAL_ID       4
#endif
#ifndef MIC_FEATURE_UNIT_ID
#define MIC_FEATURE_UNIT_ID         5
#endif
#ifndef MIC_OUTPUT_TERMINAL_ID
#define MIC_OUTPUT_TERMINAL_ID      6
#endif
#ifndef MIC_SELECTOR_UNIT_ID
#define MIC_SELECTOR_UNIT_ID        7
#endif

////////////CDC Class
#ifndef CDC_DATA_EP_IN
#define CDC_DATA_EP_IN              2
#endif
#ifndef CDC_DATA_EP_OUT
#define CDC_DATA_EP_OUT             2
#endif
#ifndef CDC_INTR_EP_IN
#define CDC_INTR_EP_IN              3
#endif
#ifndef MAXP_SIZE_CDC_BULKIN
#define MAXP_SIZE_CDC_BULKIN        64
#endif
#ifndef MAXP_SIZE_CDC_BULKOUT
#define MAXP_SIZE_CDC_BULKOUT       64
#endif
#ifndef MAXP_SIZE_CDC_INTRIN
#define MAXP_SIZE_CDC_INTRIN        8
#endif
#ifndef CDC_INTR_EP_ENABLE
#define CDC_INTR_EP_ENABLE          0
#endif

///////////USB Video Class
#ifndef UVC_ISO_MODE
#define UVC_ISO_MODE                1
#endif
#ifndef UVC_STREAM_EP_IN
#define UVC_STREAM_EP_IN            4
#endif
#ifndef VIDEO_STATUS_EP_IN
#define VIDEO_STATUS_EP_IN          5
#endif
#ifndef VIDEO_STR_INDEX
#define VIDEO_STR_INDEX             8
#endif

#if UVC_ISO_MODE
#define UVC_FIFO_TXMAXP             768//1023
#define UVC_PKT_SPILT               1
#else
#define UVC_FIFO_TXMAXP             512
#define UVC_PKT_SPILT               1
#endif

#ifndef VIDEO_STATUS_TXMAXP
#define VIDEO_STATUS_TXMAXP         16
#endif

#endif
