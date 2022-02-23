
# make 编译并下载
# make VERBOSE=1 显示编译详细过程
# make clean 清除编译临时文件
# 注意： Linux 下编译方式：
# 1. 从 http://pkgman.jieliapp.com/doc/all 处找到下载链接
# 2. 下载后，解压到 /opt/jieli 目录下，保证
#   /opt/jieli/common/bin/clang 存在（注意目录层次）
# 3. 确认 ulimit -n 的结果足够大（建议大于8096），否则链接可能会因为打开文件太多而失败
#   可以通过 ulimit -n 8096 来设置一个较大的值

# 工具路径设置
ifeq ($(OS), Windows_NT)
# Windows 下工具链位置
TOOL_DIR := C:/JL/pi32/bin
CC    := clang.exe
CXX   := clang.exe
LD    := pi32v2-lto-wrapper.exe
AR    := pi32v2-lto-ar.exe
MKDIR := mkdir_win -p
RM    := rm -rf

SYS_LIB_DIR := C:/JL/pi32/pi32v2-lib/r3
SYS_INC_DIR := C:/JL/pi32/pi32v2-include

## 后处理脚本
POST_SCRIPT     := ..\..\..\..\..\cpu\wl82\tools\download.bat
RUN_POST_SCRIPT := $(POST_SCRIPT)
else
# Linux 下工具链位置
TOOL_DIR := /opt/jieli/pi32v2/bin
CC    := clang
CXX   := clang++
LD    := lto-wrapper
AR    := lto-ar
MKDIR := mkdir -p
RM    := rm -rf

SYS_LIB_DIR := $(TOOL_DIR)/../lib/r3
SYS_INC_DIR := $(TOOL_DIR)/../include

## 后处理脚本
POST_SCRIPT     := ../../../../../cpu/wl82/tools/download.sh
RUN_POST_SCRIPT := bash $(POST_SCRIPT)
endif

CC  := $(TOOL_DIR)/$(CC)
CXX := $(TOOL_DIR)/$(CXX)
LD  := $(TOOL_DIR)/$(LD)
AR  := $(TOOL_DIR)/$(AR)
# 输出文件设置
OUT_ELF   := ../../../../../cpu/wl82/tools/sdk.elf
OBJ_FILE  := $(OUT_ELF).objs.txt
# 编译路径设置
BUILD_DIR := objs
# 工程路径前缀
ROOT_PREFIX := ../../../../..

# 编译参数设置
CFLAGS := \
	-target pi32v2 \
	-mcpu=r3 \
	-integrated-as \
	-flto \
	-Wuninitialized \
	-Wno-invalid-noreturn \
	-fno-common \
	-integrated-as \
	-Oz \
	-g \
	-flto \
	-fallow-pointer-null \
	-fprefer-gnu-section \
	-Wno-shift-negative-value \
	-Wframe-larger-than=2560 \
	-fms-extensions \
	-w \


# 宏定义
DEFINES := \
	-DSUPPORT_MS_EXTENSIONS \
	-DCONFIG_RELEASE_ENABLE \
	-DCONFIG_CPU_WL82 \
	-DCONFIG_JLFAT_ENABLE \
	-DCONFIG_FREE_RTOS_ENABLE \
	-DCONFIG_THREAD_ENABLE \
	-DCONFIG_AUDIO_ONCHIP \
	-D__GCC_PI32V2__ \
	-DCONFIG_NEW_ECC_ENABLE \
	-DCONFIG_EQ_SUPPORT_ASYNC \
	-DEQ_CORE_V1 \
	-DCONFIG_SFC_ENABLE \
	-DCONFIG_AUDIO_ENABLE \
	-DCONFIG_NO_SDRAM_ENABLE \
	-DCONFIG_VIDEO_ENABLE=1 \
	-DCONFIG_VIDEO0_ENABLE=1 \
	-DEVENT_HANDLER_NUM_CONFIG=2 \
	-DEVENT_TOUCH_ENABLE_CONFIG=0 \
	-DEVENT_POOL_SIZE_CONFIG=256 \
	-DCONFIG_EVENT_KEY_MAP_ENABLE=0 \
	-DTIMER_POOL_NUM_CONFIG=15 \
	-DAPP_ASYNC_POOL_NUM_CONFIG=0 \
	-DVFS_ENABLE=1 \
	-DUSE_SDFILE_NEW=1 \
	-DSDFILE_STORAGE=1 \
	-DVFS_FILE_POOL_NUM_CONFIG=8 \
	-DVM_MAX_SIZE_CONFIG=32*1024 \
	-DVM_ITEM_MAX_NUM=256 \
	-DCONFIG_UPDATA_ENABLE \
	-DCONFIG_OTA_UPDATA_ENABLE \
	-DCONFIG_ITEM_FORMAT_VM \


