AR_OUT = $(AR_DIR)/lib_mqtt.a

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
-I$(ROOT)/include_lib/utils \
-I$(ROOT)/include_lib/system \
-I$(ROOT)/include_lib/system/generic \
-I$(ROOT)/include_lib/driver/cpu/$(CPU) \
-I$(ROOT)/include_lib/net \
-I$(ROOT)/include_lib/net/$(MBEDTLS_DIR) \
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/app \
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/src/include \
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/port \
-I$(ROOT)/include_lib/net/mqtt \

objs = \
./src/MQTTClient-C/src/linux/MQTTLinux.o	\
./src/MQTTClient-C/src/MQTTClient.o	\
./src/MQTTPacket/samples/null.o	\
./src/MQTTPacket/samples/pub0sub1.o	\
./src/MQTTPacket/samples/pub0sub1_nb.o	\
./src/MQTTPacket/samples/qos0pub.o	\
./src/MQTTPacket/samples/transport.o	\
./src/MQTTPacket/src/MQTTConnectClient.o	\
./src/MQTTPacket/src/MQTTConnectServer.o	\
./src/MQTTPacket/src/MQTTDeserializePublish.o	\
./src/MQTTPacket/src/MQTTFormat.o	\
./src/MQTTPacket/src/MQTTPacket.o	\
./src/MQTTPacket/src/MQTTSerializePublish.o	\
./src/MQTTPacket/src/MQTTSubscribeClient.o	\
./src/MQTTPacket/src/MQTTSubscribeServer.o	\
./src/MQTTPacket/src/MQTTUnsubscribeClient.o	\
./src/MQTTPacket/src/MQTTUnsubscribeServer.o	\
# ./src/MQTTPacket/test/test1.o	\
./src/MQTTClient-C/samples/linux/stdoutsub.o	\

obj_ls = \

obj_bs = \

objs_ver = version.o

GEN_LIB = y

include $(MAKE_RULE)

