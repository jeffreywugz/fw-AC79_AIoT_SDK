
# make 编译并下载
# make VERBOSE=1 显示编译详细过程
# make clean 清除编译临时文件
# 注意： Linux 下编译方式：
# 1. 从 http://pkgman.jieliapp.com/doc/all 处找到下载链接
# 2. 下载后，解压到 /opt/jieli 目录下，保证
#   /opt/jieli/common/bin/clang 存在（注意目录层次）
# 3. 确认 ulimit -n 的结果足够大（建议大于8096），否则链接可能会因为打开文件太多而失败
#   可以通过 ulimit -n 8096 来设置一个较大的值

# 工具路径设置
ifeq ($(OS), Windows_NT)
# Windows 下工具链位置
TOOL_DIR := C:/JL/pi32/bin
CC    := clang.exe
CXX   := clang.exe
LD    := pi32v2-lto-wrapper.exe
AR    := pi32v2-lto-ar.exe
MKDIR := mkdir_win -p
RM    := rm -rf

SYS_LIB_DIR := C:/JL/pi32/pi32v2-lib/r3
SYS_INC_DIR := C:/JL/pi32/pi32v2-include

## 后处理脚本
POST_SCRIPT     := ..\..\..\..\..\cpu\wl82\tools\download.bat
RUN_POST_SCRIPT := $(POST_SCRIPT)
else
# Linux 下工具链位置
TOOL_DIR := /opt/jieli/pi32v2/bin
CC    := clang
CXX   := clang++
LD    := lto-wrapper
AR    := lto-ar
MKDIR := mkdir -p
RM    := rm -rf

SYS_LIB_DIR := $(TOOL_DIR)/../lib/r3
SYS_INC_DIR := $(TOOL_DIR)/../include

## 后处理脚本
POST_SCRIPT     := ../../../../../cpu/wl82/tools/download.sh
RUN_POST_SCRIPT := bash $(POST_SCRIPT)
endif

CC  := $(TOOL_DIR)/$(CC)
CXX := $(TOOL_DIR)/$(CXX)
LD  := $(TOOL_DIR)/$(LD)
AR  := $(TOOL_DIR)/$(AR)
# 输出文件设置
OUT_ELF   := ../../../../../cpu/wl82/tools/sdk.elf
OBJ_FILE  := $(OUT_ELF).objs.txt
# 编译路径设置
BUILD_DIR := objs
# 工程路径前缀
ROOT_PREFIX := ../../../../..

# 编译参数设置
CFLAGS := \
	-target pi32v2 \
	-mcpu=r3 \
	-integrated-as \
	-flto \
	-Wuninitialized \
	-Wno-invalid-noreturn \
	-fno-common \
	-integrated-as \
	-Oz \
	-g \
	-flto \
	-fallow-pointer-null \
	-fprefer-gnu-section \
	-Wno-shift-negative-value \
	-Wframe-larger-than=2560 \
	-fms-extensions \
	-w \


# 宏定义
DEFINES := \
	-DSUPPORT_MS_EXTENSIONS \
	-DCONFIG_RELEASE_ENABLE \
	-DCONFIG_CPU_WL82 \
	-DCONFIG_JLFAT_ENABLE \
	-DCONFIG_FREE_RTOS_ENABLE \
	-DCONFIG_THREAD_ENABLE \
	-DCONFIG_AUDIO_ONCHIP \
	-D__GCC_PI32V2__ \
	-DCONFIG_NEW_ECC_ENABLE \
	-DCONFIG_EQ_SUPPORT_ASYNC \
	-DEQ_CORE_V1 \
	-DCONFIG_SFC_ENABLE \
	-DCONFIG_AUDIO_ENABLE \
	-DCONFIG_VIDEO_DEC_ENABLE \
	-DCONFIG_UI_ENABLE=1 \
	-DCONFIG_SD_ENABLE=1 \
	-DCONFIG_VIDEO_ENABLE=1 \
	-DCONFIG_USB_ENABLE=1 \
	-DCONFIG_NET_ENABLE=1 \
	-DCONFIG_BT_ENABLE=1 \
	-DEVENT_HANDLER_NUM_CONFIG=2 \
	-DEVENT_TOUCH_ENABLE_CONFIG=0 \
	-DEVENT_POOL_SIZE_CONFIG=256 \
	-DCONFIG_EVENT_KEY_MAP_ENABLE=0 \
	-DTIMER_POOL_NUM_CONFIG=15 \
	-DAPP_ASYNC_POOL_NUM_CONFIG=0 \
	-DVFS_ENABLE=1 \
	-DUSE_SDFILE_NEW=1 \
	-DSDFILE_STORAGE=1 \
	-DVFS_FILE_POOL_NUM_CONFIG=8 \
	-DVM_MAX_SIZE_CONFIG=32*1024 \
	-DVM_ITEM_MAX_NUM=256 \
	-DCONFIG_TWS_ENABLE \
	-DCONFIG_BTCTRLER_TASK_DEL_ENABLE \
	-DCONFIG_LMP_CONN_SUSPEND_ENABLE \
	-DCONFIG_LMP_REFRESH_ENCRYPTION_KEY_ENABLE \
	-DCONFIG_ITEM_FORMAT_VM \
	-DRT_USING_FINSH \
	-DFINSH_USING_SYMTAB \
	-DFINSH_USING_DESCRIPTION \
	-DFINSH_USING_HISTORY \
	-DFINSH_USING_MSH \
	-DFINSH_USING_MSH_DEFAULT \


