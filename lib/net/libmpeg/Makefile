AR_OUT = $(AR_DIR)/libmpeg.a

CC_DEFINE += \

ifeq ($(ARCH),pi32v2)
CC_ARGS += \
-ffunction-sections \
-Wno-unused-value \
-Os \

endif

includes = \
-I$(ROOT)/include_lib/net/libmpeg \

objs = \
source/mpeg-crc32.o \
source/mpeg-element-descriptor.o \
source/mpeg-packet.o \
source/mpeg-pack-header.o \
source/mpeg-pat.o \
source/mpeg-pes.o \
source/mpeg-pmt.o \
source/mpeg-psd.o \
source/mpeg-ps-dec.o \
source/mpeg-ps-enc.o \
source/mpeg-psm.o \
source/mpeg-system-header.o \
source/mpeg-ts-dec.o \
source/mpeg-ts-enc.o \
source/mpeg-ts-h264.o \
source/mpeg-ts-h265.o \
source/mpeg-util.o \
source/mpeg-sdt.o \

obj_ls = \

obj_bs = \

objs_ver = version.o

GEN_LIB=y

include $(MAKE_RULE)
