AR_OUT = $(AR_DIR)/libcoap.a

CC_DEFINE +=\
  -D_POSIX_SOURCE \
  -D_SYS_SELECT_H \
  -DCONFIG_CHRDEV \
  -DWITH_DTLS \

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
-I$(ROOT)/include_lib/net \
-I$(ROOT)/include_lib/net/libcoap \
-I$(ROOT)/include_lib/net/tinydtls \
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/src/include \
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/src/include/lwip \
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/port \
-I$(ROOT)/include_lib/system \

objs = \
src/subscribe.o \
src/async.o \
src/pdu.o \
src/hashkey.o \
src/uri.o \
src/resource.o \
src/option.o \
src/coap_list.o \
src/todo/coap_vfs.o \
src/block.o \
src/str.o \
src/net.o \
src/examples/server-dtls.o \
src/examples/client.o \
src/examples/client-dtls.o \
src/examples/server.o \
src/encode.o \
src/debug.o \

obj_ls = \

obj_bs = \

objs_ver = version.o

GEN_LIB = y

include $(MAKE_RULE)

