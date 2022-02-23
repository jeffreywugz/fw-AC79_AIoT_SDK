AR_OUT = $(AR_DIR)/tmallgenie_todo.a


CC_DEFINE +=\
  -D_SYS_SELECT_H \
	-D_STDIO_H \
	-D__STDBOOL_H\
	-D_POSIX_SOURCE\

ifeq ($(ARCH),pi32v2)
CC_ARGS += \
	-ffunction-sections \
	-Wno-unused-value \
	-Os \

endif


includes = \
-I$(ROOT)/include_lib/driver/cpu/$(CPU)/ \
-I$(ROOT)/include_lib/system/generic\
-I$(ROOT)/include_lib\
-I$(ROOT)/include_lib/utils\
-I$(ROOT)/include_lib/driver\
-I$(ROOT)/include_lib/driver/device\
-I$(ROOT)/include_lib/system\
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/app\
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/src/include\
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/port \
-I$(ROOT)/include_lib/net\
-I$(ROOT)/include_lib/system/device\
-I$(ROOT)/include_lib/net/$(MBEDTLS_DIR) \
-I$(ROOT)/include_lib/net/$(MBEDTLS_DIR)/mbedtls\
-I$(ROOT)/include_lib/net/$(MBEDTLS_DIR)/mbedtls/configs\
-I./include\


objs = \
src/aligenie_audio.o\
src/aligenie_notify.o\
src/aligenie_os.o\
src/aligenie_ws.o\
src/auth.o\
src/sdk.o\


obj_ls = \


obj_bs = \

objs_ver = version.o

GEN_LIB = y

include $(MAKE_RULE)