# 头文件搜索路径
INCLUDES := \
	-I../../../../../include_lib \
	-I../../../../../include_lib/driver \
	-I../../../../../include_lib/driver/device \
	-I../../../../../include_lib/driver/cpu/wl82 \
	-I../../../../../include_lib/system \
	-I../../../../../include_lib/system/generic \
	-I../../../../../include_lib/btctrler \
	-I../../../../../include_lib/btctrler/port/wl82 \
	-I../../../../../include_lib/update \
	-I../../../../../include_lib/btstack/third_party/common \
	-I../../../../../include_lib/btstack/third_party/rcsp \
	-I../../../../../include_lib/utils \
	-I../../../../../include_lib/utils/syscfg \
	-I../../../../../include_lib/utils/event \
	-I../../../../../include_lib/media/media_develop \
	-I../../../../../include_lib/media/media_develop/media \
	-I../../../../../include_lib/media/media_develop/media/cpu/wl82 \
	-I../../../../../include_lib/media/media_develop/media/cpu/wl82/asm \
	-I../../../../../apps \
	-I../../../../../apps/demo/demo_DevKitBoard \
	-I../../../../../apps/demo/demo_DevKitBoard/include \
	-I../../../../../apps/common \
	-I../../../../../apps/common/include \
	-I../../../../../apps/common/config/include \
	-I../../../../../apps/common/system \
	-I../../../../../apps/common/usb/host \
	-I../../../../../apps/common/jl_math \
	-I../../../../../apps/common/example/system/server \
	-I../../../../../include_lib/server \
	-I../../../../../include_lib/driver/device/usb \
	-I../../../../../include_lib/driver/device/usb/host \
	-I../../../../../include_lib/driver/device/video \
	-I../../../../../include_lib/utils/ui \
	-I../../../../../include_lib/btstack \
	-I../../../../../include_lib/net \
	-I../../../../../include_lib/net/mbedtls_2_2_1 \
	-I../../../../../include_lib/net/wifi_module \
	-I../../../../../include_lib/net/wifie \
	-I../../../../../include_lib/net/lwip_2_1_3/lwip/src/include \
	-I../../../../../include_lib/net/lwip_2_1_3/lwip/src/include/lwip \
	-I../../../../../include_lib/net/lwip_2_1_3/lwip/port \
	-I../../../../../include_lib/net/lwip_2_1_3/lwip/app \
	-I../../../../../include_lib/net/lwip_2_1_3/lwip/app/ctp \
	-I../../../../../include_lib/net/lwip_2_1_3/lwip/app/cdp \
	-I../../../../../apps/common/net \
	-I../../../../../apps/common/net/testbox/include \
	-I../../../../../apps/common/ui/include \
	-I../../../../../apps/common/gsensor \
	-I../../../../../apps/common/example/third_party/FlashDB/port/fal/inc \
	-I../../../../../apps/common/example/third_party/FlashDB/inc \
	-I../../../../../apps/common/example/third_party/FlashDB/port/fal/samples/porting \
	-I../../../../../apps/common/usb \
	-I../../../../../apps/common/usb/device \
	-I../../../../../apps/common/usb/include/host \
	-I../../../../../apps/common/usb/include \
	-I../../../../../include_lib/driver/device/usb/device \
	-I$(SYS_INC_DIR) \


