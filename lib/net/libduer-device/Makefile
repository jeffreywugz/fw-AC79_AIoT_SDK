AR_OUT = $(AR_DIR)/libduer-device.a

CC_DEFINE +=\
  -D_POSIX_SOURCE \
  -D_SYS_SELECT_H \
  # -DYOTTA_CFG_MBED_TRACE \
  -D YOTTA_CFG_MBED_TRACE_FEA_IPV6=0 \
  -DMBED_CONF_MBED_TRACE_ENABLE \

ifeq ($(ARCH),pi32v2)
CC_ARGS += \
-ffunction-sections \
-Wno-unused-value \
-Os \

endif

includes = \
-I$(ROOT)/include_lib \
-I$(ROOT)/include_lib/utils \
-I$(ROOT)/include_lib/system \
-I$(ROOT)/include_lib/system/generic \
-I$(ROOT)/include_lib/system/os \
-I$(ROOT)/include_lib/system/device \
-I$(ROOT)/include_lib/driver \
-I$(ROOT)/include_lib/driver/cpu/$(CPU) \
-I$(ROOT)/include_lib/net \
-I$(ROOT)/include_lib/net/$(MBEDTLS_DIR) \
-I$(ROOT)/include_lib/net/$(MBEDTLS_DIR)/mbedtls \
-I$(ROOT)/include_lib/net/$(MBEDTLS_DIR)/mbedtls/configs \
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/app/sock_api \
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/src/include \
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/port \
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/app \
-I$(ROOT)/include_lib/net/zliblite \
-I$(ROOT)/lib/net/libduer-device/external/baidu_json \
-I$(ROOT)/lib/net/libduer-device/external/mbed-client-c-port \
-I$(ROOT)/lib/net/libduer-device/external/mbed-client-c/nsdl-c \
-I$(ROOT)/lib/net/libduer-device/external/mbed-client-c/source/libCoap/src/include \
-I$(ROOT)/lib/net/libduer-device/external/mbed-client-c/source/libNsdl/src/include \
-I$(ROOT)/lib/net/libduer-device/external/mbed-trace \
-I$(ROOT)/lib/net/libduer-device/platform/include \
-I$(ROOT)/lib/net/libduer-device/platform/source-jl \
-I$(ROOT)/lib/net/libduer-device/framework/include \
-I$(ROOT)/lib/net/libduer-device/framework/core \
-I$(ROOT)/lib/net/libduer-device/framework/utils \
-I$(ROOT)/lib/net/libduer-device/modules/coap \
-I$(ROOT)/lib/net/libduer-device/modules/connagent \
-I$(ROOT)/lib/net/libduer-device/modules/dcs \
-I$(ROOT)/lib/net/libduer-device/modules/Device_Info \
-I$(ROOT)/lib/net/libduer-device/modules/HTTP \
-I$(ROOT)/lib/net/libduer-device/modules/ntp \
-I$(ROOT)/lib/net/libduer-device/modules/OTA/Decompression \
-I$(ROOT)/lib/net/libduer-device/modules/OTA/Downloader \
-I$(ROOT)/lib/net/libduer-device/modules/OTA/Installer \
-I$(ROOT)/lib/net/libduer-device/modules/OTA/Notifier \
-I$(ROOT)/lib/net/libduer-device/modules/OTA/Unpacker \
-I$(ROOT)/lib/net/libduer-device/modules/OTA/Updater \
-I$(ROOT)/lib/net/libduer-device/modules/OTA/Verifier \
-I$(ROOT)/lib/net/libduer-device/modules/voice_engine \
-I$(ROOT)/lib/net/libduer-device/modules/device_status \
-I$(ROOT)/lib/net/libduer-device/modules/System_Info \
-I$(ROOT)/lib/net/libduer-device/modules/bind_device \
-I$(ROOT)/lib/net/libduer-device/examples/dcs3.0-demo \

