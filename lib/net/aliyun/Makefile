
AR_OUT = $(AR_DIR)/libaliyun.a

CC_DEFINE +=\
  -D_SYS_SELECT_H \
  -DLIST_H \

ifeq ($(ARCH),pi32v2)
CC_ARGS += \
	-ffunction-sections \
	-Wno-unused-value \
	-Os \

endif


includes = \
-I$(ROOT)/include_lib \
-I$(ROOT)/include_lib/system \
-I$(ROOT)/include_lib/system/generic \
-I$(ROOT)/include_lib/driver/cpu/$(CPU) \
-I$(ROOT)/include_lib/net \
-I$(ROOT)/include_lib/net/$(MBEDTLS_DIR) \
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/app \
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/src/include \
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/port \
-I$(ROOT)/include_lib/net/mqtt \
-I$(ROOT)/include_lib/net/cJSON_common\
-I$(ROOT)/include_lib/net/aliyun \


objs = \
./certs/root_ca.o \
./dev_model/dm_api.o \
./dev_model/dm_client.o \
./dev_model/dm_client_adapter.o \
./dev_model/dm_cota.o \
./dev_model/dm_fota.o \
./dev_model/dm_ipc.o \
./dev_model/dm_log_report.o \
./dev_model/dm_manager.o \
./dev_model/dm_message.o \
./dev_model/dm_message_cache.o \
./dev_model/dm_msg_process.o \
./dev_model/dm_opt.o \
./dev_model/dm_ota.o \
./dev_model/dm_server.o \
./dev_model/dm_server_adapter.o \
./dev_model/dm_shadow.o \
./dev_model/dm_tsl_alink.o \
./dev_model/dm_utils.o \
./dev_model/impl_gateway.o \
./dev_model/impl_linkkit.o \
./dev_model/impl_ntp.o \
./dev_model/impl_solo.o \
./dev_model/iotx_cm.o \
./dev_model/iotx_cm_coap.o \
./dev_model/iotx_cm_mqtt.o \
./dev_sign/dev_sign_mqtt.o \
./infra/infra_cjson.o \
./infra/infra_compat.o \
./infra/infra_defs.o \
./infra/infra_log.o \
./infra/infra_net.o \
./infra/infra_prt_nwk_payload.o \
./infra/infra_report.o \
./infra/infra_sha256.o \
./infra/infra_string.o \
./infra/infra_timer.o \
./mqtt/iotx_mqtt_client.o \
./mqtt/mqtt_api.o \
./wrappers/HAL_TLS_mbedtls.o \
./wrappers/wrapper.o \
./examples/mqtt_example.o \

objs_ver = version.o

obj_ls = \

obj_bs = \

GEN_LIB = y

include $(MAKE_RULE)

