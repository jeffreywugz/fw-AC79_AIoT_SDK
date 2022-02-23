AR_OUT = $(AR_DIR)/libtencent.a

CC_DEFINE +=\
-D_SYS_SELECT_H \
-D_POSIX_SOURCE\
-D_STDIO_H_ \

ifeq ($(ARCH),pi32v2)
CC_ARGS += \
	-ffunction-sections \
	-Wno-unused-value \
	-Os \

endif

includes = \
-I$(ROOT)/lib/net/tencent/tvs_sdk/inc\
-I$(ROOT)/lib/net/tencent/tvs_sdk/tvs_sdk_api\
-I$(ROOT)/lib/net/tencent/tvs_common\
-I$(ROOT)/lib/net/tencent/tvs_adapter\
-I$(ROOT)/lib/net/tencent/third_party/inc/mongoose\
-I$(ROOT)/lib/net/tencent/third_party/inc/net_ping\
-I$(ROOT)/lib/net/tencent/os/inc\
-I$(ROOT)/lib/net/tencent/compatible/inc/tvs_api\
-I$(ROOT)/lib/net/tencent/compatible/inc\
-I$(ROOT)/lib/net/tencent/qcloud_iot_c_sdk/include\
-I$(ROOT)/lib/net/tencent/qcloud_iot_c_sdk/include/exports\
-I$(ROOT)/lib/net/tencent/qcloud_iot_c_sdk/sdk_src/internal_inc\
-I$(ROOT)/include_lib/utils \
-I$(ROOT)/include_lib/driver\
-I$(ROOT)/include_lib/driver/cpu/$(CPU)\
-I$(ROOT)/include_lib/driver/device\
-I$(ROOT)/include_lib/system/generic\
-I$(ROOT)/include_lib/system\
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/app\
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/src/include\
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/src/include/lwip\
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/port\
-I$(ROOT)/include_lib/net\
-I$(ROOT)/include_lib/net/$(MBEDTLS_DIR) \
-I$(ROOT)/include_lib/net/$(MBEDTLS_DIR)/mbedtls\
-I$(ROOT)/include_lib/net/$(MBEDTLS_DIR)/mbedtls/configs\
-I$(ROOT)/include_lib\

SRCS_C = \
	$(wildcard tvs_sdk/src/*.c) \
	$(wildcard tvs_adapter/*.c) \
	$(wildcard third_party/src/mongoose/*.c) \
	$(wildcard third_party/src/net_ping/*.c) \
	$(wildcard os/freeRTOS/*.c) \
	$(wildcard compatible/src/*.c) \
	$(wildcard qcloud_iot_c_sdk/platform/*.c) \
	$(wildcard qcloud_iot_c_sdk/sdk_src/*.c) \
	$(wildcard qcloud_iot_c_sdk/wifi_config/*.c) \
	$(wildcard qcloud_iot_c_sdk/samples/tvs/*.c) \

objs = $(patsubst %.c, %.o, $(SRCS_C))

obj_ls = \

obj_bs = \

objs_ver = version.o

GEN_LIB = y

include $(MAKE_RULE)

