#include "cmptest.h"

typedef unsigned char u8;
extern void put_buf(const u8 *buf, int len);
//aead_chahca20poly1305算法测试
//combined mode :该模式下，身份验证标签直接附加到加密的消息中。
int
aead_chacha20poly1305_combined_mode(void)
{
    static const unsigned char firstkey[crypto_aead_chacha20poly1305_KEYBYTES]
        = { 0x42, 0x90, 0xbc, 0xb1, 0x54, 0x17, 0x35, 0x31, 0xf3, 0x14, 0xaf,
            0x57, 0xf3, 0xbe, 0x3b, 0x50, 0x06, 0xda, 0x37, 0x1e, 0xce, 0x27,
            0x2a, 0xfa, 0x1b, 0x5d, 0xbd, 0xd1, 0x10, 0x0a, 0x10, 0x07
          };  //32B

    static const unsigned char nonce[crypto_aead_chacha20poly1305_NPUBBYTES]
        = { 0xcd, 0x7c, 0xf6, 0x7b, 0xe3, 0x9c, 0x79, 0x4a }; //8B

    static char input[] = "aead_chachapoly1305_test.";   //需要加密的数据
    unsigned int mlen = strlen(input);                   //加密数据长度

    static char ad[] = "messagetag";                     //附加数据
    unsigned int adlen = strlen(ad);                     //附加数据长度

    unsigned int clen = (mlen + crypto_aead_chacha20poly1305_ABYTES);  //加密后的数据总长度：加密数据 + 认证码
    unsigned char *c = (unsigned char *) sodium_malloc(clen);         //加密后的数据缓存

    unsigned long long found_clen;

    unsigned char *output = (unsigned char *) sodium_malloc(mlen);    //解密后数据缓存
    unsigned long long outlen;

    crypto_aead_chacha20poly1305_encrypt(c, &found_clen, input, mlen,
                                         ad, adlen,
                                         NULL, nonce, firstkey);
    if (found_clen != clen) {
        printf("found_clen is not properly set\n");
        goto EXIT;
    }

    printf("Encrypted string :\n\r");
    printf("%s\n\r", input);
    printf("\n");

    printf("input (hex):\n\r");
    put_buf(input, mlen);
    printf("\n");

    printf("Encrypted (hex):\n\r");
    put_buf(c, clen);
    printf("\n");

    if (crypto_aead_chacha20poly1305_decrypt(output, &outlen, NULL, c, clen,
            ad, adlen,
            nonce, firstkey) != 0) {
        printf("crypto_aead_chacha20poly1305_decrypt() failed\n");
        goto EXIT;
    }
    if (outlen != mlen) {
        printf("outlen is not properly set\n");
        goto EXIT;
    }

    printf("Decrypted (hex):\n\r");
    put_buf(output, outlen);
    printf("\n");

    printf("Decrypted string :\n\r");
    printf("%s\n\r", output);
    printf("\n");

EXIT:
    if (c) {
        sodium_free(c);
    }

    if (output) {
        sodium_free(output);
    }

    return 0;
}

//detached mode: 该模式下，身份验证的标签和加密的消息存储在不同的位置上。
int
aead_chacha20poly1305_detached_mode(void)
{
    static const unsigned char firstkey[crypto_aead_chacha20poly1305_KEYBYTES]
        = { 0x42, 0x90, 0xbc, 0xb1, 0x54, 0x17, 0x35, 0x31, 0xf3, 0x14, 0xaf,
            0x57, 0xf3, 0xbe, 0x3b, 0x50, 0x06, 0xda, 0x37, 0x1e, 0xce, 0x27,
            0x2a, 0xfa, 0x1b, 0x5d, 0xbd, 0xd1, 0x10, 0x0a, 0x10, 0x07
          };  //32B

    static const unsigned char nonce[crypto_aead_chacha20poly1305_NPUBBYTES]
        = { 0xcd, 0x7c, 0xf6, 0x7b, 0xe3, 0x9c, 0x79, 0x4a }; //8B

    static char input[] = "aead_chachapoly1305_test.";   //需要加密的数据
    unsigned int mlen = strlen(input);

    static char ad[] = "messagetag";                     //附加数据
    unsigned int adlen = strlen(ad);                     //附加数据长度

    unsigned int clen = (mlen);                          //需要加密的数据长度
    unsigned char *c = (unsigned char *) sodium_malloc(clen);  //加密后的数据缓存

    //unsigned long long found_clen;

    unsigned char *mac = (unsigned char *) sodium_malloc(crypto_aead_chacha20poly1305_ABYTES);  //认证码缓存
    unsigned long long found_maclen;                                                            //认证码长度


    unsigned char *output = (unsigned char *) sodium_malloc(mlen);
    unsigned long long outlen;

    crypto_aead_chacha20poly1305_encrypt_detached(c,
            mac, &found_maclen,
            input, mlen, ad, adlen,
            NULL, nonce, firstkey);
    if (found_maclen != crypto_aead_chacha20poly1305_abytes()) {
        printf("found_maclen is not properly set\n");
        goto EXIT;
    }

    printf("Encrypted string :\n\r");
    printf("%s\n\r", input);
    printf("\n");

    printf("input (hex):\n\r");
    put_buf(input, mlen);
    printf("\n");

    printf("Encrypted (hex):\n\r");
    put_buf(c, clen);
    printf("\n");

    printf("Mac (hex):\n\r");
    put_buf(mac, found_maclen);
    printf("\n");

    if (crypto_aead_chacha20poly1305_decrypt_detached(output, NULL,
            c, mlen, mac,
            ad, adlen,
            nonce, firstkey) != 0) {
        printf("crypto_aead_chacha20poly1305_decrypt_detached() failed\n");
        goto EXIT;
    }

    printf("Decrypted (hex):\n\r");
    put_buf(output, mlen);
    printf("\n");

    printf("Decrypted string :\n\r");
    printf("%s\n\r", output);
    printf("\n");

EXIT:
    if (c) {
        sodium_free(c);
    }

    if (output) {
        sodium_free(output);
    }

    if (mac) {
        sodium_free(mac);
    }

    return 0;
}