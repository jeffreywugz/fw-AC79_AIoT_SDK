AR_OUT = $(AR_DIR)/iperf.a

CC_DEFINE +=\
  -D_POSIX_SOURCE \
  -D_SYS_SELECT_H \

ifeq ($(ARCH),pi32v2)
CC_ARGS += \
-ffunction-sections \
-Wno-unused-value \
-Os\

endif

includes = \
-I$(ROOT)/include_lib \
-I$(ROOT)/include_lib/system \
-I$(ROOT)/include_lib/system/generic \
-I$(ROOT)/include_lib/driver/cpu/$(CPU) \
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/src/include \
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/src/include/lwip \
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/port \
-I$(ROOT)/lib/net/iperf/src \

objs = \
./src/cjson.o\
./src/dscp.o\
./src/iperf_api.o\
./src/iperf_client_api.o\
./src/iperf_error.o\
./src/iperf_locale.o\
./src/iperf_sctp.o\
./src/iperf_server_api.o\
./src/iperf_tcp.o\
./src/iperf_udp.o\
./src/iperf_util.o\
./src/main.o\
./src/net.o\
./src/tcp_info.o\
./src/tcp_window_size.o\
./src/timer.o\
./src/t_timer.o\
./src/t_units.o\
./src/t_uuid.o\
./src/units.o\
./example/mic.o\
./example/mis.o\

obj_ls = \

obj_bs = \

objs_ver = version.o

GEN_LIB = y

include $(MAKE_RULE)
