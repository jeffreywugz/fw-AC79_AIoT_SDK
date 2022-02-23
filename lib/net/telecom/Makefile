
AR_OUT = $(AR_DIR)/libtelecom.a

CC_DEFINE +=\
  -D_SYS_SELECT_H \

ifeq ($(ARCH),pi32v2)
CC_ARGS += \
	-ffunction-sections \
	-Wno-unused-value \
	-Os \
	-Wno-pointer-sign
endif

includes = \
-I$(ROOT)/include_lib\
-I$(ROOT)/include_lib/utils\
-I$(ROOT)/include_lib/driver\
-I$(ROOT)/include_lib/driver/cpu/$(CPU)\
-I$(ROOT)/include_lib/driver/device\
-I$(ROOT)/include_lib/system/generic\
-I$(ROOT)/include_lib/system\
-I$(ROOT)/include_lib/net\
-I$(ROOT)/include_lib/net/$(MBEDTLS_DIR) \
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/app\
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/src/include\
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/src/include/lwip\
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/port\
-I$(ROOT)/include_lib/net/json_c\
-I./common\

objs = \
./common/aes_cbc.o\
./common/aes_core.o\
./common/base64.o\
./ctei_aes.o\
./ctei_device.o\
./ap_qlink.o\
./elink_qlink.o\
./mc_device.o\

obj_ls = \

obj_bs = \

objs_ver = version.o

GEN_LIB = y

include $(MAKE_RULE)

