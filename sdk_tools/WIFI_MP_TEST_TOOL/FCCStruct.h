#ifndef __FCCSTRUCT_H__
#define __FCCSTRUCT_H__
#include "typedef.h"
/* FCCStruct ==NOT Modified==*/

/**
 *    File: F:/SDK/wl80/trunk/master/sdk_tools/WIFI_MP_TEST_TOOL/config/struct/__FCC_CPTX.struct
 *      ID: 1
 *    Name: FCC_CPTX
 *  UIName: 不限封包数量调制信号发送测试
 * Version: 1
 *    Info: Continuous Packet Tx testing,pathx_txpower默认0代表发射功率使用SDK里面的默认配置
**/
#define STRUCT_ID_FCC_CPTX 1
#pragma pack (1) 
typedef struct __FCC_CPTX
{
	u8	mp_channel;	// 
	u8	bandwidth;	// 
	u8	short_gi;	// 
	u8	antenna_x;	// 
	u8	pathx_txpower;	// 0代表发射功率使用SDK里面的默认配置
	u32	mp_rate;	// 
	u32	packet_len;	// 
	u32	send_interval;	// 
}FCC_CPTX;
#pragma pack ()
/*******/

/**
 *    File: F:/SDK/wl80/trunk/master/sdk_tools/WIFI_MP_TEST_TOOL/config/struct/__FCC_CPTX_COUNT.struct
 *      ID: 2
 *    Name: FCC_CPTX_COUNT
 *  UIName: 有限封包数量调制信号发送测试
 * Version: 1
 *    Info: Count Packet Tx testingpathx_tx,pathx_txpower默认0代表发射功率使用SDK里面的默认配置
**/
#define STRUCT_ID_FCC_CPTX_COUNT 2
#pragma pack (1) 
typedef struct __FCC_CPTX_COUNT
{
	u8	mp_channel;	// 
	u8	bandwidth;	// 
	u8	short_gi;	// 
	u8	antenna_x;	// 
	u8	pathx_txpower;	// 0代表发射功率使用SDK里面的默认配置
	u32	mp_rate;	// 
	u32	npackets;	// 
	u32	packet_len;	// 
	u32	send_interval;	// 
}FCC_CPTX_COUNT;
#pragma pack ()
/*******/

/**
 *    File: F:/SDK/wl80/trunk/master/sdk_tools/WIFI_MP_TEST_TOOL/config/struct/__FCC_CS.struct
 *      ID: 3
 *    Name: FCC_CS
 *  UIName: 载波抑制发送测试
 * Version: 1
 *    Info: Carrier suppression testing,pathx_txpower0代表发射功率使用SDK里面的默认配置
**/
#define STRUCT_ID_FCC_CS 3
#pragma pack (1) 
typedef struct __FCC_CS
{
	u8	mp_channel;	// 
	u8	bandwidth;	// 
	u8	short_gi;	// 
	u8	antenna_x;	// 
	u8	pathx_txpower;	// 0代表发射功率使用SDK里面的默认配置
	u32	mp_rate;	// 
	u32	npackets;	// 
}FCC_CS;
#pragma pack ()
/*******/

/**
 *    File: F:/SDK/wl80/trunk/master/sdk_tools/WIFI_MP_TEST_TOOL/config/struct/__FCC_CTX.struct
 *      ID: 4
 *    Name: FCC_CTX
 *  UIName: 无间断调制信号发送测试
 * Version: 1
 *    Info: Continuous Tx testing,pathx_txpower0代表发射功率使用SDK里面的默认配置
**/
#define STRUCT_ID_FCC_CTX 4
#pragma pack (1) 
typedef struct __FCC_CTX
{
	u8	mp_channel;	// 
	u8	bandwidth;	// 
	u8	short_gi;	// 
	u8	antenna_x;	// 
	u8	pathx_txpower;	// 0代表发射功率使用SDK里面的默认配置
	u32	mp_rate;	// 
	u32	npackets;	// 
}FCC_CTX;
#pragma pack ()
/*******/

/**
 *    File: F:/SDK/wl80/trunk/master/sdk_tools/WIFI_MP_TEST_TOOL/config/struct/__FCC_MAC_GET.struct
 *      ID: 5
 *    Name: FCC_MAC_GET
 *  UIName: MAC地址
 * Version: 1
 *    Info: MAC address
**/
#define STRUCT_ID_FCC_MAC_GET 5
#pragma pack (1) 
typedef struct __FCC_MAC_GET
{
}FCC_MAC_GET;
#pragma pack ()
/*******/

