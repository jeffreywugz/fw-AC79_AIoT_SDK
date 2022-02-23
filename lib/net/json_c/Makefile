AR_OUT = $(AR_DIR)/json.a

CC_DEFINE +=\
  -D_POSIX_SOURCE \
  -DCONFIG_CHRDEV \
  -DMC_MAINTAINER_MODE\

ifeq ($(ARCH),pi32v2)
CC_ARGS += \
-ffunction-sections \
-Os	\

endif

includes = \
-I$(ROOT)/include_lib/system/generic \
-I$(ROOT)/include_lib/net \
-I$(ROOT)/include_lib/net/json_c \

objs = \
src/random_seed.o\
src/json_tokener.o\
src/libjson.o\
src/json_c_version.o\
src/json_util.o\
src/json_object.o\
src/arraylist.o\
src/printbuf.o\
src/linkhash.o\
src/debug.o\
src/json_object_iterator.o\
src/json_pointer.o\
src/json_visit.o\
src/strerror_override.o\

obj_ls = \

obj_bs = \

objs_ver = version.o

GEN_LIB = y

include $(MAKE_RULE)

