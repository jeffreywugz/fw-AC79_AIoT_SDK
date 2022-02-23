AR_OUT = $(AR_DIR)/wolfmqtt.a

CC_DEFINE +=\
  -D_SYS_SELECT_H \

ifeq ($(ARCH),pi32v2)
CC_ARGS += \
	-ffunction-sections \
	-Wno-unused-value \
	-Os \

endif

includes = \
-I$(ROOT)/include_lib \
-I$(ROOT)/include_lib/system \
-I$(ROOT)/include_lib/utils \
-I$(ROOT)/include_lib/system/generic \
-I$(ROOT)/include_lib/driver \
-I$(ROOT)/include_lib/driver/cpu/$(CPU) \
-I$(ROOT)/include_lib/system/os \
-I$(ROOT)/include_lib/net \
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/app \
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/src/include \
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/port \
-I$(ROOT)/include_lib/net/wolfmqtt \

objs += \
./src/mqtt_client.o	\
./src/mqtt_packet.o	\
./src/mqtt_socket.o	\
./src/mqtt_net.o	\

obj_ls = \

obj_bs = \

objs_ver = version.o

GEN_LIB = y

include $(MAKE_RULE)