/**
 *    File: F:/SDK/wl80/trunk/master/sdk_tools/WIFI_MP_TEST_TOOL/config/struct/__FCC_RX_START.struct
 *      ID: 6
 *    Name: FCC_RX_START
 *  UIName: 开始RX测试
 * Version: 1
 *    Info: Air Rx testing
**/
#define STRUCT_ID_FCC_RX_START 6
#pragma pack (1) 
typedef struct __FCC_RX_START
{
	u8	mp_channel;	// 
	u8	bandwidth;	// 
	u8	short_gi;	// 
	u8	antenna_x;	// 
	u8	filter_enable;	// mac地址白名单开关
	u8	filter_mac0;	// 
	u8	filter_mac1;	// 
	u8	filter_mac2;	// 
	u8	filter_mac3;	// 
	u8	filter_mac4;	// 
	u8	filter_mac5;	// 
}FCC_RX_START;
#pragma pack ()
/*******/

/**
 *    File: F:/SDK/wl80/trunk/master/sdk_tools/WIFI_MP_TEST_TOOL/config/struct/__FCC_RX_STAT_GET.struct
 *      ID: 7
 *    Name: FCC_RX_STAT_GET
 *  UIName: 获取RX测试的接收统计结果
 * Version: 1
 *    Info: 
**/
#define STRUCT_ID_FCC_RX_STAT_GET 7
#pragma pack (1) 
typedef struct __FCC_RX_STAT_GET
{
}FCC_RX_STAT_GET;
#pragma pack ()
/*******/

/**
 *    File: F:/SDK/wl80/trunk/master/sdk_tools/WIFI_MP_TEST_TOOL/config/struct/__FCC_RX_STOP.struct
 *      ID: 8
 *    Name: FCC_RX_STOP
 *  UIName: 退出RX测试
 * Version: 1
 *    Info: 
**/
#define STRUCT_ID_FCC_RX_STOP 8
#pragma pack (1) 
typedef struct __FCC_RX_STOP
{
}FCC_RX_STOP;
#pragma pack ()
/*******/

/**
 *    File: F:/SDK/wl80/trunk/master/sdk_tools/WIFI_MP_TEST_TOOL/config/struct/__FCC_SC.struct
 *      ID: 9
 *    Name: FCC_SC
 *  UIName: 载波发送测试
 * Version: 1
 *    Info: Carrier suppression testing
**/
#define STRUCT_ID_FCC_SC 9
#pragma pack (1) 
typedef struct __FCC_SC
{
	u8	mp_channel;	// 
	u8	bandwidth;	// 
	u8	short_gi;	// 
	u8	antenna_x;	// 
	u8	pathx_txpower;	// 0代表发射功率使用SDK里面的默认配置
	u32	mp_rate;	// 
	u32	npackets;	// 
}FCC_SC;
#pragma pack ()
/*******/

/**
 *    File: F:/SDK/wl80/trunk/master/sdk_tools/WIFI_MP_TEST_TOOL/config/struct/__FCC_SET_PA.struct
 *      ID: 10
 *    Name: FCC_SET_PA
 *  UIName: 设置PA值
 * Version: 1
 *    Info: 
**/
#define STRUCT_ID_FCC_SET_PA 10
#pragma pack (1) 
typedef struct __FCC_SET_PA
{
	u8	value1;	// 
	u8	value2;	// 
	u8	value3;	// 
	u8	value4;	// 
	u8	value5;	// 
	u8	value6;	// 
	u8	value7;	// 
}FCC_SET_PA;
#pragma pack ()
/*******/

/**
 *    File: F:/SDK/wl80/trunk/master/sdk_tools/WIFI_MP_TEST_TOOL/config/struct/__FCC_SET_XOSC.struct
 *      ID: 11
 *    Name: FCC_SET_XOSC
 *  UIName: 晶振电容
 * Version: 1
 *    Info: 范围0-15
**/
#define STRUCT_ID_FCC_SET_XOSC 11
#pragma pack (1) 
typedef struct __FCC_SET_XOSC
{
	u8	xosc_l;	// 
	u8	xosc_r;	// 
}FCC_SET_XOSC;
#pragma pack ()
/*******/

#endif