objs = \
./external/baidu_json/baidu_json.o \
./external/mbed-trace/source/mbed_trace.o \
./external/mbed-client-c/source/libCoap/src/sn_coap_builder.o \
./external/mbed-client-c/source/libCoap/src/sn_coap_header_check.o \
./external/mbed-client-c/source/libCoap/src/sn_coap_parser.o \
./external/mbed-client-c/source/libCoap/src/sn_coap_protocol.o \
./external/mbed-client-c/source/libNsdl/src/sn_grs.o \
./external/mbed-client-c/source/libNsdl/src/sn_nsdl.o \
./external/mbed-client-c-port/ns_list.o \
./framework/core/lightduer_timestamp.o \
./framework/core/lightduer_thread.o \
./framework/core/lightduer_sleep.o \
./framework/core/lightduer_random.o \
./framework/core/lightduer_net_transport_wrapper.o \
./framework/core/lightduer_net_transport.o \
./framework/core/lightduer_net_trans_encrypted.o \
./framework/core/lightduer_net_trans_aes_cbc_encrypted.o \
./framework/core/lightduer_mutex.o \
./framework/core/lightduer_memory.o \
./framework/core/lightduer_debug.o \
./framework/utils/lightduer_hashcode.o \
./framework/utils/lightduer_net_util.o \
./framework/utils/lightduer_aes.o \
./framework/utils/lightduer_queue_cache.o \
./framework/utils/lightduer_snprintf.o \
./framework/utils/lightduer_bitmap.o \
./framework/utils/lightduer_handler.o \
./modules/coap/lightduer_coap.o \
./modules/coap/lightduer_coap_ep.o \
./modules/coap/lightduer_coap_trace.o \
./modules/coap/lightduer_nsdl_adapter.o \
./modules/connagent/lightduer_ca.o \
./modules/connagent/lightduer_ca_conf.o \
./modules/connagent/lightduer_connagent.o \
./modules/connagent/lightduer_engine.o \
./modules/connagent/lightduer_event_emitter.o \
./modules/connagent/lightduer_data_cache.o \
./modules/dcs/lightduer_dcs_router.o \
./modules/dcs/lightduer_dcs_alert.o \
./modules/dcs/lightduer_dcs_audio_player.o \
./modules/dcs/lightduer_dcs_dummy.o \
./modules/dcs/lightduer_dcs_local.o \
./modules/dcs/lightduer_dcs_system.o \
./modules/dcs/lightduer_dcs_playback_control.o \
./modules/dcs/lightduer_dcs_speaker_control.o \
./modules/dcs/lightduer_dcs_voice_input.o \
./modules/dcs/lightduer_dcs_voice_output.o \
./modules/dcs/lightduer_dcs_screen.o \
./modules/dcs/lightduer_dcs_device_control.o \
./modules/dcs/lightduer_dcs_miscellaneous.o \
./modules/Device_Info/lightduer_dev_info.o \
./modules/HTTP/lightduer_http_client.o \
./modules/HTTP/lightduer_http_client_ops.o \
./modules/HTTP/lightduer_http_dns_client_ops.o \
./modules/interactive_class/lightduer_interactive_class.o \
./modules/ntp/lightduer_net_ntp.o \
./modules/voice_engine/lightduer_voice.o \
./modules/voice_engine/lightduer_session.o \
./modules/voice_engine/lightduer_speex.o \
./modules/device_status/lightduer_ds_log.o \
./modules/device_status/lightduer_ds_log_audio.o \
./modules/device_status/lightduer_ds_log_ca.o \
./modules/device_status/lightduer_ds_log_dcs.o \
./modules/device_status/lightduer_ds_report.o \
./modules/device_status/lightduer_ds_log_http.o \
./modules/device_status/lightduer_ds_report_ca.o \
./modules/device_status/lightduer_ds_log_cache.o \
./modules/device_status/lightduer_ds_log_recorder.o \
./modules/device_status/lightduer_ds_log_e2e.o \
./modules/device_status/lightduer_ds_log_audio_player.o \
./modules/System_Info/lightduer_system_info.o \
./modules/bind_device/lightduer_bind_device.o \
./platform/source-jl/baidu_ca_adapter.o \
./platform/source-jl/baidu_ca_debug_adp.o \
./platform/source-jl/baidu_ca_memory_adp.o \
./platform/source-jl/baidu_ca_mutex_adp.o \
./platform/source-jl/baidu_ca_socket_adp.o \
./platform/source-jl/lightduer_events.o \
./platform/source-jl/lightduer_priority_conf.o \
./platform/source-jl/lightduer_timers.o \
./platform/source-jl/lightduer_random_impl.o \
./platform/source-jl/lightduer_statistics.o \
./platform/source-jl/net_timer.o \
./modules/OTA/Downloader/lightduer_ota_downloader.o \
./modules/OTA/Downloader/lightduer_ota_http_downloader.o \
./modules/OTA/Downloader/lightduer_ota_local_downloader.o \
./modules/OTA/Decompression/lightduer_ota_decompression.o \
./modules/OTA/Decompression/lightduer_ota_zlib.o \
./modules/OTA/Installer/lightduer_ota_installer.o \
./modules/OTA/Notifier/lightduer_ota_notifier.o \
./modules/OTA/Unpacker/lightduer_ota_unpacker.o \
./modules/OTA/Updater/lightduer_ota_updater.o \
./modules/OTA/Verifier/lightduer_ota_verifier.o \
./examples/dcs3.0-demo/duerapp.o \
./examples/dcs3.0-demo/duerapp_event.o \
./examples/dcs3.0-demo/duerapp_profile_config.o \
./examples/dcs3.0-demo/duerapp_media.o \
./examples/dcs3.0-demo/duerapp_alert.o \
./examples/dcs3.0-demo/duer_api.o \
./examples/dcs3.0-demo/duerapp_ota.o \
# ./examples/dcs3.0-demo/duer_my_dcs.o \
./examples/dcs3.0-demo/duerapp_alarm.o \
./modules/flash_strings/lightduer_flash_strings.o \

obj_ls = \

obj_bs = \

objs_ver = version.o

GEN_LIB = y

include $(MAKE_RULE)