# 需要编译的 .c 文件
c_SRC_FILES := \
	../../../../../apps/common/asr/aisp/aisp.c \
	../../../../../apps/common/audio_music/audio_config.c \
	../../../../../apps/common/audio_music/audio_digital_vol.c \
	../../../../../apps/common/audio_music/eq_config_new.c \
	../../../../../apps/common/audio_music/music_decrypt.c \
	../../../../../apps/common/audio_music/usb_audio_api.c \
	../../../../../apps/common/camera/byd20a6/byd20a6.c \
	../../../../../apps/common/camera/byd30a2/byd30a2.c \
	../../../../../apps/common/camera/byd3720/byd3720.c \
	../../../../../apps/common/camera/byd3901/byd3901.c \
	../../../../../apps/common/camera/byd3a03/byd3a03.c \
	../../../../../apps/common/camera/gc0307_dvp/gc0307.c \
	../../../../../apps/common/camera/gc0308_dvp/gc0308.c \
	../../../../../apps/common/camera/gc0309_dvp/gc0309.c \
	../../../../../apps/common/camera/gc0310_spi/gc0310.c \
	../../../../../apps/common/camera/gc0312_dvp/gc0312.c \
	../../../../../apps/common/camera/gc0328_dvp/gc0328.c \
	../../../../../apps/common/camera/gc0329_dvp/gc0329.c \
	../../../../../apps/common/camera/gc2145_dvp/gc2145.c \
	../../../../../apps/common/camera/h7375s/h7375s.c \
	../../../../../apps/common/camera/ov7670_dvp/ov7670.c \
	../../../../../apps/common/camera/pr2000k/pr2000k.c \
	../../../../../apps/common/camera/xc7011_dvp/xc7011.c \
	../../../../../apps/common/config/bt_profile_config.c \
	../../../../../apps/common/config/ci_transport_uart.c \
	../../../../../apps/common/config/log_config/app_config.c \
	../../../../../apps/common/config/log_config/lib_btctrler_config.c \
	../../../../../apps/common/config/log_config/lib_btstack_config.c \
	../../../../../apps/common/config/log_config/lib_driver_config.c \
	../../../../../apps/common/config/log_config/lib_media_config.c \
	../../../../../apps/common/config/log_config/lib_system_config.c \
	../../../../../apps/common/config/log_config/lib_update_config.c \
	../../../../../apps/common/config/user_cfg.c \
	../../../../../apps/common/debug/debug.c \
	../../../../../apps/common/debug/debug_user.c \
	../../../../../apps/common/example/audio/audio_demo/ai_speaker.c \
	../../../../../apps/common/example/audio/audio_demo/local_music.c \
	../../../../../apps/common/example/audio/audio_demo/main.c \
	../../../../../apps/common/example/audio/audio_demo/net_music.c \
	../../../../../apps/common/example/audio/audio_demo/recorder.c \
	../../../../../apps/common/example/bluetooth/edr/bt_music/bt_decode.c \
	../../../../../apps/common/example/bluetooth/edr/bt_music/bt_emitter.c \
	../../../../../apps/common/example/bluetooth/edr/bt_music/main.c \
	../../../../../apps/common/example/network_protocols/coap/coap_client/main.c \
	../../../../../apps/common/example/network_protocols/coap/coap_server/main.c \
	../../../../../apps/common/example/network_protocols/http/http_download/main.c \
	../../../../../apps/common/example/network_protocols/http/http_post_get_put/main.c \
	../../../../../apps/common/example/network_protocols/http/http_server/main.c \
	../../../../../apps/common/example/network_protocols/http/user_http_head/post_sd/main.c \
	../../../../../apps/common/example/network_protocols/mqtt/main.c \
	../../../../../apps/common/example/network_protocols/sockets/tcp/client/main.c \
	../../../../../apps/common/example/network_protocols/sockets/tcp/server/main.c \
	../../../../../apps/common/example/network_protocols/sockets/udp/server/main.c \
	../../../../../apps/common/example/network_protocols/websocket/main.c \
	../../../../../apps/common/example/peripheral/adc/adc_test.c \
	../../../../../apps/common/example/peripheral/flash/flash_api/flash_test.c \
	../../../../../apps/common/example/peripheral/flash/flash_api/main.c \
	../../../../../apps/common/example/peripheral/flash/flash_opt/main.c \
	../../../../../apps/common/example/peripheral/flash/flash_register/main.c \
	../../../../../apps/common/example/peripheral/flash/flash_syscfg/main.c \
	../../../../../apps/common/example/peripheral/flash/flash_user/main.c \
	../../../../../apps/common/example/peripheral/gpcnt/gpcnt_test.c \
	../../../../../apps/common/example/peripheral/gpio/gpio.c \
	../../../../../apps/common/example/peripheral/gsensor/main.c \
	../../../../../apps/common/example/peripheral/pwm/main.c \
	../../../../../apps/common/example/peripheral/pwm/pwm_led.c \
	../../../../../apps/common/example/peripheral/timer/test1.c \
	../../../../../apps/common/example/peripheral/timer/test2.c \
	../../../../../apps/common/example/peripheral/uart/uart_test.c \
	../../../../../apps/common/example/peripheral/uart/uart_tx_rx_test.c \
	../../../../../apps/common/example/sensor/hc_sr04/main.c \
	../../../../../apps/common/example/storage/sd_fs/main.c \
	../../../../../apps/common/example/storage/sd_fs/main1.c \
	../../../../../apps/common/example/system/BenchMark/Linpack/linpack.c \
	../../../../../apps/common/example/system/BenchMark/dhrystone/dhry21a.c \
	../../../../../apps/common/example/system/BenchMark/dhrystone/dhry21b.c \
	../../../../../apps/common/example/system/BenchMark/main.c \
	../../../../../apps/common/example/system/BenchMark/pi_css5_src/fftsg_h.c \
	../../../../../apps/common/example/system/BenchMark/pi_css5_src/pi_fftcs.c \
	../../../../../apps/common/example/system/BenchMark/whetstone/whetstone.c \
	../../../../../apps/common/example/system/ai_sdk/ai_test_server.c \
	../../../../../apps/common/example/system/ai_sdk/main.c \
	../../../../../apps/common/example/system/ai_sdk/test_ai.c \
	../../../../../apps/common/example/system/app_state_machine/main.c \
	../../../../../apps/common/example/system/cbuf/main.c \
	../../../../../apps/common/example/system/crypto/main.c \
	../../../../../apps/common/example/system/event/main.c \
	../../../../../apps/common/example/system/exception/main.c \
	../../../../../apps/common/example/system/hw_float/main.c \
	../../../../../apps/common/example/system/init_call/main.c \
	../../../../../apps/common/example/system/interrupt/main.c \
	../../../../../apps/common/example/system/lbuf/main.c \
	../../../../../apps/common/example/system/long_press_reset/main.c \
	../../../../../apps/common/example/system/malloc/main.c \
	../../../../../apps/common/example/system/math/main.c \
	../../../../../apps/common/example/system/memory/main.c \
	../../../../../apps/common/example/system/os/os_api/main.c \
	../../../../../apps/common/example/system/os/os_api/mutex_test.c \
	../../../../../apps/common/example/system/os/os_api/queue_test.c \
	../../../../../apps/common/example/system/os/os_api/sem_test.c \
	../../../../../apps/common/example/system/os/os_api/static_task_test.c \
	../../../../../apps/common/example/system/os/os_api/thread_can_not_kill_test.c \
	../../../../../apps/common/example/system/os/pthreads_api/main.c \
	../../../../../apps/common/example/system/printf/main.c \
	../../../../../apps/common/example/system/server/main.c \
	../../../../../apps/common/example/system/server/test_server.c \
	../../../../../apps/common/example/system/soft_power_off/main.c \
	../../../../../apps/common/example/system/system_reset_reason/main.c \
	../../../../../apps/common/example/system/system_timer/sys_timeout_test.c \
	../../../../../apps/common/example/system/system_timer/sys_timer_test.c \
	../../../../../apps/common/example/system/system_timer/usr_timeout_test.c \
	../../../../../apps/common/example/system/system_timer/usr_timer_test.c \
	../../../../../apps/common/example/system/wait_completion/main.c \
	../../../../../apps/common/example/third_party/FlashDB/port/fal/samples/porting/fal_AC79.c \
	../../../../../apps/common/example/third_party/FlashDB/port/fal/samples/porting/fal_AC79_dev.c \
	../../../../../apps/common/example/third_party/FlashDB/port/fal/src/fal.c \
	../../../../../apps/common/example/third_party/FlashDB/port/fal/src/fal_flash.c \
	../../../../../apps/common/example/third_party/FlashDB/port/fal/src/fal_partition.c \
	../../../../../apps/common/example/third_party/FlashDB/samples/kvdb_basic_sample.c \
	../../../../../apps/common/example/third_party/FlashDB/samples/kvdb_type_blob_sample.c \
	../../../../../apps/common/example/third_party/FlashDB/samples/kvdb_type_string_sample.c \
	../../../../../apps/common/example/third_party/FlashDB/samples/tsdb_sample.c \
	../../../../../apps/common/example/third_party/FlashDB/src/fdb.c \
	../../../../../apps/common/example/third_party/FlashDB/src/fdb_kvdb.c \
	../../../../../apps/common/example/third_party/FlashDB/src/fdb_tsdb.c \
	../../../../../apps/common/example/third_party/FlashDB/src/fdb_utils.c \
	../../../../../apps/common/example/third_party/FlashDB/tests/flashDB_test.c \
	../../../../../apps/common/example/third_party/finsh/cmd.c \
	../../../../../apps/common/example/third_party/finsh/finsh_compiler.c \
	../../../../../apps/common/example/third_party/finsh/finsh_error.c \
	../../../../../apps/common/example/third_party/finsh/finsh_heap.c \
	../../../../../apps/common/example/third_party/finsh/finsh_init.c \
	../../../../../apps/common/example/third_party/finsh/finsh_node.c \
	../../../../../apps/common/example/third_party/finsh/finsh_ops.c \
	../../../../../apps/common/example/third_party/finsh/finsh_parser.c \
	../../../../../apps/common/example/third_party/finsh/finsh_token.c \
	../../../../../apps/common/example/third_party/finsh/finsh_var.c \
	../../../../../apps/common/example/third_party/finsh/finsh_vm.c \
	../../../../../apps/common/example/third_party/finsh/msh.c \
	../../../../../apps/common/example/third_party/finsh/msh_file.c \
	../../../../../apps/common/example/third_party/finsh/shell.c \
	../../../../../apps/common/example/third_party/finsh/symbol.c \
	../../../../../apps/common/example/ui/ui_touch_demo.c \
	../../../../../apps/common/example/update/ftp_upgrade/main.c \
	../../../../../apps/common/example/update/http_upgrade/example1/main.c \
	../../../../../apps/common/example/update/http_upgrade/example2/main.c \
	../../../../../apps/common/example/update/sd_udisk_upgrade/main.c \
	../../../../../apps/common/example/video/camera/dvp/main.c \
	../../../../../apps/common/example/video/image/avi/main.c \
	../../../../../apps/common/example/video/image/gif/main.c \
	../../../../../apps/common/example/wifi/low_power/main.c \
	../../../../../apps/common/example/wifi/wifi_music/main.c \
	../../../../../apps/common/gsensor/da380.c \
	../../../../../apps/common/gsensor/gSensor_manage.c \
	../../../../../apps/common/gsensor/gma301.c \
	../../../../../apps/common/gsensor/msa300.c \
	../../../../../apps/common/gsensor/stk8312.c \
	../../../../../apps/common/iic/iic.c \
	../../../../../apps/common/iic/software_iic.c \
	../../../../../apps/common/jl_math/flfft_core_pi32v2.c \
	../../../../../apps/common/jl_math/jl_fft.c \
	../../../../../apps/common/jl_math/jl_math.c \
	../../../../../apps/common/jl_math/kiss_fft.c \
	../../../../../apps/common/jl_math/nn_function_vec.c \
	../../../../../apps/common/key/adkey.c \
	../../../../../apps/common/key/adkey_rtcvdd.c \
	../../../../../apps/common/key/ctmu_touch_key.c \
	../../../../../apps/common/key/iokey.c \
	../../../../../apps/common/key/irkey.c \
	../../../../../apps/common/key/key_driver.c \
	../../../../../apps/common/key/matrix_keyboard.c \
	../../../../../apps/common/key/rdec_key.c \
	../../../../../apps/common/key/slidekey.c \
	../../../../../apps/common/key/tent600_key.c \
	../../../../../apps/common/key/touch_key.c \
	../../../../../apps/common/key/uart_key.c \
	../../../../../apps/common/net/assign_macaddr.c \
	../../../../../apps/common/net/config_network.c \
	../../../../../apps/common/net/iperf_test.c \
	../../../../../apps/common/net/platform_cfg.c \
	../../../../../apps/common/net/testbox/client.c \
	../../../../../apps/common/net/testbox/fsm.c \
	../../../../../apps/common/net/testbox/wbcp.c \
	../../../../../apps/common/net/voiceprint_cfg.c \
	../../../../../apps/common/net/wifi_conf.c \
	../../../../../apps/common/system/device_mount.c \
	../../../../../apps/common/system/init.c \
	../../../../../apps/common/system/system_reset_reason.c \
	../../../../../apps/common/system/version.c \
	../../../../../apps/common/ui/GUI_GIF.c \
	../../../../../apps/common/ui/font/fontinit.c \
	../../../../../apps/common/ui/gif_api.c \
	../../../../../apps/common/ui/lcd/lcd_ui_api.c \
	../../../../../apps/common/ui/lcd_driver/lcd_480x272_8bits.c \
	../../../../../apps/common/ui/lcd_driver/lcd_drive.c \
	../../../../../apps/common/ui/lcd_driver/lcd_hx8357.c \
	../../../../../apps/common/ui/lcd_driver/lcd_ili9341.c \
	../../../../../apps/common/ui/lcd_driver/lcd_ili9488.c \
	../../../../../apps/common/ui/lcd_driver/lcd_st7735s.c \
	../../../../../apps/common/ui/lcd_driver/lcd_st7789s.c \
	../../../../../apps/common/ui/lcd_driver/lcd_st7789v.c \
	../../../../../apps/common/ui/lcd_driver/te_driver/lcd_data_driver.c \
	../../../../../apps/common/ui/lcd_driver/te_driver/lcd_te_driver.c \
	../../../../../apps/common/ui/touch_panel/gt911.c \
	../../../../../apps/common/ui/ui_action_video.c \
	../../../../../apps/common/ui/ui_driver/interface/ui_platform.c \
	../../../../../apps/common/update/net_update.c \
	../../../../../apps/common/update/update.c \
	../../../../../apps/common/usb/device/cdc.c \
	../../../../../apps/common/usb/device/descriptor.c \
	../../../../../apps/common/usb/device/hid.c \
	../../../../../apps/common/usb/device/msd.c \
	../../../../../apps/common/usb/device/msd_upgrade.c \
	../../../../../apps/common/usb/device/ram_disk_circle.c \
	../../../../../apps/common/usb/device/ram_disk_update.c \
	../../../../../apps/common/usb/device/task_pc.c \
	../../../../../apps/common/usb/device/uac.c \
	../../../../../apps/common/usb/device/uac_stream.c \
	../../../../../apps/common/usb/device/usb_device.c \
	../../../../../apps/common/usb/device/user_setup.c \
	../../../../../apps/common/usb/device/uvc.c \
	../../../../../apps/common/usb/host/adb.c \
	../../../../../apps/common/usb/host/aoa.c \
	../../../../../apps/common/usb/host/audio.c \
	../../../../../apps/common/usb/host/hid.c \
	../../../../../apps/common/usb/host/uac_host_demo.c \
	../../../../../apps/common/usb/host/usb_bulk_transfer.c \
	../../../../../apps/common/usb/host/usb_ctrl_transfer.c \
	../../../../../apps/common/usb/host/usb_host.c \
	../../../../../apps/common/usb/host/usb_storage.c \
	../../../../../apps/common/usb/host/usb_video.c \
	../../../../../apps/common/usb/host/usbnet.c \
	../../../../../apps/common/usb/host/uvc_host.c \
	../../../../../apps/common/usb/usb_config.c \
	../../../../../apps/common/usb/usb_epbuf_manager.c \
	../../../../../apps/common/usb/usb_host_config.c \
	../../../../../apps/demo/demo_DevKitBoard/app_main.c \
	../../../../../apps/demo/demo_DevKitBoard/board/wl82/DevKitBoard.c \
	../../../../../apps/demo/demo_DevKitBoard/get_yuv_data.c \
	../../../../../apps/demo/demo_DevKitBoard/jpeg2yuv.c \
	../../../../../apps/demo/demo_DevKitBoard/simple_avi_unpkg.c \
	../../../../../apps/demo/demo_DevKitBoard/spp_trans_data.c \
	../../../../../apps/demo/demo_DevKitBoard/wifi_demo_task.c \
	../../../../../cpu/wl82/debug.c \
	../../../../../cpu/wl82/iic.c \
	../../../../../cpu/wl82/key/adc_api.c \
	../../../../../cpu/wl82/key/irflt.c \
	../../../../../cpu/wl82/port_waked_up.c \
	../../../../../cpu/wl82/setup.c \


