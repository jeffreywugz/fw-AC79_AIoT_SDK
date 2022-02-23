AR_OUT = $(AR_DIR)/http_server.a

CC_DEFINE +=\
  -D_POSIX_SOURCE \
  -D_SYS_SELECT_H \
  -DCONFIG_CHRDEV \

ifeq ($(ARCH),pi32v2)
	CC_ARGS += \
	-ffunction-sections \
	-Wno-unused-value \

endif


includes += \
-I$(ROOT)/include_lib/system/cpu/$(CPU) \
-I$(ROOT)/include_lib/generic \
-I$(ROOT)/include_lib/os \
-I$(ROOT)/include_lib/net \
-I$(ROOT)/include_lib/net/$(MBEDTLS_DIR) \
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/app/sock_api \
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/src/include \
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/port \
-I$(ROOT)/include_lib/net/http \



objs = \
src/http_server.o\
src/http_vfs.o\
src/http_headers.o\
src/http_functions.o\




obj_ls = \


obj_bs = \

objs_ver = version.o

GEN_LIB = y

include $(MAKE_RULE)

