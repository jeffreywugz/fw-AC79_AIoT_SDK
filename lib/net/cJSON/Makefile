AR_OUT = $(AR_DIR)/cJSON.a

CC_DEFINE +=\

ifeq ($(ARCH),pi32v2)
CC_ARGS += \
-ffunction-sections \
-Os	\

endif

includes = \
-I$(ROOT)/include_lib/system/generic \
-I$(ROOT)/include_lib/net/cJSON_common \

objs = \
src/cJSON_Utils.o\
src/cJSON.o\

objs_ver = version.o

obj_ls = \

obj_bs = \

GEN_LIB = y

include $(MAKE_RULE)

