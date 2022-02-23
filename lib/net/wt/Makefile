AR_OUT = $(AR_DIR)/libwt.a

CC_DEFINE +=\


CC_ARGS += \
	-ffunction-sections \
	-Wno-unused-value \
	-Os\


includes += \
-I$(ROOT)/include_lib/system \
-I$(ROOT)/include_lib/net \
-I$(ROOT)/include_lib/net/http \
-I$(ROOT)/lib/net/wt/inc \


objs = \
./src/process.o \
./src/wanxue_os.o \
./src/wt_camera.o \
./src/wt_http.o \
./src/wt_media_player.o \
./src/wt_server.o \

obj_ls = \


obj_bs = \

objs_ver = version.o

GEN_LIB=y

include $(MAKE_RULE)
