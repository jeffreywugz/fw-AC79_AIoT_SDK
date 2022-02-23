
AR_OUT = $(AR_DIR)/lib_sig_mesh.a

includes = \
-I$(ROOT)/include_lib/btstack/ \
-I$(ROOT)/include_lib/btctrler/ \
-I$(ROOT)/include_lib/btstack/third_party/common/ \
-I$(ROOT)/include_lib/utils \
-I$(ROOT)/include_lib/utils/btmesh \
-I$(ROOT)/include_lib/utils/btmesh/tinycrypt/include \
-I$(ROOT)/include_lib/utils/btmesh/adaptation \
-I$(ROOT)/include_lib/ \
-I$(ROOT)/include_lib/system \
-I$(ROOT)/include_lib/system/generic \
-I$(ROOT)/include_lib/driver/cpu/$(CPU) \

# SIG Mesh source code
MESH_C = \
	$(wildcard ./*.c) \
	$(wildcard ./net/*.c) \
	$(wildcard ./api/*.c) \
	$(wildcard ./adaptation/*.c) \
	$(wildcard ./adaptation/kernel/*.c) \
	$(wildcard ./adaptation/ble_core/*.c) \
	tinycrypt/source/cmac_mode.c \
	tinycrypt/source/aes_encrypt.c \
	tinycrypt/source/utils.c \

objs += $(patsubst %.c, %.o, $(MESH_C))

objs_ver = version.o

GEN_LIB = y

include $(MAKE_RULE)

