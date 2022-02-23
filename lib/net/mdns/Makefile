AR_OUT = $(AR_DIR)/libmdns.a

CC_DEFINE +=\
  -D_POSIX_SOURCE \
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
-I$(ROOT)/include_lib/net \
-I$(ROOT)/include_lib/net/$(MBEDTLS_DIR) \
-I$(ROOT)/include_lib/net/mdns \
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/src/include \
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/src/include/lwip \
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/port \

objs = \
./code/dnssd.o \
./code/dnssd_client.o \
./code/dnssd_clientlib.o \
./code/DNSCommon.o \
./code/DNSDigest.o \
./code/GenLinkedList.o \
./code/mDNSUNP.o \
./code/mDNS.o \
./code/uDNS.o \
./code/mDNSDebug.o \
./code/PlatformCommon.o \
./code/mDNSPosix.o \


obj_ls = \

obj_bs = \

objs_ver = version.o

GEN_LIB=y

include $(MAKE_RULE)
