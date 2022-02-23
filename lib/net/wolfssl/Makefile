AR_OUT = $(AR_DIR)/wolfssl.a

CC_DEFINE +=\
  -D_POSIX_SOURCE \
  -D_SYS_SELECT_H \
  -DCONFIG_CHRDEV \
  -D__unix__ \

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
-I$(ROOT)/include_lib/net/wolfssl \
-I$(ROOT)/include_lib/net/wolfssl/mbedtls \
-I$(ROOT)/include_lib/net/wolfssl/openssl \
-I$(ROOT)/include_lib/net/wolfssl/wolfcrypt \
-I$(ROOT)/include_lib/net/wolfssl/cyassl \
-I$(ROOT)/include_lib/net/wolfssl/cyassl/openssl \
-I$(ROOT)/include_lib/net/wolfssl/cyassl/ctaocrypt \
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/app/sock_api \
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/src/include \
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/port \
-I$(ROOT)/include_lib/system \
-I$(ROOT)/include_lib/system/device \
-I$(ROOT)/lib/net/device/wireless \
-I$(ROOT)/lib/net/device/wireless/wpa_supplicant-2.9 \
-I$(ROOT)/lib/net/device/wireless/wpa_supplicant-2.9/src \
-I$(ROOT)/lib/net/device/wireless/wpa_supplicant-2.9/src/utils \
-I./ \
-I./wolfcrypt/src \

objs = \
./src/internal.o \
./src/ocsp.o \
./src/ssl.o \
./src/tls13.o \
./src/crl.o \
./src/keys.o \
./src/sniffer.o \
./src/tls.o \
./src/wolfio.o \
./wolfcrypt/src/aes.o \
./wolfcrypt/src/arc4.o \
./wolfcrypt/src/asm.o \
./wolfcrypt/src/asn.o \
./wolfcrypt/src/blake2b.o \
./wolfcrypt/src/blake2s.o \
./wolfcrypt/src/camellia.o \
./wolfcrypt/src/chacha.o \
./wolfcrypt/src/chacha20_poly1305.o \
./wolfcrypt/src/cmac.o \
./wolfcrypt/src/coding.o \
./wolfcrypt/src/compress.o \
./wolfcrypt/src/cpuid.o \
./wolfcrypt/src/cryptocb.o \
./wolfcrypt/src/curve25519.o \
./wolfcrypt/src/curve448.o \
./wolfcrypt/src/des3.o \
./wolfcrypt/src/dh.o \
./wolfcrypt/src/dsa.o \
./wolfcrypt/src/ecc_fp.o \
./wolfcrypt/src/ed25519.o \
./wolfcrypt/src/ed448.o \
./wolfcrypt/src/error.o \
./wolfcrypt/src/fe_448.o \
./wolfcrypt/src/fe_low_mem.o \
./wolfcrypt/src/fe_operations.o \
./wolfcrypt/src/ge_448.o \
./wolfcrypt/src/ge_low_mem.o \
./wolfcrypt/src/ge_operations.o \
./wolfcrypt/src/hash.o \
./wolfcrypt/src/hc128.o \
./wolfcrypt/src/hmac.o \
./wolfcrypt/src/idea.o \
./wolfcrypt/src/integer.o \
./wolfcrypt/src/logging.o \
./wolfcrypt/src/md2.o \
./wolfcrypt/src/md5.o \
./wolfcrypt/src/memory.o \
./wolfcrypt/src/pkcs12.o \
./wolfcrypt/src/pkcs7.o \
./wolfcrypt/src/poly1305.o \
./wolfcrypt/src/pwdbased.o \
./wolfcrypt/src/rabbit.o \
./wolfcrypt/src/random.o \
./wolfcrypt/src/ripemd.o \
./wolfcrypt/src/rsa.o \
./wolfcrypt/src/sha.o \
./wolfcrypt/src/sha256.o \
./wolfcrypt/src/sha3.o \
./wolfcrypt/src/sha512.o \
./wolfcrypt/src/signature.o \
./wolfcrypt/src/sp_arm32.o \
./wolfcrypt/src/sp_arm64.o \
./wolfcrypt/src/sp_armthumb.o \
./wolfcrypt/src/sp_c32.o \
./wolfcrypt/src/sp_c64.o \
./wolfcrypt/src/sp_cortexm.o \
./wolfcrypt/src/sp_dsp32.o \
./wolfcrypt/src/sp_int.o \
./wolfcrypt/src/sp_x86_64.o \
./wolfcrypt/src/srp.o \
./wolfcrypt/src/tfm.o \
./wolfcrypt/src/wc_dsp.o \
./wolfcrypt/src/wc_encrypt.o \
./wolfcrypt/src/wc_pkcs11.o \
./wolfcrypt/src/wc_port.o \
./wolfcrypt/src/wolfevent.o \
./wolfcrypt/src/wolfmath.o \
./wolfcrypt/src/ecc.o \
./mbedtls/aes.o \
./mbedtls/algorithm_alt.o \
#./wolfcrypt/src/md4.o \
./wolfcrypt/src/misc.o \
./src/bio.o \
./wolfcrypt/src/evp.o \

obj_ls = \

obj_bs = \

objs_ver = version.o

GEN_LIB = y

include $(MAKE_RULE)

