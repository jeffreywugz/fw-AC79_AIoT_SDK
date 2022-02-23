AR_OUT = $(AR_DIR)/uc_httpc.a

CC_DEFINE +=\
  -D_POSIX_SOURCE \
  -D_SYS_SELECT_H \
  -DCONFIG_CHRDEV \

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
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/app/ \
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/app/sock_api \
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/src/include \
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/port \
-I$(ROOT)/include_lib/net/http \
-I$(ROOT)/include_lib/net/uC-HTTPc \
-I$(ROOT)/include_lib/net/uC-HTTPc/include \

objs = \
add/lib_str.o\
add/lib_ascii.o\
add/lib_mem.o\
Client/Source/http-c_conn.o\
Client/Source/http-c_mem.o\
Client/Source/http-c_req.o\
Client/Source/http-c_resp.o\
Client/Source/http-c_sock.o\
Client/Source/http-c_task.o\
Client/Source/http-c_websock.o\
Client/Examples/http-c_hooks.o\
Client/Examples/static_files.o\
Client/Cfg/http-c_cfg.o\
Common/http.o\
Common/http_dict.o\
Client/net_sock.o\
Client/http_cli.o\
Client/Source/http-c.o\
Client/Examples/http-c_app.o\

obj_ls = \

obj_bs = \

objs_ver = version.o

GEN_LIB = y

include $(MAKE_RULE)