# 需要编译的 .S 文件
S_SRC_FILES :=


# 需要编译的 .s 文件
s_SRC_FILES :=


# 需要编译的 .cpp 文件
cpp_SRC_FILES :=


# 链接参数
LFLAGS := \
	--plugin-opt=-pi32v2-always-use-itblock=false \
	--plugin-opt=-enable-ipra=true \
	--plugin-opt=-pi32v2-merge-max-offset=4096 \
	--plugin-opt=-pi32v2-enable-simd=true \
	--plugin-opt=mcpu=r3 \
	--plugin-opt=-global-merge-on-const \
	--plugin-opt=-inline-threshold=5 \
	--plugin-opt=-inline-max-allocated-size=32 \
	--plugin-opt=-inline-normal-into-special-section=true \
	--plugin-opt=-dont-used-symbol-list=malloc,free,sprintf,printf,puts,putchar \
	--plugin-opt=save-temps \
	--plugin-opt=-pi32v2-enable-rep-memop \
	--plugin-opt=-warn-stack-size=2560 \
	--sort-common \
	--dont-complain-call-overflow \
	--plugin-opt=-used-symbol-file=../../../../../cpu/wl82/sdk_used_list.used \
	--plugin-opt=-pi32v2-large-program=true \
	--start-group \
	../../../../../cpu/wl82/liba/fs.a \
	../../../../../cpu/wl82/liba/event.a \
	../../../../../cpu/wl82/liba/system.a \
	../../../../../cpu/wl82/liba/cfg_tool.a \
	../../../../../cpu/wl82/liba/cpu.a \
	../../../../../cpu/wl82/liba/common_lib.a \
	../../../../../cpu/wl82/liba/libaec.a \
	../../../../../cpu/wl82/liba/wl_rf_common.a \
	../../../../../cpu/wl82/liba/btctrler.a \
	../../../../../cpu/wl82/liba/btstack.a \
	../../../../../cpu/wl82/liba/crypto_toolbox_Osize.a \
	../../../../../cpu/wl82/liba/audio_server.a \
	../../../../../cpu/wl82/liba/lib_m4a_dec_seek.a \
	../../../../../cpu/wl82/liba/system.a  \
	../../../../../cpu/wl82/liba/video_rec_server.a \
	../../../../../cpu/wl82/liba/echo_server.a \
	../../../../../cpu/wl82/liba/libspeex.a \
	../../../../../cpu/wl82/liba/libvad.a \
	../../../../../cpu/wl82/liba/lib_aac_enc.a \
	../../../../../cpu/wl82/liba/lib_m4a_dec.a \
	../../../../../cpu/wl82/liba/lib_mp3_dec.a \
	../../../../../cpu/wl82/liba/lib_mp3_enc.a \
	../../../../../cpu/wl82/liba/lib_opus_enc.a \
	../../../../../cpu/wl82/liba/lib_opus_dec.a \
	../../../../../cpu/wl82/liba/lib_wma_dec.a \
	../../../../../cpu/wl82/liba/lib_ape_dec.a \
	../../../../../cpu/wl82/liba/lib_flac_dec.a \
	../../../../../cpu/wl82/liba/lib_wav_dec.a \
	../../../../../cpu/wl82/liba/lib_dts_dec.a \
	../../../../../cpu/wl82/liba/lib_amr_dec.a \
	../../../../../cpu/wl82/liba/lib_amr_enc.a \
	../../../../../cpu/wl82/liba/lib_amrwb_enc.a \
	../../../../../cpu/wl82/liba/lib_adpcm_enc.a \
	../../../../../cpu/wl82/liba/lib_pitchshifter.a \
	../../../../../cpu/wl82/liba/lib_rmono2stereo.a \
	../../../../../cpu/wl82/liba/lib_reverb_cal.a \
	../../../../../cpu/wl82/liba/lib_howling.a \
	../../../../../cpu/wl82/liba/lib_pitch_speed.a \
	../../../../../cpu/wl82/liba/liblimiter.a \
	../../../../../cpu/wl82/liba/libdns.a \
	../../../../../cpu/wl82/liba/lib_usb_syn.a \
	../../../../../cpu/wl82/liba/libaisp_single.a \
	../../../../../cpu/wl82/liba/libauth_aisp.a \
	../../../../../cpu/wl82/liba/libauth.a \
	../../../../../cpu/wl82/liba/aisIntlib_single_xiaoai.a \
	../../../../../cpu/wl82/liba/cJSON.a \
	../../../../../cpu/wl82/liba/update.a \
	../../../../../cpu/wl82/liba/video_dec_server.a \
	../../../../../cpu/wl82/liba/net_server.a \
	../../../../../cpu/wl82/liba/libcompressor.a \
	../../../../../cpu/wl82/liba/lib_crossover_coff.a \
	../../../../../cpu/wl82/liba/media_app.a \
	../../../../../cpu/wl82/liba/hsm.a \
	../../../../../cpu/wl82/liba/wpasupplicant.a \
	../../../../../cpu/wl82/liba/http_cli.a \
	../../../../../cpu/wl82/liba/http_server.a \
	../../../../../cpu/wl82/liba/ftpserver.a \
	../../../../../cpu/wl82/liba/websocket.a \
	../../../../../cpu/wl82/liba/lib_mqtt.a \
	../../../../../cpu/wl82/liba/libcoap.a \
	../../../../../cpu/wl82/liba/json.a \
	../../../../../cpu/wl82/liba/iperf.a \
	../../../../../cpu/wl82/liba/profile.a \
	../../../../../cpu/wl82/liba/libmbedtls_2_2_1.a \
	../../../../../cpu/wl82/liba/network_download.a \
	../../../../../cpu/wl82/liba/libmpeg.a \
	../../../../../cpu/wl82/liba/libdlna.a \
	../../../../../cpu/wl82/liba/ai_server.a \
	../../../../../cpu/wl82/liba/libmbedtls_2_2_1.a  \
	../../../../../cpu/wl82/liba/lwip_2_1_3.a \
	../../../../../cpu/wl82/liba/wl_wifi.a \
	--end-group \
	../../../../../cpu/wl82/liba/video_dec_server.a  \
	../../../../../cpu/wl82/liba/ui.a \
	../../../../../cpu/wl82/liba/res.a \
	../../../../../cpu/wl82/liba/font.a \
	../../../../../cpu/wl82/liba/ui_draw.a \
	-T../../../../../cpu/wl82/sdk.ld \
	-M=../../../../../cpu/wl82/tools/sdk.map \
	--plugin-opt=mcpu=r3 \
	--plugin-opt=-mattr=+fprev1 \


