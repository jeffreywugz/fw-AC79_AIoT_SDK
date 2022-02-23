AR_OUT = $(AR_DIR)/zliblite.a

CC_DEFINE +=\
  -D_POSIX_SOURCE \
  -DUSE_FILE32API \
  -DMAX_WBITS=15	\

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
-I$(ROOT)/include_lib/driver/cpu/$(CPU) \
-I$(ROOT)/include_lib/net \
-I$(ROOT)/include_lib/net/zliblite \

objs = \
./src/adler32.o \
./src/compress.o \
./src/crc32.o \
./src/deflate.o \
./src/infback.o \
./src/inffast.o \
./src/inflate.o \
./src/inftrees.o \
./src/trees.o \
./src/uncompr.o \
./src/ioapi.o \
./src/zutil.o \
./src/unzip.o \
./src/miniunz.o \

obj_ls = \

obj_bs = \

objs_ver = version.o

GEN_LIB=y

include $(MAKE_RULE)
