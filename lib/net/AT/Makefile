
AR_OUT = $(AR_DIR)/at.a

CC_DEFINE += \
-DCONFIG_FREE_RTOS_ENABLE \
-DCONFIG_THREAD_ENABLE \

includes = \
-I$(ROOT)/include_lib \
-I$(ROOT)/include_lib/system \
-I$(ROOT)/include_lib/system/os/FreeRTOS \
-I$(ROOT)/include_lib/system/os/pi32v2 \
-I$(ROOT)/include_lib/driver \
-I$(ROOT)/include_lib/driver/device \
-I$(ROOT)/include_lib/system/generic \
-I$(ROOT)/include_lib/driver/cpu/$(CPU) \
-I$(ROOT)/lib \
-I$(ROOT)/lib/net \
-I$(ROOT)/lib/net/AT \
-I$(ROOT)/include_lib/utils \
-I$(ROOT)/include_lib/utils/event \
-I$(ROOT)/include_lib/net \
-I$(ROOT)/include_lib/net/wifi \
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/port \
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/app \
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/app/sock_api \
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/src/include \
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/src/include/lwip \

objs = \
at_base_cmd.o \
at_cli.o \
at_client.o \
at_server.o \
at_socket.o \
at_utils.o \

GEN_LIB=y

include $(MAKE_RULE)