LIBPATHS := \
	-L$(SYS_LIB_DIR) \


LIBS := \
	$(SYS_LIB_DIR)/libm.a \
	$(SYS_LIB_DIR)/libc.a \
	$(SYS_LIB_DIR)/libm.a \
	$(SYS_LIB_DIR)/libcompiler-rt.a \



c_OBJS    := $(c_SRC_FILES:%.c=%.c.o)
S_OBJS    := $(S_SRC_FILES:%.S=%.S.o)
s_OBJS    := $(s_SRC_FILES:%.s=%.s.o)
cpp_OBJS  := $(cpp_SRC_FILES:%.cpp=%.cpp.o)

OBJS      := $(c_OBJS) $(S_OBJS) $(s_OBJS) $(cpp_OBJS)
DEP_FILES := $(OBJS:%.o=%.d)


OBJS      := $(addprefix $(BUILD_DIR)/, $(OBJS:$(ROOT_PREFIX)/%=%))
DEP_FILES := $(addprefix $(BUILD_DIR)/, $(DEP_FILES:$(ROOT_PREFIX)/%=%))


VERBOSE ?= 0
ifeq ($(VERBOSE), 1)
QUITE :=
else
QUITE := @
endif

LINK_AT ?= 1 # 一些旧的 make 不支持 file 函数，需要 make 的时候指定 LINK_AT=0 make

