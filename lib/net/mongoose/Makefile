AR_OUT = $(AR_DIR)/mongoose.a

CC_DEFINE +=\

ifeq ($(ARCH),pi32v2)
CC_ARGS += \
	-ffunction-sections \
	-Wno-unused-value \
	-Os
endif


includes = \
-I$(ROOT)/include_lib \
-I$(ROOT)/include_lib/utils \
-I$(ROOT)/include_lib/system \
-I$(ROOT)/include_lib/system/generic \
-I$(ROOT)/include_lib/driver/cpu/$(CPU) \
-I$(ROOT)/include_lib/net \
-I$(ROOT)/include_lib/net/$(MBEDTLS_DIR) \
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/app/sock_api \
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/src/include \
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/port \
-I$(ROOT)/include_lib/net/mongoose \

objs = \
./mongoose.o\
./Unit_test.o\
./mg_test.o\

obj_ls = \

obj_bs = \

objs_ver = version.o

GEN_LIB = y

include $(MAKE_RULE)

