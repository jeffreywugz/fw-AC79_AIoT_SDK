
#define TEST_NAME "core2"
#include "cmptest.h"

static unsigned char firstkey[32] = { 0x1b, 0x27, 0x55, 0x64, 0x73, 0xe9, 0x85,
                                      0xd4, 0x62, 0xcd, 0x51, 0x19, 0x7a, 0x9a,
                                      0x46, 0xc7, 0x60, 0x09, 0x54, 0x9e, 0xac,
                                      0x64, 0x74, 0xf2, 0x06, 0xc4, 0xee, 0x08,
                                      0x44, 0xf6, 0x83, 0x89
                                    };

static unsigned char nonceprefix[16] = { 0x69, 0x69, 0x6e, 0xe9, 0x55, 0xb6,
                                         0x2b, 0x73, 0xcd, 0x62, 0xbd, 0xa8,
                                         0x75, 0xfc, 0x73, 0xd6
                                       };

static unsigned char c[16] = { 0x65, 0x78, 0x70, 0x61, 0x6e, 0x64, 0x20, 0x33,
                               0x32, 0x2d, 0x62, 0x79, 0x74, 0x65, 0x20, 0x6b
                             };

static unsigned char secondkey[32];

int
main(void)
{
    int i;

    crypto_core_hsalsa20(secondkey, nonceprefix, firstkey, c);
    for (i = 0; i < 32; ++i) {
        if (i > 0) {
            printf(",");
        } else {
            printf(" ");
        }
        printf("0x%02x", (unsigned int) secondkey[i]);
        if (i % 8 == 7) {
            printf("\n");
        }
    }
    return 0;
}
