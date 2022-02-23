AR_OUT = $(AR_DIR)/libecho_cloud.a

CC_DEFINE +=\
  -D_SYS_SELECT_H \



ifeq ($(ARCH),pi32v2)
CC_ARGS += \
	-ffunction-sections \
	-Wno-unused-value \
	-Os \

endif


includes += \
-I$(ROOT)/include_lib \
-I$(ROOT)/include_lib/system \
-I$(ROOT)/include_lib/system/generic \
-I$(ROOT)/include_lib/net \
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/app \
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/src/include \
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/port \
-I$(ROOT)/include_lib/net/json_c\
-I$(ROOT)/include_lib/net/$(MBEDTLS_DIR) \
-I$(ROOT)/include_lib/net/$(MBEDTLS_DIR)/mbedtls\

objs = \
echo_cloud_nonce.o\
echo_cloud_mqtt.o\
echo_cloud_http.o\
echo_cloud_ota.o\
echo_cloud_api.o\
# echo_cloud_airkiss.o\

objs_ver = version.o

obj_ls = \


obj_bs = \

GEN_LIB = y

include $(MAKE_RULE)