# 头文件搜索路径
INCLUDES := \
	-I../../../../../include_lib \
	-I../../../../../include_lib/driver \
	-I../../../../../include_lib/driver/device \
	-I../../../../../include_lib/driver/cpu/wl82 \
	-I../../../../../include_lib/system \
	-I../../../../../include_lib/system/generic \
	-I../../../../../include_lib/btctrler \
	-I../../../../../include_lib/btctrler/port/wl82 \
	-I../../../../../include_lib/update \
	-I../../../../../include_lib/btstack/third_party/common \
	-I../../../../../include_lib/btstack/third_party/rcsp \
	-I../../../../../include_lib/utils \
	-I../../../../../include_lib/utils/syscfg \
	-I../../../../../include_lib/utils/event \
	-I../../../../../include_lib/media/media_develop \
	-I../../../../../include_lib/media/media_develop/media \
	-I../../../../../include_lib/media/media_develop/media/cpu/wl82 \
	-I../../../../../include_lib/media/media_develop/media/cpu/wl82/asm \
	-I../../../../../include_lib/driver/device/video \
	-I../../../../../apps \
	-I../../../../../apps/common/include \
	-I../../../../../apps/common/config/include \
	-I../../../../../apps/demo/demo_video \
	-I../../../../../apps/demo/demo_video/include \
	-I../../../../../include_lib/net \
	-I../../../../../include_lib/net/mbedtls_2_2_1 \
	-I../../../../../include_lib/net/server \
	-I../../../../../include_lib/server \
	-I../../../../../include_lib/net/lwip_2_1_3/lwip/src/include \
	-I../../../../../include_lib/net/lwip_2_1_3/lwip/src/include/lwip \
	-I../../../../../include_lib/net/lwip_2_1_3/lwip/port \
	-I../../../../../include_lib/net/lwip_2_1_3/lwip/app \
	-I$(SYS_INC_DIR) \


# 需要编译的 .c 文件
c_SRC_FILES := \
	../../../../../apps/common/audio_music/audio_config.c \
	../../../../../apps/common/camera/byd20a6/byd20a6.c \
	../../../../../apps/common/camera/byd30a2/byd30a2.c \
	../../../../../apps/common/camera/byd3720/byd3720.c \
	../../../../../apps/common/camera/byd3901/byd3901.c \
	../../../../../apps/common/camera/byd3a03/byd3a03.c \
	../../../../../apps/common/camera/gc0307_dvp/gc0307.c \
	../../../../../apps/common/camera/gc0308_dvp/gc0308.c \
	../../../../../apps/common/camera/gc0309_dvp/gc0309.c \
	../../../../../apps/common/camera/gc0310_spi/gc0310.c \
	../../../../../apps/common/camera/gc0312_dvp/gc0312.c \
	../../../../../apps/common/camera/gc0328_dvp/gc0328.c \
	../../../../../apps/common/camera/gc0329_dvp/gc0329.c \
	../../../../../apps/common/camera/gc2145_dvp/gc2145.c \
	../../../../../apps/common/camera/h7375s/h7375s.c \
	../../../../../apps/common/camera/ov7670_dvp/ov7670.c \
	../../../../../apps/common/camera/pr2000k/pr2000k.c \
	../../../../../apps/common/camera/xc7011_dvp/xc7011.c \
	../../../../../apps/common/config/log_config/lib_driver_config.c \
	../../../../../apps/common/config/log_config/lib_media_config.c \
	../../../../../apps/common/config/log_config/lib_system_config.c \
	../../../../../apps/common/config/log_config/lib_update_config.c \
	../../../../../apps/common/config/user_cfg.c \
	../../../../../apps/common/debug/debug.c \
	../../../../../apps/common/debug/debug_user.c \
	../../../../../apps/common/iic/iic.c \
	../../../../../apps/common/iic/software_iic.c \
	../../../../../apps/common/system/init.c \
	../../../../../apps/common/system/system_reset_reason.c \
	../../../../../apps/common/system/version.c \
	../../../../../apps/common/update/update.c \
	../../../../../apps/demo/demo_video/app_main.c \
	../../../../../apps/demo/demo_video/board/wl82/board.c \
	../../../../../apps/demo/demo_video/stream_protocol.c \
	../../../../../apps/demo/demo_video/user_video_rec.c \
	../../../../../apps/demo/demo_video/video_rt_usr.c \
	../../../../../cpu/wl82/debug.c \
	../../../../../cpu/wl82/iic.c \
	../../../../../cpu/wl82/key/adc_api.c \
	../../../../../cpu/wl82/port_waked_up.c \
	../../../../../cpu/wl82/setup.c \


# 需要编译的 .S 文件
S_SRC_FILES :=


# 需要编译的 .s 文件
s_SRC_FILES :=


# 需要编译的 .cpp 文件
cpp_SRC_FILES :=


