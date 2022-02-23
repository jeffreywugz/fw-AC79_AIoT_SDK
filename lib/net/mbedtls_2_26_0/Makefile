
AR_OUT = $(AR_DIR)/libmbedtls_2_26_0.a

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
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/app \
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/app/sock_api \
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/src/include \
-I$(ROOT)/include_lib/net/$(LWIP_DIR)/lwip/port \
-I$(ROOT)/include_lib/net/ \
-I$(ROOT)/include_lib/net/$(MBEDTLS_DIR) \
-I$(ROOT)/include_lib/net/$(MBEDTLS_DIR)/mbedtls \
-I$(ROOT)/include_lib/net/$(MBEDTLS_DIR)/mbedtls/configs \

objs = \
./library/aes.o \
./library/aesni.o \
./library/arc4.o \
./library/aria.o \
./library/asn1parse.o \
./library/asn1write.o \
./library/base64.o \
./library/bignum.o \
./library/blowfish.o \
./library/camellia.o \
./library/ccm.o \
./library/certs.o \
./library/chacha20.o \
./library/chachapoly.o \
./library/cipher.o \
./library/cipher_wrap.o \
./library/cmac.o \
./library/ctr_drbg.o \
./library/debug.o \
./library/des.o \
./library/dhm.o \
./library/ecdh.o \
./library/ecdsa.o \
./library/ecjpake.o \
./library/ecp.o \
./library/ecp_curves.o \
./library/entropy.o \
./library/entropy_poll.o \
./library/error.o \
./library/gcm.o \
./library/havege.o \
./library/hkdf.o \
./library/hmac_drbg.o \
./library/md.o \
./library/md2.o \
./library/md4.o \
./library/md5.o \
./library/memory_buffer_alloc.o \
./library/nist_kw.o \
./library/oid.o \
./library/padlock.o \
./library/pem.o \
./library/pk.o \
./library/pkcs11.o \
./library/pkcs12.o \
./library/pkcs5.o \
./library/pkparse.o \
./library/pkwrite.o \
./library/pk_wrap.o \
./library/platform.o \
./library/platform_util.o \
./library/poly1305.o \
./library/psa_crypto.o \
./library/psa_crypto_client.o \
./library/psa_crypto_driver_wrappers.o \
./library/psa_crypto_ecp.o \
./library/psa_crypto_rsa.o \
./library/psa_crypto_se.o \
./library/psa_crypto_slot_management.o \
./library/psa_crypto_storage.o \
./library/psa_its_file.o \
./library/ripemd160.o \
./library/rsa.o \
./library/rsa_internal.o \
./library/sha1.o \
./library/sha256.o \
./library/sha512.o \
./library/ssl_cache.o \
./library/ssl_ciphersuites.o \
./library/ssl_cli.o \
./library/ssl_cookie.o \
./library/ssl_msg.o \
./library/ssl_srv.o \
./library/ssl_ticket.o \
./library/ssl_tls.o \
./library/ssl_tls13_keys.o \
./library/threading.o \
./library/timing.o \
./library/version.o \
./library/version_features.o \
./library/x509.o \
./library/x509write_crt.o \
./library/x509write_csr.o \
./library/x509_create.o \
./library/x509_crl.o \
./library/x509_crt.o \
./library/x509_csr.o \
./library/xtea.o \
./programs/ssl/dtls_client.o \
./programs/ssl/dtls_server.o \
./programs/ssl/ssl_client1.o \
./programs/ssl/ssl_server.o \
./library/sock_api_net.o \
#./todo/algorithm_alt.o \

obj_ls = \

obj_bs = \

objs_ver = version.o

GEN_LIB = y

include $(MAKE_RULE)


