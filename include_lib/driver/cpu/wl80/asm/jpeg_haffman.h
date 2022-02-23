
#ifndef __HAFFMAN_H__
#define __HAFFMAN_H__


#include "typedef.h"


//// 存放2个量化表
//unsigned char YQT[DCTBLOCKSIZE];
//unsigned char UVQT[DCTBLOCKSIZE];
//// 存放2个FDCT变换要求格式的量化表
//float YQT_DCT[DCTBLOCKSIZE];
//float UVQT_DCT[DCTBLOCKSIZE];

//typedef struct tagHUFFCODE
//{
//  u16 code;  // huffman码字
//  u8 length; // 编码长度
//  u16 val;   // 码字对应的值
//}HUFFCODE;


//const static u8 FZBT[64] =
//{
//  0, 1, 5, 6, 14,15,27,28,
//  2, 4, 7, 13,16,26,29,42,
//  3, 8, 12,17,25,30,41,43,
//  9, 11,18,24,31,40,44,53,
//  10,19,23,32,39,45,52,54,
//  20,22,33,38,46,51,55,60,
//  21,34,37,47,50,56,59,61,
//  35,36,48,49,57,58,62,63
//};

//static const double aanScaleFactor[8] = {1.0, 1.387039845, 1.306562965, 1.175875602,1.0, 0.785694958, 0.541196100, 0.275899379};


typedef struct tagHUFFCODE {
    u16 code;  // huffmanÂë×Ö
    u8 length; // Âë×Ö³¤¶È
} HUFFCODE;

#pragma pack(1)
typedef struct jpeg_dht {
    u16 segmentTag;
    u8 reserved;
    u8 length;
    u8 tableInfo;
    u8 huffCode[16];
} JPEGDHT;
#pragma pack()


static const u8 dc_values[12] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

static const u8 ac_values[162] = {
    0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xa,
    0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a,
    0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a,
    0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a,
    0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a,
    0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a,
    0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a,
    0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a,
    0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a,
    0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9a,
    0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa,
    0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba,
    0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca,
    0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda,
    0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea,
    0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9,
    0xfa,
};




#endif