# 链接参数
LFLAGS := \
	--plugin-opt=-pi32v2-always-use-itblock=false \
	--plugin-opt=-enable-ipra=true \
	--plugin-opt=-pi32v2-merge-max-offset=4096 \
	--plugin-opt=-pi32v2-enable-simd=true \
	--plugin-opt=mcpu=r3 \
	--plugin-opt=-global-merge-on-const \
	--plugin-opt=-inline-threshold=5 \
	--plugin-opt=-inline-max-allocated-size=32 \
	--plugin-opt=-inline-normal-into-special-section=true \
	--plugin-opt=-dont-used-symbol-list=malloc,free,sprintf,printf,puts,putchar \
	--plugin-opt=save-temps \
	--plugin-opt=-pi32v2-enable-rep-memop \
	--plugin-opt=-warn-stack-size=2560 \
	--sort-common \
	--dont-complain-call-overflow \
	--plugin-opt=-used-symbol-file=../../../../../cpu/wl82/sdk_used_list.used \
	--plugin-opt=-pi32v2-large-program=true \
	--start-group \
	../../../../../cpu/wl82/liba/cpu.a \
	../../../../../cpu/wl82/liba/event.a \
	../../../../../cpu/wl82/liba/system.a \
	../../../../../cpu/wl82/liba/fs.a \
	../../../../../cpu/wl82/liba/cfg_tool.a \
	../../../../../cpu/wl82/liba/video_rec_server.a \
	../../../../../cpu/wl82/liba/audio_server.a \
	../../../../../cpu/wl82/liba/common_lib.a \
	../../../../../cpu/wl82/liba/net_server.a \
	../../../../../cpu/wl82/liba/update.a \
	../../../../../cpu/wl82/liba/media_app.a \
	--end-group \
	-T../../../../../cpu/wl82/sdk.ld \
	-M=../../../../../cpu/wl82/tools/sdk.map \
	--plugin-opt=mcpu=r3 \
	--plugin-opt=-mattr=+fprev1 \


LIBPATHS := \
	-L$(SYS_LIB_DIR) \


LIBS := \
	$(SYS_LIB_DIR)/libm.a \
	$(SYS_LIB_DIR)/libc.a \
	$(SYS_LIB_DIR)/libm.a \
	$(SYS_LIB_DIR)/libcompiler-rt.a \



c_OBJS    := $(c_SRC_FILES:%.c=%.c.o)
S_OBJS    := $(S_SRC_FILES:%.S=%.S.o)
s_OBJS    := $(s_SRC_FILES:%.s=%.s.o)
cpp_OBJS  := $(cpp_SRC_FILES:%.cpp=%.cpp.o)

OBJS      := $(c_OBJS) $(S_OBJS) $(s_OBJS) $(cpp_OBJS)
DEP_FILES := $(OBJS:%.o=%.d)


OBJS      := $(addprefix $(BUILD_DIR)/, $(OBJS:$(ROOT_PREFIX)/%=%))
DEP_FILES := $(addprefix $(BUILD_DIR)/, $(DEP_FILES:$(ROOT_PREFIX)/%=%))


VERBOSE ?= 0
ifeq ($(VERBOSE), 1)
QUITE :=
else
QUITE := @
endif

LINK_AT ?= 1 # 一些旧的 make 不支持 file 函数，需要 make 的时候指定 LINK_AT=0 make

.PHONY: all clean pre_build

all: pre_build $(OUT_ELF)
	@echo +POST-BUILD
	$(QUITE) $(RUN_POST_SCRIPT) sdk

pre_build:
	@echo +PRE-BUILD
	$(QUITE) $(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -D__LD__ -E -P ../../../../../cpu/wl82/sdk_used_list.c -o ../../../../../cpu/wl82/sdk_used_list.used
	$(QUITE) $(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -D__LD__ -E -P ../../../../../cpu/wl82/sdk_ld.c -o ../../../../../cpu/wl82/sdk.ld

clean:
	$(QUITE) $(RM) $(OUT_ELF)
	$(QUITE) $(RM) $(OBJS)
	$(QUITE) $(RM) $(DEP_FILES)

$(OUT_ELF): $(OBJS)
	@echo +LINK $@
ifeq ($(LINK_AT), 1)
	$(file >$(OBJ_FILE), $(OBJS))
	$(QUITE) $(LD) -o $(OUT_ELF) @$(OBJ_FILE) $(LFLAGS) $(LIBPATHS) $(LIBS)
else
	$(QUITE) $(LD) -o $(OUT_ELF) $(OBJS) $(LFLAGS) $(LIBPATHS) $(LIBS)
endif

$(BUILD_DIR)/%.c.o : $(ROOT_PREFIX)/%.c
	@echo +CC $<
	@$(MKDIR) $(@D)
	@$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -MM -MT "$@" $< -o $(@:.o=.d)
	$(QUITE) $(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/%.S.o : $(ROOT_PREFIX)/%.S
	@echo +AS $<
	@$(MKDIR) $(@D)
	@$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -MM -MT "$@" $< -o $(@:.o=.d)
	$(QUITE) $(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/%.s.o : $(ROOT_PREFIX)/%.s
	@echo +AS $<
	@$(MKDIR) $(@D)
	@$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -MM -MT "$@" $< -o $(@:.o=.d)
	$(QUITE) $(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/%.cpp.o : $(ROOT_PREFIX)/%.cpp
	@echo +CXX $<
	@$(MKDIR) $(@D)
	@$(CXX) $(CFLAGS) $(DEFINES) $(INCLUDES) -MM -MT "$@" $< -o $(@:.o=.d)
	$(QUITE) $(CXX) $(CFLAGS) $(DEFINES) $(INCLUDES) -c $< -o $@

-include $(DEP_FILES)