.PHONY: all clean pre_build

all: pre_build $(OUT_ELF)
	@echo +POST-BUILD
	$(QUITE) $(RUN_POST_SCRIPT) sdk

pre_build:
	@echo +PRE-BUILD
	$(QUITE) $(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -D__LD__ -E -P ../../../../../cpu/wl82/sdk_used_list.c -o ../../../../../cpu/wl82/sdk_used_list.used
	$(QUITE) $(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -D__LD__ -E -P ../../../../../cpu/wl82/sdk_ld.c -o ../../../../../cpu/wl82/sdk.ld

clean:
	$(QUITE) $(RM) $(OUT_ELF)
	$(QUITE) $(RM) $(OBJS)
	$(QUITE) $(RM) $(DEP_FILES)

$(OUT_ELF): $(OBJS)
	@echo +LINK $@
ifeq ($(LINK_AT), 1)
	$(file >$(OBJ_FILE), $(OBJS))
	$(QUITE) $(LD) -o $(OUT_ELF) @$(OBJ_FILE) $(LFLAGS) $(LIBPATHS) $(LIBS)
else
	$(QUITE) $(LD) -o $(OUT_ELF) $(OBJS) $(LFLAGS) $(LIBPATHS) $(LIBS)
endif

$(BUILD_DIR)/%.c.o : $(ROOT_PREFIX)/%.c
	@echo +CC $<
	@$(MKDIR) $(@D)
	@$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -MM -MT "$@" $< -o $(@:.o=.d)
	$(QUITE) $(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/%.S.o : $(ROOT_PREFIX)/%.S
	@echo +AS $<
	@$(MKDIR) $(@D)
	@$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -MM -MT "$@" $< -o $(@:.o=.d)
	$(QUITE) $(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/%.s.o : $(ROOT_PREFIX)/%.s
	@echo +AS $<
	@$(MKDIR) $(@D)
	@$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -MM -MT "$@" $< -o $(@:.o=.d)
	$(QUITE) $(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/%.cpp.o : $(ROOT_PREFIX)/%.cpp
	@echo +CXX $<
	@$(MKDIR) $(@D)
	@$(CXX) $(CFLAGS) $(DEFINES) $(INCLUDES) -MM -MT "$@" $< -o $(@:.o=.d)
	$(QUITE) $(CXX) $(CFLAGS) $(DEFINES) $(INCLUDES) -c $< -o $@

-include $(DEP_FILES)
