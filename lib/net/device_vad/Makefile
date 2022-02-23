AR_OUT = $(AR_DIR)/libvad.a

CC_DEFINE += \
  -D_SYS_SELECT_H \

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
-I$(ROOT)/lib/net/device_vad/include \

objs = \
src/device_vad.o \
src/division_operations.o \
src/energy.o \
src/fvad.o \
src/get_scaling_square.o \
src/resample_48khz.o \
src/resample_by_2_internal.o \
src/resample_fractional.o \
src/spl_inl.o \
src/vad_core.o \
src/vad_filterbank.o \
src/vad_gmm.o \
src/vad_sp.o \

objs_ver = version.o

obj_ls = \

obj_bs = \

GEN_LIB=y

include $(MAKE_RULE)
