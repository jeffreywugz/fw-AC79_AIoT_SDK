AR_OUT = $(AR_DIR)/libmbedtls_2_2_1.a

CC_DEFINE +=\
  -D_POSIX_SOURCE \
  -D_SYS_SELECT_H \
  -DCONFIG_CHRDEV \
  -D__unix__ \

ifeq ($(ARCH),pi32v2)
CC_ARGS += \
-ffunction-sections \
-Wno-unused-value \
-Os\

endif

includes = \
-I$(ROOT)/include_lib \
-I$(ROOT)/include_lib/system \
-I$(ROOT)/include_lib/system/generic \
-I$(ROOT)/include_lib/driver/cpu/$(CPU) \
-I$(ROOT)/include_lib/os \
-I$(ROOT)/include_lib/net \
-I$(ROOT)/include_lib/net/$(MBEDTLS_DIR) \
-I$(ROOT)/include_lib/net/$(MBEDTLS_DIR)/mbedtls \
-I$(ROOT)/include_lib/net/$(MBEDTLS_DIR)/mbedtls/configs \
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/app \
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/app/sock_api \
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/src/include \
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/port \

objs = \
./programs/ssl/ssl_server.o \
./programs/ssl/ssl_client1.o \
./programs/ssl/ssl_main.o \
./todo/algorithm_alt.o \
./library/ssl_cache.o \
./library/entropy_poll.o \
./library/rsa.o \
./library/ccm.o \
./library/pk_wrap.o \
./library/gcm.o \
./library/md5.o \
./library/sha1.o \
./library/dhm.o \
./library/havege.o \
./library/pk.o \
./library/pkcs5.o \
./library/cipher.o \
./library/ecp.o \
./library/md4.o \
./library/ecdh.o \
./library/md_wrap.o \
./library/padlock.o \
./library/version_features.o \
./library/camellia.o \
./library/pkwrite.o \
./library/asn1write.o \
./library/ecp_curves.o \
./library/error.o \
./library/ecdsa.o \
./library/x509_create.o \
./library/platform.o \
./library/ssl_cli.o \
./library/aes.o \
./library/ssl_cookie.o \
./library/pkparse.o \
./library/des.o \
./library/ctr_drbg.o \
./library/certs.o \
./library/pem.o \
./library/sha256.o \
./library/x509_crl.o \
./library/x509_crt.o \
./library/pkcs11.o \
./library/cipher_wrap.o \
./library/x509write_crt.o \
./library/version.o \
./library/entropy.o \
./library/ripemd160.o \
./library/hmac_drbg.o \
./library/md2.o \
./library/ssl_tls.o \
./library/base64.o \
./library/x509_csr.o \
./library/ssl_ticket.o \
./library/arc4.o \
./library/x509write_csr.o \
./library/oid.o \
./library/md.o \
./library/pkcs12.o \
./library/aesni.o \
./library/debug.o \
./library/blowfish.o \
./library/timing.o \
./library/xtea.o \
./library/bignum.o \
./library/asn1parse.o \
./library/ssl_ciphersuites.o \
./library/x509.o \
./library/ssl_srv.o \
./library/threading.o \
./library/sha512.o \
./library/memory_buffer_alloc.o \
./library/sock_api_net.o \
./programs/ssl/dtls_server.o \
./programs/ssl/dtls_client.o \

obj_ls = \

obj_bs = \

objs_ver = version.o

GEN_LIB = y

include $(MAKE_RULE)

