
AR_OUT = $(AR_DIR)/libnopoll.a

CC_DEFINE +=\
  -D_SYS_SELECT_H \
  -D_FCNTL_H \
  -D_SYS_UNISTD_H \

ifeq ($(ARCH),pi32v2)
CC_ARGS += \
-ffunction-sections \
-Wno-unused-value \
-Os \

endif

includes = \
-I$(ROOT)/include_lib \
-I$(ROOT)/include_lib/system \
-I$(ROOT)/include_lib/system/generic \
-I$(ROOT)/include_lib/driver/cpu/$(CPU) \
-I$(ROOT)/include_lib/net \
-I$(ROOT)/include_lib/net/$(MBEDTLS_DIR) \
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/app \
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/src/include \
-I$(ROOT)/include_lib/net//$(LWIP_DIR)/lwip/port \
-I$(ROOT)/include_lib/net/nopoll \

objs = \
./src/nopoll.o \
./src/nopoll_conn.o \
./src/nopoll_conn_opts.o \
./src/nopoll_ctx.o \
./src/nopoll_decl.o \
./src/nopoll_io.o \
./src/nopoll_listener.o \
./src/nopoll_log.o \
./src/nopoll_loop.o \
./src/nopoll_mbedtls.o \
./src/nopoll_msg.o \
./src/nopoll_socket_api.o \
./src/nopoll_win32.o \
./example/nopoll_client.o \
./example/nopoll_server.o \

obj_ls = \

obj_bs = \

objs_ver = version.o

GEN_LIB = y

include $(MAKE_RULE)
