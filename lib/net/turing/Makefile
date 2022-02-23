AR_OUT = $(AR_DIR)/libturing.a

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
-I$(ROOT)/include_lib/net/$(MBEDTLS_DIR) \
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/app \
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/src/include \
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/port \
-I$(ROOT)/include_lib/net/json_c\
-I./include\


objs = \
turing_wechat_api.o\
turing_api.o\
turing.o\
turing_iot.o\
turing_airkiss.o\
turing_mqtt.o\
turing_mqtt_cmd.o \
turing_alarm.o\
tl_imgproc.o\


obj_ls = \


obj_bs = \

objs_ver = version.o

GEN_LIB = y

include $(MAKE_RULE)

