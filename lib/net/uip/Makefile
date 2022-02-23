AR_OUT = $(AR_DIR)/uip.a


CC_DEFINE += \

ifeq ($(ARCH),pi32v2)
CC_ARGS += \
	-ffunction-sections \
	-Wno-unused-value \
	-Os\

endif

includes = \
-I$(ROOT)/include_lib \
-I$(ROOT)/include_lib/utils \
-I$(ROOT)/include_lib/system \
-I$(ROOT)/include_lib/system/generic \
-I$(ROOT)/include_lib/driver \
-I$(ROOT)/include_lib/driver/cpu/$(CPU) \
-I$(ROOT)/include_lib/system/os \
-I$(ROOT)/include_lib/net/uip \
-I$(ROOT)/include_lib/net/uip/apps/dhcp \
-I$(ROOT)/include_lib/net/uip/port \
-I$(ROOT)/include_lib/net/uip/src \
-I$(ROOT)/include_lib/net/uip/test \

objs = \
./apps/dhcp/dhcpc.o\
./apps/dhcp/dhcps.o\
./apps/dhcp/dhcp_appcall.o\
./port/wifi-tapdev.o\
./src/clock-arch.o\
./src/psock.o\
./src/timer.o\
./src/uip-fw.o\
./src/uip-neighbor.o\
./src/uip-split.o\
./src/uip.o\
./src/uip_arp.o\
./src/uiplib.o\
./test/tcp_demo.o\
./test/tcp_client_demo.o\
./test/tcp_server_demo.o\
./test/udp_demo.o\
./test/udp_clien_demo.o\
./test/udp_server_demo.o\
./test/uip_test.o\

obj_ls = \


obj_bs = \

objs_ver = version.o

GEN_LIB=y


include $(MAKE_RULE)

