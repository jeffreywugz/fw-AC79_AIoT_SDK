#include <string.h>
#include "generic/typedef.h"
#include "app_config.h"
#include "asm/crc16.h"
#include "database.h"
#include "syscfg/syscfg_id.h"


/*
// 设置WIFI DEBUG 信息输出等级
Debug information verbosity: lower values indicate higher urgency
0:RT_DEBUG_OFF
1:RT_DEBUG_ERROR
2:RT_DEBUG_WARN
3:RT_DEBUG_TRACE
4:RT_DEBUG_INFO
5:RT_DEBUG_LOUD
*/
const u8 RTDebugLevel = 2;

const u16 MAX_CHANNEL_TIME_BSS_INFRA = 200;//扫描每个信道停留时间,单位ms,最小20ms, 200-400ms最佳

const  char WIFI_CHANNEL_QUALITY_INDICATION_BAD = 5; //STA模式下的信道通信质量差阈值,一旦低于这个值就断线重连,如果配置为-1则信号质量再差也不通知断线,但是太久不重连会被路由器认为死亡踢掉的风险

#if defined  CONFIG_NO_SDRAM_ENABLE
const u16 MAX_PACKETS_IN_QUEUE = 16; //配置WiFi驱动最大发送数据包队列
const u16 MAX_PACKETS_IN_MCAST_PS_QUEUE	= 4;	//配置WiFi驱动最大发送数据包队列 //8	16 modify by lyx 32
const u16 MAX_PACKETS_IN_PS_QUEUE	=	2;	//配置WiFi驱动最大发送数据包队列  //128	/*16 */
#else
const u16 MAX_PACKETS_IN_QUEUE = 64; //配置WiFi驱动最大发送数据包队列
const u16 MAX_PACKETS_IN_MCAST_PS_QUEUE = 8;  //配置WiFi驱动最大发送MCAST-power-save包队列 //modify by lyx 32
const u16 MAX_PACKETS_IN_PS_QUEUE	= 16; //配置WiFi驱动最大发送power-save队列	//128	/*16 */
#endif

const u8 RFIinitUseTrimValue = 1;//记忆wifi rf 初始化使用vm记忆的trim的值,可大大降低wifi初始化时间

#if 0
WirelessMode:
PHY_11BG_MIXED = 0,
PHY_11B = 1,
PHY_11A = 2,,
PHY_11ABG_MIXED = 3,,
PHY_11G = 4,
PHY_11ABGN_MIXED,	/* both band   5 */
PHY_11N_2_4G,		/* 11n-only with 2.4G band      6 */
PHY_11GN_MIXED,		/* 2.4G band      7 */
PHY_11AN_MIXED,		/* 5G  band       8 */
PHY_11BGN_MIXED,	/* if check 802.11b.      9 */
PHY_11AGN_MIXED,	/* if check 802.11b.      10 */
PHY_11N_5G,		/* 11n-only with 5G band                11 */


#endif

/*
 MaxStaNum  最大连接数不能超过 MAX_LEN_OF_MAC_TABLE(5)
 */
static char WLAP_DAT[] = {
    "\
#The word of \"Default\" must not be removed\n\
Default\n\
MacAddress=00:00:00:00:00:00\n\
CountryRegion=1\n\
CountryRegionABand=0\n\
CountryCode=CN\n\
BssidNum=1\n\
MaxStaNum=2\n\
IdleTimeout=300\n\
SSID=####SSID_LENTH_MUST_LESS_THAN_32\n\
WirelessMode=9\n\
TxRate=0\n\
Channel=11#\n\
BasicRate=15\n\
BeaconPeriod=100\n\
DtimPeriod=1\n\
TxPower=100\n\
DisableOLBC=0\n\
BGProtection=0\n\
TxAntenna=\n\
RxAntenna=\n\
TxPreamble=1\n\
RTSThreshold=2347\n\
FragThreshold=2346\n\
TxBurst=0\n\
PktAggregate=0\n\
TurboRate=0\n\
WmmCapable=0\n\
APSDCapable=0\n\
DLSCapable=0\n\
APAifsn=3;7;1;1\n\
APCwmin=4;4;3;2\n\
APCwmax=6;10;4;3\n\
APTxop=0;0;94;47\n\
APACM=0;0;0;0\n\
BSSAifsn=3;7;2;2\n\
BSSCwmin=4;4;3;2\n\
BSSCwmax=10;10;4;3\n\
BSSTxop=0;0;94;47\n\
BSSACM=0;0;0;0\n\
AckPolicy=0;0;0;0\n\
NoForwarding=0\n\
NoForwardingBTNBSSID=0\n\
HideSSID=0\n\
StationKeepAlive=0\n\
ShortSlot=1\n\
AutoChannelSelect=0\n\
IEEE8021X=0\n\
IEEE80211H=0\n\
CSPeriod=10\n\
WirelessEvent=0\n\
IdsEnable=0\n\
AuthFloodThreshold=32\n\
AssocReqFloodThreshold=32\n\
ReassocReqFloodThreshold=32\n\
ProbeReqFloodThreshold=32\n\
DisassocFloodThreshold=32\n\
DeauthFloodThreshold=32\n\
EapReqFooldThreshold=32\n\
PreAuth=0\n\
AuthMode=################\n\
EncrypType=################\n\
RekeyInterval=0\n\
RekeyMethod=DISABLE\n\
PMKCachePeriod=10\n\
WPAPSK=#########wpa_passphrase_lenth_must_more_than_7_and_less_than_63\n\
DefaultKeyID=1\n\
Key1Type=0\n\
Key1Str=\n\
Key2Type=0\n\
Key2Str=\n\
Key3Type=0\n\
Key3Str=\n\
Key4Type=0\n\
Key4Str=\n\
HSCounter=0\n\
AccessPolicy0=0\n\
AccessControlList0=\n\
AccessPolicy1=0\n\
AccessControlList1=\n\
AccessPolicy2=0\n\
AccessControlList2=\n\
AccessPolicy3=0\n\
AccessControlList3=\n\
WdsEnable=0\n\
WdsEncrypType=NONE\n\
WdsList=\n\
WdsKey=\n\
RADIUS_Server=192.168.2.3\n\
RADIUS_Port=1812\n\
RADIUS_Key=ralink\n\
own_ip_addr=192.168.5.234\n\
EAPifname=br0\n\
PreAuthifname=br0\n\
HT_HTC=0\n\
HT_RDG=1\n\
HT_EXTCHA=0\n\
HT_LinkAdapt=0\n\
HT_OpMode=0\n\
HT_MpduDensity=4\n\
HT_BW=0\n\
HT_BADecline=0\n\
HT_AutoBA=1\n\
HT_AMSDU=0\n\
HT_BAWinSize=64\n\
HT_GI=0\n\
HT_MCS=33\n\
MeshId=MESH\n\
MeshAutoLink=1\n\
MeshAuthMode=OPEN\n\
MeshEncrypType=NONE\n\
MeshWPAKEY=\n\
MeshDefaultkey=1\n\
MeshWEPKEY=\n\
WscManufacturer=\n\
WscModelName=\n\
WscDeviceName=\n\
WscModelNumber=\n\
WscSerialNumber=\n\
RadioOn=1\n\
PMFMFPC=0\n\
PMFMFPR=0\n\
PMFSHA256=0"
};



/*PSMode=CAM\n\*/
/*PSMode=Legacy_PSP\n\//STA休眠*/
#ifdef CONFIG_LOW_POWER_ENABLE
#define WL_STA_SLEEP	1
#else
#define WL_STA_SLEEP	0
#endif

static const char WL_STA_DAT[] = {
    "\
#The word of \"Default\" must not be removed\n\
Default\n\
MacAddress=00:00:00:00:00:00\n\
CountryRegion=1\n\
CountryRegionABand=0\n\
CountryCode=CN\n\
ChannelGeography=1\n\
SSID=DEFAULT_CONNECT_SSID\n\
NetworkType=Infra\n\
WirelessMode=9\n\
Channel=1\n\
BeaconPeriod=100\n\
TxPower=100\n\
BGProtection=0\n\
TxPreamble=1\n\
RTSThreshold=2347\n\
FragThreshold=2346\n\
TxBurst=0\n\
PktAggregate=0\n\
WmmCapable=1\n\
AckPolicy=0;0;0;0\n\
AuthMode=OPEN\n\
EncrypType=NONE\n\
WPAPSK=\n\
DefaultKeyID=1\n\
Key1Type=0\n\
Key1Str=\n\
Key2Type=0\n\
Key2Str=\n\
Key3Type=0\n\
Key3Str=\n\
Key4Type=0\n\
Key4Str=\n"
#if WL_STA_SLEEP
    "PSMode=Legacy_PSP\n"
#else
    "PSMode=CAM\n"
#endif
    "AutoRoaming=0\n\
RoamThreshold=70\n\
APSDCapable=1\n\
APSDAC=0;0;0;0\n\
HT_RDG=1\n\
HT_EXTCHA=0\n\
HT_OpMode=0\n\
HT_MpduDensity=4\n\
HT_BW=0\n\
HT_BADecline=0\n\
HT_AutoBA=1\n\
HT_AMSDU=0\n\
HT_BAWinSize=64\n\
HT_GI=0\n\
HT_MCS=33\n\
HT_MIMOPSMode=3\n\
HT_DisallowTKIP=1\n\
HT_STBC=0\n\
EthConvertMode=\n\
EthCloneMac=\n\
IEEE80211H=0\n\
TGnWifiTest=0\n\
WirelessEvent=0\n\
MeshId=MESH\n\
MeshAutoLink=1\n\
MeshAuthMode=OPEN\n\
MeshEncrypType=NONE\n\
MeshWPAKEY=\n\
MeshDefaultkey=1\n\
MeshWEPKEY=\n\
CarrierDetect=0\n\
AntDiversity=0\n\
BeaconLostTime=16\n\
FtSupport=0\n\
Wapiifname=ra0\n\
WapiPsk=\n\
WapiPskType=\n\
WapiUserCertPath=\n\
WapiAsCertPath=\n\
PSP_XLINK_MODE=0\n\
WscManufacturer=\n\
WscModelName=\n\
WscDeviceName=\n\
WscModelNumber=\n\
WscSerialNumber=\n\
RadioOn=1\n\
WIDIEnable=0\n\
P2P_L2SD_SCAN_TOGGLE=8\n\
Wsc4digitPinCode=0\n\
P2P_WIDIEnable=0\n\
PMFMFPC=0\n\
PMFMFPR=0\n\
PMFSHA256=0"
};

const char *GET_WL_STA_DAT(void)
{
    return WL_STA_DAT;
}

int GET_WL_STA_DAT_LEN(void)
{
    return strlen(WL_STA_DAT);
}

const char *GET_WL_AP_DAT(void)
{
    return WLAP_DAT;
}

int GET_WL_AP_DAT_LEN(void)
{
    return strlen(WLAP_DAT);
}



int wl_set_wifi_channel(int channel)
{
    char channel_str[3] = {0};

    if (channel < 1 || channel > 14) {
        return -1;
    }

    sprintf(channel_str, "%d", channel);

    const char *find_channel = "Channel=";
    char *channel_position = strstr(GET_WL_AP_DAT(), find_channel) + strlen(find_channel);

    strcpy(channel_position, channel_str);
    channel_position[strlen(channel_str)] = '\n';
    memset(channel_position + strlen(channel_str) + 1, '#', 3 - strlen(channel_str) - 1);

    return 0;
}

static int wl_set_ssid(const char *ssid)
{
    if (ssid == 0) {
        return -1;
    }

    const char *find_ssid = "SSID=";
    char *ssid_position = strstr(GET_WL_AP_DAT(), find_ssid) + strlen(find_ssid);

    if (strlen(ssid) > 32) {
        printf("set_ssid_passphrase fail,ssid len (0x%lx) longer than 32!\n", strlen(ssid));
        return -1;
    }

    strcpy(ssid_position, ssid);
    ssid_position[strlen(ssid)] = '\n';
    memset(ssid_position + strlen(ssid) + 1, '#', 32 - strlen(ssid) - 1);

    return 0;
}

static int wl_set_passphrase(const char *passphrase)
{
    if (passphrase == 0) {
        return -1;
    }

    const char *AUTH_MODE, *ENCRYP_TYPE;
    const char *find_wpa_passphrase = "WPAPSK=";
    const char *find_AuthMode = "AuthMode=";
    const char *find_EncrypType = "EncrypType=";

    char *wpa_passphrase_position = strstr(GET_WL_AP_DAT(), find_wpa_passphrase) + strlen(find_wpa_passphrase);
    char *AuthMode_position = strstr(GET_WL_AP_DAT(), find_AuthMode) + strlen(find_AuthMode);
    char *EncrypType_position = strstr(GET_WL_AP_DAT(), find_EncrypType) + strlen(find_EncrypType);

    if (strcmp(passphrase, "")) {
        if ((strlen(passphrase) < 8) || (strlen(passphrase) > 63)) {
            printf("set_ssid_passphrase fail,passphrase len (0x%lx) must more than 7 and less than 63!\n", strlen(passphrase));
            return -1;
        }

        strcpy(wpa_passphrase_position, passphrase);
        wpa_passphrase_position[strlen(passphrase)] = '\n';
        memset(wpa_passphrase_position + strlen(passphrase) + 1, '#', 63 - strlen(passphrase) - 1);

        AUTH_MODE = "WPA2PSK";
        strcpy(AuthMode_position, AUTH_MODE);
        AuthMode_position[strlen(AUTH_MODE)] = '\n';
        memset(AuthMode_position + strlen(AUTH_MODE) + 1, '#', 16 - strlen(AUTH_MODE) - 1);

        ENCRYP_TYPE = "AES";
        strcpy(EncrypType_position, ENCRYP_TYPE);
        EncrypType_position[strlen(ENCRYP_TYPE)] = '\n';
        memset(EncrypType_position + strlen(ENCRYP_TYPE) + 1, '#', 16 - strlen(ENCRYP_TYPE) - 1);
    } else {
        AUTH_MODE = "OPEN";
        strcpy(AuthMode_position, AUTH_MODE);
        AuthMode_position[strlen(AUTH_MODE)] = '\n';
        memset(AuthMode_position + strlen(AUTH_MODE) + 1, '#', 16 - strlen(AUTH_MODE) - 1);

        ENCRYP_TYPE = "NONE";
        strcpy(EncrypType_position, ENCRYP_TYPE);
        EncrypType_position[strlen(ENCRYP_TYPE)] = '\n';
        memset(EncrypType_position + strlen(ENCRYP_TYPE) + 1, '#', 16 - strlen(ENCRYP_TYPE) - 1);
    }

    return 0;
}



int wl_ap_init(const char *ssid, const char *passphrase)
{
    int ret;
    wl_set_ssid(ssid);
    wl_set_passphrase(passphrase);

//    printf("WLAP_DAT = \r\n %s \r\n",GET_WL_AP_DAT());

    return 0;
}

void set_bss_table_record(char *data, u32 len)
{
#ifdef WIFI_COLD_START_FAST_CONNECTION
    syscfg_write(WIFI_BSS_TABLE, data, len);
#endif
}
int get_bss_table_record(char *data, u32 len)
{
#ifdef WIFI_COLD_START_FAST_CONNECTION
    int ret;
    ret = syscfg_read(WIFI_BSS_TABLE, data, len);
    if (ret < 0) {
        return 0;
    }
    return 1;
#else
    return 0;
#endif
}

void set_scan_result_record(char *data, u32 len)
{
#ifdef WIFI_COLD_START_FAST_CONNECTION
    syscfg_write(WIFI_SCAN_INFO, data, len);
#endif
}
int get_stored_scan_record(char *data, u32 len)
{
#ifdef WIFI_COLD_START_FAST_CONNECTION
    int ret;
    ret = syscfg_read(WIFI_SCAN_INFO, data, len);
    if (ret < 0) {
        return 0;
    }
    return 1;
#else
    return 0;
#endif
}

#if 1
struct pmk_info {
    u32 crc;
    char pmk[32];
};
int get_stored_pmk(char *password, char *ssid, int ssidlength, char *output)
{
    int ret;
    u32 crc;
    struct pmk_info info;

    /* ret = db_select_buffer(WIFI_PMK_INFO, (char *)&info, sizeof(struct pmk_info)); */
    ret = syscfg_read(WIFI_PMK_INFO, (char *)&info, sizeof(struct pmk_info));
    if (ret < 0) {
        return 0;
    }

    crc = CRC16(password, strlen(password));
    crc += CRC16(ssid, ssidlength);

    if (crc != info.crc) {
        return 0;
    }

    memcpy(output, info.pmk, sizeof(info.pmk));
    printf("stored pmk_info match[%s] [%s],,,\n", ssid, password);
    return 1;
}
void set_stored_pmk(char *password, char *ssid, int ssidlength, char *output)
{
    printf("pmk_info not match, store... [%s] [%s],,,\n", ssid, password);

    struct pmk_info info;
    info.crc = CRC16(password, strlen(password));
    info.crc += CRC16(ssid, ssidlength);
    memcpy(info.pmk, output, sizeof(info.pmk));
    /* db_update_buffer(WIFI_PMK_INFO, (char *)&info, sizeof(struct pmk_info)); */
    syscfg_write(WIFI_PMK_INFO, (char *)&info, sizeof(struct pmk_info));
}

#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int wifi_get_rf_trim_data(void *info, int size)
{
    if (syscfg_read(VM_WIFI_RF_INIT_INFO, info, size) != size) {
        return -1;
    }
    return 0;
}
int wifi_set_rf_trim_data(void *info, int size)
{
    if (syscfg_write(VM_WIFI_RF_INIT_INFO, info, size) != size) {
        return -1;
    }
    return 0;
}

#if 0  //WIFI RX 中庸策略
const u32 STF_END_CNT_THR_VAL = 179;
const u32 STF_DET_CNT_THR_VAL = 16;
const u32 DS_DET_CNT_THR0_VAL = 3;
const u32 STF_DET_THR1_VAL = 180;
const u32 STF_DET_THR2_VAL = 180;
const u32 STF_DET_THR3_VAL = 180;
const u32 DSSS_XCDET_THR1_VAL = 120;
const u32 DSSS_XCDET_THR2_VAL = 80;
const u32 DS_VLD_CNT_THR_VAL = 72;
const u32 DS_DET_CNT_THR1_VAL = 3;

///=======================================================//
///       ram adr        ctl
///      0  - 127        feq    index 0 - 127
///      128 - 223       agc    index 0 - 95
///      224 - 225       apc    index 0 - 31
///=======================================================//
const unsigned long wl_rfd_ram_lut [256][2] = {
    //{  H   ,   L }
/// =================================================
///               FEQ
///
///
///==================================================
    /* 0 */ { 0x06405555	,	0x8A1B01A0 },
    /* 1 */ { 0x06410000	,	0x8A3B01A0 },
    /* 2 */ { 0x0641AAAB	,	0x8A5B01A0 },
    /* 3 */ { 0x06425555	,	0x8A7B01A0 },
    /* 4 */ { 0x06430000	,	0x8A9B01A0 },
    /* 5 */ { 0x0643AAAB	,	0x8ABB01A0 },
    /* 6 */ { 0x06445555	,	0x8ADB01A0 },
    /* 7 */ { 0x06450000	,	0x8AFB01A0 },
    /* 8 */ { 0x0645AAAB	,	0x8B1B01A0 },
    /* 9 */ { 0x06465555	,	0x8B3B01A0 },
    /*10 */ { 0x06470000	,	0x8B5B01A0 },
    /*11 */ { 0x0647AAAB	,	0x8B7B01A0 },
    /*12 */ { 0x06485555	,	0x8B9B01A0 },
    /*13 */ { 0x06490000	,	0x8BBB01A0 },
    /*14 */ { 0x0649AAAB	,	0x8BDB01A0 },
    /*15 */ { 0x064A5555	,	0x8BFB01A0 },
    /*16 */ { 0x064B0000	,	0x8C1B01A0 },
    /*17 */ { 0x064BAAAB	,	0x8C3B01A0 },
    /*18 */ { 0x064C5555	,	0x8C5B01A0 },
    /*19 */ { 0x064D0000	,	0x8C7B01A0 },
    /*20 */ { 0x064DAAAB	,	0x8C9B01A0 },
    /*21 */ { 0x064E5555	,	0x8CBB01A0 },
    /*22 */ { 0x064F0000	,	0x8CDB01A0 },
    /*23 */ { 0x064FAAAB	,	0x8CFB01A0 },
    /*24 */ { 0x06505555	,	0x8D1B01A0 },
    /*25 */ { 0x06510000	,	0x8D3B01A0 },
    /*26 */ { 0x0651AAAB	,	0x8D5B01A0 },
    /*27 */ { 0x06525555	,	0x8D7B01A0 },
    /*28 */ { 0x06530000	,	0x8D9B01A0 },
    /*29 */ { 0x0653AAAB	,	0x8DBB01A0 },
    /*30 */ { 0x06545555	,	0x8DDB01A0 },
    /*31 */ { 0x06550000	,	0x8DFB01A0 },
    /*32 */ { 0x0655AAAB	,	0x8E1B01A0 },
    /*33 */ { 0x06565555	,	0x8E3B01A0 },
    /*34 */ { 0x06570000	,	0x8E5B01A0 },
    /*35 */ { 0x0657AAAB	,	0x8E7B01A0 },
    /*36 */ { 0x06585555	,	0x8E9B01A0 },
    /*37 */ { 0x06590000	,	0x8EBB01A0 },
    /*38 */ { 0x0659AAAB	,	0x8EDB01A0 },
    /*39 */ { 0x065A5555	,	0x8EFB01A0 },
    /*40 */ { 0x065B0000	,	0x8F1B01A0 },
    /*41 */ { 0x065BAAAB	,	0x8F3B01A0 },
    /*42 */ { 0x065C5555	,	0x8F5B01A0 },
    /*43 */ { 0x065D0000	,	0x8F7B01A0 },
    /*44 */ { 0x065DAAAB	,	0x8F9B01A0 },
    /*45 */ { 0x065E5555	,	0x8FBB01A0 },
    /*46 */ { 0x065F0000	,	0x8FDB01A0 },
    /*47 */ { 0x065FAAAB	,	0x8FFB01A0 },
    /*48 */ { 0x06605555	,	0x901B01A0 },
    /*49 */ { 0x06610000	,	0x903B01A0 },
    /*50 */ { 0x0661AAAB	,	0x905B01A0 },
    /*51 */ { 0x06625555	,	0x907B01A0 },
    /*52 */ { 0x06630000	,	0x909B01A0 },
    /*53 */ { 0x0663AAAB	,	0x90BB01A0 },
    /*54 */ { 0x06645555	,	0x90DB01A0 },
    /*55 */ { 0x06650000	,	0x90FB01A0 },
    /*56 */ { 0x0665AAAB	,	0x911B01A0 },
    /*57 */ { 0x06665555	,	0x913B01A0 },
    /*58 */ { 0x06670000	,	0x915B01A0 },
    /*59 */ { 0x0667AAAB	,	0x917B01A0 },
    /*60 */ { 0x06685555	,	0x919B01A0 },
    /*61 */ { 0x06690000	,	0x91BB01A0 },
    /*62 */ { 0x0669AAAB	,	0x91DB01A0 },
    /*63 */ { 0x066A5555	,	0x91FB01A0 },
    /*64 */ { 0x066B0000	,	0x921B01A0 },
    /*65 */ { 0x066BAAAB	,	0x923B01A0 },
    /*66 */ { 0x066C5555	,	0x925B01A0 },
    /*67 */ { 0x066D0000	,	0x927B01A0 },
    /*68 */ { 0x066DAAAB	,	0x929B01A0 },
    /*69 */ { 0x066E5555	,	0x92BB01A0 },
    /*70 */ { 0x066F0000	,	0x92DB01A0 },
    /*71 */ { 0x066FAAAB	,	0x92FB01A0 },
    /*72 */ { 0x06705555	,	0x931B01A0 },
    /*73 */ { 0x06710000	,	0x933B01A0 },
    /*74 */ { 0x0671AAAB	,	0x935B01A0 },
    /*75 */ { 0x06725555	,	0x937B01A0 },
    /*76 */ { 0x06730000	,	0x939B01A0 },
    /*77 */ { 0x0673AAAB	,	0x93BB01A0 },
    /*78 */ { 0x06745555	,	0x93DB01A0 },
    /*79 */ { 0x06750000	,	0x93FB01A0 },
    /*80 */ { 0x0675AAAB	,	0x941B01A0 },
    /*81 */ { 0x06765555	,	0x943B01A0 },
    /*82 */ { 0x06770000	,	0x945B01A0 },
    /*83 */ { 0x0677AAAB	,	0x947B01A0 },
    /*84 */ { 0x06785555	,	0x949B01A0 },
    /*85 */ { 0x06790000	,	0x94BB01A0 },
    /*86 */ { 0x06790000	,	0x94BB01A0 },
    /*87 */ { 0x06790000	,	0x94BB01A0 },
    /*88 */ { 0x06790000	,	0x94BB01A0 },
    /*89 */ { 0x06790000	,	0x94BB01A0 },
    /*90 */ { 0x06790000	,	0x94BB01A0 },
    /*91 */ { 0x06790000	,	0x94BB01A0 },
    /*92 */ { 0x06790000	,	0x94BB01A0 },
    /*93 */ { 0x06790000	,	0x94BB01A0 },
    /*94 */ { 0x06790000	,	0x94BB01A0 },
    /*95 */ { 0x06790000	,	0x94BB01A0 },
    /*96 */ { 0x06790000	,	0x94BB01A0 },
    /*97 */ { 0x06790000	,	0x94BB01A0 },
    /*98 */ { 0x06790000	,	0x94BB01A0 },
    /*99 */ { 0x06790000	,	0x94BB01A0 },
    /*100 */{ 0x06415555	,	0x8A3B01A0 },
    /*101 */{ 0x0644AAAB	,	0x8ADB01A0 },
    /*102 */{ 0x06480000	,	0x8B7B01A0 },
    /*103 */{ 0x064B5555	,	0x8C1B01A0 },
    /*104 */{ 0x064EAAAB	,	0x8CBB01A0 },
    /*105 */{ 0x06520000	,	0x8D5B01A0 },
    /*106 */{ 0x06555555	,	0x8DFB01A0 },
    /*107 */{ 0x0658AAAB	,	0x8E9B01A0 },
    /*108 */{ 0x065C0000	,	0x8F3B01A0 },
    /*109 */{ 0x065F5555	,	0x8FDB01A0 },
    /*110 */{ 0x0662AAAB	,	0x907B01A0 },
    /*111 */{ 0x06660000	,	0x911B01A0 },
    /*112 */{ 0x06695555	,	0x91BB01A0 },
    /*113 */{ 0x066CAAAB	,	0x925B01A0 },
    /*114 */{ 0x06700000	,	0x92FB01A0 },
    /*115 */{ 0x06735555	,	0x939B01A0 },
    /*116 */{ 0x0676AAAB	,	0x943B01A0 },
    /*117 */{ 0x067A0000	,	0x94DB01A0 },
    /*118 */{ 0x067A0000	,	0x94DB01A0 },
    /*119 */{ 0x067A0000	,	0x94DB01A0 },
    /*120 */{ 0x067A0000	,	0x94DB01A0 },
    /*121 */{ 0x067A0000	,	0x94DB01A0 },
    /*122 */{ 0x067A0000	,	0x94DB01A0 },
    /*123 */{ 0x067A0000	,	0x94DB01A0 },
    /*124 */{ 0x067A0000	,	0x94DB01A0 },
    /*125 */{ 0x067A0000	,	0x94DB01A0 },
    /*126 */{ 0x067A0000	,	0x94DB01A0 },
    /*127 */{ 0x067A0000	,	0x94DB01A0 },
/// =================================================
///              AGC
/// [32]      [31:29]  [28:26]  [25:23]   [22:20] [19:18]  [17:15]    [14:11]   [10:4]   [3:2]        [1:0]
/// TRX_RFSW TRX_RFCAP VG1_5-0 VG0_12/6/0  HPF_GS RXF3_12/6RXF_12/6/0 MIX_TIA_RT MIX_TIA_CT MIX_GM_GS LNA_GS
///==================================================
    /*128*/{0x0000007A	, 0x008087F0	},
    /*129*/{0x0000007A	, 0x048087F0	},
    /*130*/{0x0000007A	, 0x088087F0	},
    /*131*/{0x0000007A	, 0x0C8087F0	},
    /*132*/{0x0000007A	, 0x108087F0	},
    /*133*/{0x0000007A	, 0x008087F4	},
    /*134*/{0x0000007A	, 0x048087F4	},
    /*135*/{0x0000007A	, 0x088087F4	},
    /*136*/{0x0000000A	, 0x008087F4	},
    /*137*/{0x0000000A	, 0x048087F4	},
    /*138*/{0x0000000A	, 0x088087F4	},
    /*139*/{0x0000000A	, 0x0C8087F4	},
    /*140*/{0x0000000A	, 0x108087F4	},
    /*141*/{0x0000000A	, 0x148087F4	},
    /*142*/{0x0000000A	, 0x008087F5	},
    /*143*/{0x0000000A	, 0x048087F5	},
    /*144*/{0x0000000A	, 0x088087F5	},
    /*145*/{0x0000000A	, 0x0C8087F5	},
    /*146*/{0x0000000A	, 0x108087F5	},
    /*147*/{0x0000000A	, 0x008087F6	},
    /*148*/{0x0000000A	, 0x048087F6	},
    /*149*/{0x0000000A	, 0x088087F6	},
    /*150*/{0x0000000A	, 0x0C8087F6	},
    /*151*/{0x0000000A	, 0x108087F6	},
    /*152*/{0x0000000A	, 0x148087F6	},
    /*153*/{0x0000000A	, 0x011087F6	},
    /*154*/{0x0000000A	, 0x051087F6	},
    /*155*/{0x0000000A	, 0x091087F6	},
    /*156*/{0x0000000A	, 0x0D1087F6	},
    /*157*/{0x0000000A	, 0x111087F6	},
    /*158*/{0x0000000A	, 0x151087F6	},
    /*159*/{0x0000000A	, 0x012487F6	},
    /*160*/{0x0000000A	, 0x052487F6	},
    /*161*/{0x0000000A	, 0x092487F6	},
    /*162*/{0x0000000A	, 0x0D2487F6	},
    /*163*/{0x0000000A	, 0x008087F7	},
    /*164*/{0x0000000A	, 0x048087F7	},
    /*165*/{0x0000000A	, 0x088087F7	},
    /*166*/{0x0000000A	, 0x0C8087F7	},
    /*167*/{0x0000000A	, 0x108087F7	},
    /*168*/{0x0000000A	, 0x148087F7	},
    /*169*/{0x0000000A	, 0x011087F7	},
    /*170*/{0x0000000A	, 0x051087F7	},
    /*171*/{0x0000000A	, 0x091087F7	},
    /*172*/{0x0000000A	, 0x0D1087F7	},
    /*173*/{0x0000000A	, 0x111087F7	},
    /*174*/{0x0000000A	, 0x151087F7	},
    /*175*/{0x0000000A	, 0x012487F7	},
    /*176*/{0x0000000A	, 0x052487F7	},
    /*177*/{0x0000000A	, 0x092487F7	},
    /*178*/{0x0000000A	, 0x0D2487F7	},
    /*179*/{0x0000000A	, 0x112487F7	},
    /*180*/{0x0000000A	, 0x15109C57	},
    /*181*/{0x0000000A	, 0x01249C57	},
    /*182*/{0x0000000A	, 0x05249C57	},
    /*183*/{0x0000000A	, 0x09249C57	},
    /*184*/{0x0000000A	, 0x0D249C57	},
    /*185*/{0x0000000A	, 0x11249C57	},
    /*186*/{0x0000000A	, 0x0110C8F7	},
    /*187*/{0x0000000A	, 0x0510C8F7	},
    /*188*/{0x0000000A	, 0x0910C8F7	},
    /*189*/{0x0000000A	, 0x0D10C8F7	},
    /*190*/{0x0000000A	, 0x1110C8F7	},
    /*191*/{0x0000000A	, 0x1510C8F7	},
    /*192*/{0x0000000A	, 0x0124C8F7	},
    /*193*/{0x0000000A	, 0x0524C8F7	},
    /*194*/{0x0000000A	, 0x0924C8F7	},
    /*195*/{0x0000000A	, 0x0D24C8F7	},
    /*196*/{0x0000000A	, 0x1124C8F7	},
    /*197*/{0x0000000A	, 0x1524C8F7	},
    /*198*/{0x0000000A	, 0x013548F7	},
    /*199*/{0x0000000A	, 0x053548F7	},
    /*200*/{0x0000000A	, 0x093548F7	},
    /*201*/{0x0000000A	, 0x0D3548F7	},
    /*202*/{0x0000000A	, 0x113548F7	},
    /*203*/{0x0000000A	, 0x153548F7	},
    /*204*/{0x0000000A	, 0x024548F7	},
    /*205*/{0x0000000A	, 0x064548F7	},
    /*206*/{0x0000000A	, 0x0A4548F7	},
    /*207*/{0x0000000A	, 0x0E4548F7	},
    /*208*/{0x0000000A	, 0x124548F7	},
    /*209*/{0x0000000A	, 0x164548F7	},
    /*210*/{0x0000000A	, 0x025948F7	},
    /*211*/{0x0000000A	, 0x065948F7	},
    /*212*/{0x0000000A	, 0x0A5948F7	},
    /*213*/{0x0000000A	, 0x0E5948F7	},
    /*214*/{0x0000000A	, 0x125948F7	},
    /*215*/{0x0000000A	, 0x165948F7	},
    /*216*/{0x0000000A	, 0x026A48F7	},
    /*217*/{0x0000000A	, 0x066A48F7	},
    /*218*/{0x0000000A	, 0x0A6A48F7	},
    /*219*/{0x0000000A	, 0x0E6A48F7	},
    /*220*/{0x0000000A	, 0x126A48F7	},
    /*221*/{0x0000000A	, 0x166A48F7	},
    /*222*/{0x0000000A	, 0x166A48F7	},
    /*223*/{0x0000000A	, 0x166A48F7	},
/// =================================================
///               APC
///
///
///==================================================

    /*224*/{ 0x00000078, 0x17836876},
    /*225*/{ 0x00000078, 0x17A36876},
    /*226*/{ 0x00000078, 0x17C36876},
    /*227*/{ 0x00000078, 0x17E36876},
    /*228*/{ 0x00000078, 0x57E36876},
    /*229*/{ 0x00000078, 0x97E36876},
    /*230*/{ 0x00000078, 0xD7E36876},
    /*231*/{ 0x00000078, 0xD7E36876},
    /*232*/{ 0x00000078, 0xD7E36876},
    /*233*/{ 0x00000078, 0xD7E36876},
    /*234*/{ 0x00000078, 0xD7E36876},
    /*235*/{ 0x00000078, 0xD7E36876},
    /*236*/{ 0x00000078, 0xD7E36876},
    /*237*/{ 0x00000078, 0xD7E36876},
    /*238*/{ 0x00000078, 0xD7E36876},
    /*239*/{ 0x00000078, 0xD7E36876},
    /*240*/{ 0x00000078, 0x17830876},
    /*241*/{ 0x00000078, 0x17832876},
    /*242*/{ 0x00000078, 0x17833876},
    /*243*/{ 0x00000078, 0x17834876},
    /*244*/{ 0x00000078, 0x17835876},
    /*245*/{ 0x00000078, 0x17836876},
    /*246*/{ 0x00000078, 0x17A36876},
    /*247*/{ 0x00000078, 0x17C36876},
    /*248*/{ 0x00000078, 0x17E36876},
    /*249*/{ 0x00000078, 0x57E36876},
    /*250*/{ 0x00000078, 0x97E36876},
    /*251*/{ 0x00000078, 0xD7E36876},
    /*252*/{ 0x00000078, 0xD7E36876},
    /*253*/{ 0x00000078, 0xD7E36876},
    /*254*/{ 0x00000078, 0xD7E36876},
    /*255*/{ 0x00000078, 0xD7E36876},
};


#else  // WIFI RX 抗干扰能力更强策略

const u32 STF_END_CNT_THR_VAL = 190;
const u32 STF_DET_CNT_THR_VAL = 12;
const u32 DS_DET_CNT_THR0_VAL = 4;
const u32 STF_DET_THR1_VAL = 190;
const u32 STF_DET_THR2_VAL = 150;
const u32 STF_DET_THR3_VAL = 140;
const u32 DSSS_XCDET_THR1_VAL = 80;
const u32 DSSS_XCDET_THR2_VAL = 100;
const u32 DS_VLD_CNT_THR_VAL = 80;
const u32 DS_DET_CNT_THR1_VAL = 4;

///=======================================================//
///       ram adr        ctl
///      0  - 127        feq    index 0 - 127
///      128 - 223       agc    index 0 - 95
///      224 - 225       apc    index 0 - 31
///=======================================================//
const unsigned long wl_rfd_ram_lut [256][2] = {
    //{  H   ,   L }
/// =================================================
///               FEQ
///
///
///==================================================
    /* 0 */ { 0x06405555	,	0x8A1B01A0 },
    /* 1 */ { 0x06410000	,	0x8A3B01A0 },
    /* 2 */ { 0x0641AAAB	,	0x8A5B01A0 },
    /* 3 */ { 0x06425555	,	0x8A7B01A0 },
    /* 4 */ { 0x06430000	,	0x8A9B01A0 },
    /* 5 */ { 0x0643AAAB	,	0x8ABB01A0 },
    /* 6 */ { 0x06445555	,	0x8ADB01A0 },
    /* 7 */ { 0x06450000	,	0x8AFB01A0 },
    /* 8 */ { 0x0645AAAB	,	0x8B1B01A0 },
    /* 9 */ { 0x06465555	,	0x8B3B01A0 },
    /*10 */ { 0x06470000	,	0x8B5B01A0 },
    /*11 */ { 0x0647AAAB	,	0x8B7B01A0 },
    /*12 */ { 0x06485555	,	0x8B9B01A0 },
    /*13 */ { 0x06490000	,	0x8BBB01A0 },
    /*14 */ { 0x0649AAAB	,	0x8BDB01A0 },
    /*15 */ { 0x064A5555	,	0x8BFB01A0 },
    /*16 */ { 0x064B0000	,	0x8C1B01A0 },
    /*17 */ { 0x064BAAAB	,	0x8C3B01A0 },
    /*18 */ { 0x064C5555	,	0x8C5B01A0 },
    /*19 */ { 0x064D0000	,	0x8C7B01A0 },
    /*20 */ { 0x064DAAAB	,	0x8C9B01A0 },
    /*21 */ { 0x064E5555	,	0x8CBB01A0 },
    /*22 */ { 0x064F0000	,	0x8CDB01A0 },
    /*23 */ { 0x064FAAAB	,	0x8CFB01A0 },
    /*24 */ { 0x06505555	,	0x8D1B01A0 },
    /*25 */ { 0x06510000	,	0x8D3B01A0 },
    /*26 */ { 0x0651AAAB	,	0x8D5B01A0 },
    /*27 */ { 0x06525555	,	0x8D7B01A0 },
    /*28 */ { 0x06530000	,	0x8D9B01A0 },
    /*29 */ { 0x0653AAAB	,	0x8DBB01A0 },
    /*30 */ { 0x06545555	,	0x8DDB01A0 },
    /*31 */ { 0x06550000	,	0x8DFB01A0 },
    /*32 */ { 0x0655AAAB	,	0x8E1B01A0 },
    /*33 */ { 0x06565555	,	0x8E3B01A0 },
    /*34 */ { 0x06570000	,	0x8E5B01A0 },
    /*35 */ { 0x0657AAAB	,	0x8E7B01A0 },
    /*36 */ { 0x06585555	,	0x8E9B01A0 },
    /*37 */ { 0x06590000	,	0x8EBB01A0 },
    /*38 */ { 0x0659AAAB	,	0x8EDB01A0 },
    /*39 */ { 0x065A5555	,	0x8EFB01A0 },
    /*40 */ { 0x065B0000	,	0x8F1B01A0 },
    /*41 */ { 0x065BAAAB	,	0x8F3B01A0 },
    /*42 */ { 0x065C5555	,	0x8F5B01A0 },
    /*43 */ { 0x065D0000	,	0x8F7B01A0 },
    /*44 */ { 0x065DAAAB	,	0x8F9B01A0 },
    /*45 */ { 0x065E5555	,	0x8FBB01A0 },
    /*46 */ { 0x065F0000	,	0x8FDB01A0 },
    /*47 */ { 0x065FAAAB	,	0x8FFB01A0 },
    /*48 */ { 0x06605555	,	0x901B01A0 },
    /*49 */ { 0x06610000	,	0x903B01A0 },
    /*50 */ { 0x0661AAAB	,	0x905B01A0 },
    /*51 */ { 0x06625555	,	0x907B01A0 },
    /*52 */ { 0x06630000	,	0x909B01A0 },
    /*53 */ { 0x0663AAAB	,	0x90BB01A0 },
    /*54 */ { 0x06645555	,	0x90DB01A0 },
    /*55 */ { 0x06650000	,	0x90FB01A0 },
    /*56 */ { 0x0665AAAB	,	0x911B01A0 },
    /*57 */ { 0x06665555	,	0x913B01A0 },
    /*58 */ { 0x06670000	,	0x915B01A0 },
    /*59 */ { 0x0667AAAB	,	0x917B01A0 },
    /*60 */ { 0x06685555	,	0x919B01A0 },
    /*61 */ { 0x06690000	,	0x91BB01A0 },
    /*62 */ { 0x0669AAAB	,	0x91DB01A0 },
    /*63 */ { 0x066A5555	,	0x91FB01A0 },
    /*64 */ { 0x066B0000	,	0x921B01A0 },
    /*65 */ { 0x066BAAAB	,	0x923B01A0 },
    /*66 */ { 0x066C5555	,	0x925B01A0 },
    /*67 */ { 0x066D0000	,	0x927B01A0 },
    /*68 */ { 0x066DAAAB	,	0x929B01A0 },
    /*69 */ { 0x066E5555	,	0x92BB01A0 },
    /*70 */ { 0x066F0000	,	0x92DB01A0 },
    /*71 */ { 0x066FAAAB	,	0x92FB01A0 },
    /*72 */ { 0x06705555	,	0x931B01A0 },
    /*73 */ { 0x06710000	,	0x933B01A0 },
    /*74 */ { 0x0671AAAB	,	0x935B01A0 },
    /*75 */ { 0x06725555	,	0x937B01A0 },
    /*76 */ { 0x06730000	,	0x939B01A0 },
    /*77 */ { 0x0673AAAB	,	0x93BB01A0 },
    /*78 */ { 0x06745555	,	0x93DB01A0 },
    /*79 */ { 0x06750000	,	0x93FB01A0 },
    /*80 */ { 0x0675AAAB	,	0x941B01A0 },
    /*81 */ { 0x06765555	,	0x943B01A0 },
    /*82 */ { 0x06770000	,	0x945B01A0 },
    /*83 */ { 0x0677AAAB	,	0x947B01A0 },
    /*84 */ { 0x06785555	,	0x949B01A0 },
    /*85 */ { 0x06790000	,	0x94BB01A0 },
    /*86 */ { 0x06790000	,	0x94BB01A0 },
    /*87 */ { 0x06790000	,	0x94BB01A0 },
    /*88 */ { 0x06790000	,	0x94BB01A0 },
    /*89 */ { 0x06790000	,	0x94BB01A0 },
    /*90 */ { 0x06790000	,	0x94BB01A0 },
    /*91 */ { 0x06790000	,	0x94BB01A0 },
    /*92 */ { 0x06790000	,	0x94BB01A0 },
    /*93 */ { 0x06790000	,	0x94BB01A0 },
    /*94 */ { 0x06790000	,	0x94BB01A0 },
    /*95 */ { 0x06790000	,	0x94BB01A0 },
    /*96 */ { 0x06790000	,	0x94BB01A0 },
    /*97 */ { 0x06790000	,	0x94BB01A0 },
    /*98 */ { 0x06790000	,	0x94BB01A0 },
    /*99 */ { 0x06790000	,	0x94BB01A0 },
    /*100 */{ 0x06415555	,	0x8A3B01A0 },
    /*101 */{ 0x0644AAAB	,	0x8ADB01A0 },
    /*102 */{ 0x06480000	,	0x8B7B01A0 },
    /*103 */{ 0x064B5555	,	0x8C1B01A0 },
    /*104 */{ 0x064EAAAB	,	0x8CBB01A0 },
    /*105 */{ 0x06520000	,	0x8D5B01A0 },
    /*106 */{ 0x06555555	,	0x8DFB01A0 },
    /*107 */{ 0x0658AAAB	,	0x8E9B01A0 },
    /*108 */{ 0x065C0000	,	0x8F3B01A0 },
    /*109 */{ 0x065F5555	,	0x8FDB01A0 },
    /*110 */{ 0x0662AAAB	,	0x907B01A0 },
    /*111 */{ 0x06660000	,	0x911B01A0 },
    /*112 */{ 0x06695555	,	0x91BB01A0 },
    /*113 */{ 0x066CAAAB	,	0x925B01A0 },
    /*114 */{ 0x06700000	,	0x92FB01A0 },
    /*115 */{ 0x06735555	,	0x939B01A0 },
    /*116 */{ 0x0676AAAB	,	0x943B01A0 },
    /*117 */{ 0x067A0000	,	0x94DB01A0 },
    /*118 */{ 0x067A0000	,	0x94DB01A0 },
    /*119 */{ 0x067A0000	,	0x94DB01A0 },
    /*120 */{ 0x067A0000	,	0x94DB01A0 },
    /*121 */{ 0x067A0000	,	0x94DB01A0 },
    /*122 */{ 0x067A0000	,	0x94DB01A0 },
    /*123 */{ 0x067A0000	,	0x94DB01A0 },
    /*124 */{ 0x067A0000	,	0x94DB01A0 },
    /*125 */{ 0x067A0000	,	0x94DB01A0 },
    /*126 */{ 0x067A0000	,	0x94DB01A0 },
    /*127 */{ 0x067A0000	,	0x94DB01A0 },
/// =================================================
///              AGC
/// [32]      [31:29]  [28:26]  [25:23]   [22:20] [19:18]  [17:15]    [14:11]   [10:4]   [3:2]        [1:0]
/// TRX_RFSW TRX_RFCAP VG1_5-0 VG0_12/6/0  HPF_GS RXF3_12/6RXF_12/6/0 MIX_TIA_RT MIX_TIA_CT MIX_GM_GS LNA_GS
///==================================================
    /*128*/{0x0000007A	, 0x008087F0	},
    /*129*/{0x0000007A	, 0x048087F0	},
    /*130*/{0x0000007A	, 0x088087F0	},
    /*131*/{0x0000007A	, 0x0C8087F0	},
    /*132*/{0x0000007A	, 0x108087F0	},
    /*133*/{0x0000007A	, 0x008087F4	},
    /*134*/{0x0000007A	, 0x048087F4	},
    /*135*/{0x0000007A	, 0x088087F4	},
    /*136*/{0x0000000A	, 0x008087F4	},
    /*137*/{0x0000000A	, 0x048087F4	},
    /*138*/{0x0000000A	, 0x088087F4	},
    /*139*/{0x0000000A	, 0x0C8087F4	},
    /*140*/{0x0000000A	, 0x108087F4	},
    /*141*/{0x0000000A	, 0x148087F4	},
    /*142*/{0x0000000A	, 0x008087F5	},
    /*143*/{0x0000000A	, 0x048087F5	},
    /*144*/{0x0000000A	, 0x088087F5	},
    /*145*/{0x0000000A	, 0x0C8087F5	},
    /*146*/{0x0000000A	, 0x108087F5	},
    /*147*/{0x0000000A	, 0x008087F6	},
    /*148*/{0x0000000A	, 0x048087F6	},
    /*149*/{0x0000000A	, 0x088087F6	},
    /*150*/{0x0000000A	, 0x0C8087F6	},
    /*151*/{0x0000000A	, 0x108087F6	},
    /*152*/{0x0000000A	, 0x148087F6	},
    /*153*/{0x0000000A	, 0x011087F6	},
    /*154*/{0x0000000A	, 0x051087F6	},
    /*155*/{0x0000000A	, 0x091087F6	},
    /*156*/{0x0000000A	, 0x0D1087F6	},
    /*157*/{0x0000000A	, 0x111087F6	},
    /*158*/{0x0000000A	, 0x151087F6	},
    /*159*/{0x0000000A	, 0x012487F6	},
    /*160*/{0x0000000A	, 0x052487F6	},
    /*161*/{0x0000000A	, 0x092487F6	},
    /*162*/{0x0000000A	, 0x0D2487F6	},
    /*163*/{0x0000000A	, 0x008087F7	},
    /*164*/{0x0000000A	, 0x048087F7	},
    /*165*/{0x0000000A	, 0x088087F7	},
    /*166*/{0x0000000A	, 0x0C8087F7	},
    /*167*/{0x0000000A	, 0x108087F7	},
    /*168*/{0x0000000A	, 0x148087F7	},
    /*169*/{0x0000000A	, 0x011087F7	},
    /*170*/{0x0000000A	, 0x051087F7	},
    /*171*/{0x0000000A	, 0x091087F7	},
    /*172*/{0x0000000A	, 0x0D1087F7	},
    /*173*/{0x0000000A	, 0x111087F7	},
    /*174*/{0x0000000A	, 0x151087F7	},
    /*175*/{0x0000000A	, 0x012487F7	},
    /*176*/{0x0000000A	, 0x052487F7	},
    /*177*/{0x0000000A	, 0x092487F7	},
    /*178*/{0x0000000A	, 0x0D2487F7	},
    /*179*/{0x0000000A	, 0x112487F7	},
    /*180*/{0x0000000A	,	0x0D248F17	},
    /*181*/{0x0000000A	,	0x11248F17	},
    /*182*/{0x0000000A	,	0x15248F17	},
    /*183*/{0x0000000A	,	0x01350F17	},
    /*184*/{0x0000000A	,	0x05350F17	},
    /*185*/{0x0000000A	,	0x09350F17	},
    /*186*/{0x0000000A	,	0x15249C57	},
    /*187*/{0x0000000A	,	0x01351C57	},
    /*188*/{0x0000000A	,	0x05351C57	},
    /*189*/{0x0000000A	,	0x09351C57	},
    /*190*/{0x0000000A	,	0x1524AAB7	},
    /*191*/{0x0000000A	,	0x01352AB7	},
    /*192*/{0x0000000A	,	0x05352AB7	},
    /*193*/{0x0000000A	,	0x09352AB7	},
    /*194*/{0x0000000A	,	0x0D352AB7	},
    /*195*/{0x0000000A	,	0x11352AB7	},
    /*196*/{0x0000000A	,	0x11353217	},
    /*197*/{0x0000000A	,	0x15353217	},
    /*198*/{0x0000000A	,	0x02453217	},
    /*199*/{0x0000000A	,	0x06453217	},
    /*200*/{0x0000000A	,	0x0A453217	},
    /*201*/{0x0000000A	,	0x0E453217	},
    /*202*/{0x0000000A	,	0x12453217	},
    /*203*/{0x0000000A	,	0x16453217	},
    /*204*/{0x0000000A	,	0x0E453997	},
    /*205*/{0x0000000A	,	0x12453997	},
    /*206*/{0x0000000A	,	0x16453997	},
    /*207*/{0x0000000A	,	0x02593997	},
    /*208*/{0x0000000A	,	0x02594137	},
    /*209*/{0x0000000A	,	0x06594137	},
    /*210*/{0x0000000A	,	0x0A594137	},
    /*211*/{0x0000000A	,	0x0E594137	},
    /*212*/{0x0000000A	,	0x12594137	},
    /*213*/{0x0000000A	,	0x0E594137	},
    /*214*/{0x0000000A	, 0x125948F7	},
    /*215*/{0x0000000A	, 0x165948F7	},
    /*216*/{0x0000000A	, 0x026A48F7	},
    /*217*/{0x0000000A	, 0x066A48F7	},
    /*218*/{0x0000000A	, 0x0A6A48F7	},
    /*219*/{0x0000000A	, 0x0E6A48F7	},
    /*220*/{0x0000000A	, 0x126A48F7	},
    /*221*/{0x0000000A	, 0x166A48F7	},
    /*222*/{0x0000000A	, 0x166A48F7	},
    /*223*/{0x0000000A	, 0x166A48F7	},
/// =================================================
///               APC
///
///
///==================================================

    /*224*/{ 0x00000078, 0x17836876},
    /*225*/{ 0x00000078, 0x17A36876},
    /*226*/{ 0x00000078, 0x17C36876},
    /*227*/{ 0x00000078, 0x17E36876},
    /*228*/{ 0x00000078, 0x57E36876},
    /*229*/{ 0x00000078, 0x97E36876},
    /*230*/{ 0x00000078, 0xD7E36876},
    /*231*/{ 0x00000078, 0xD7E36876},
    /*232*/{ 0x00000078, 0xD7E36876},
    /*233*/{ 0x00000078, 0xD7E36876},
    /*234*/{ 0x00000078, 0xD7E36876},
    /*235*/{ 0x00000078, 0xD7E36876},
    /*236*/{ 0x00000078, 0xD7E36876},
    /*237*/{ 0x00000078, 0xD7E36876},
    /*238*/{ 0x00000078, 0xD7E36876},
    /*239*/{ 0x00000078, 0xD7E36876},
    /*240*/{ 0x00000078, 0x17830876},
    /*241*/{ 0x00000078, 0x17832876},
    /*242*/{ 0x00000078, 0x17833876},
    /*243*/{ 0x00000078, 0x17834876},
    /*244*/{ 0x00000078, 0x17835876},
    /*245*/{ 0x00000078, 0x17836876},
    /*246*/{ 0x00000078, 0x17A36876},
    /*247*/{ 0x00000078, 0x17C36876},
    /*248*/{ 0x00000078, 0x17E36876},
    /*249*/{ 0x00000078, 0x57E36876},
    /*250*/{ 0x00000078, 0x97E36876},
    /*251*/{ 0x00000078, 0xD7E36876},
    /*252*/{ 0x00000078, 0xD7E36876},
    /*253*/{ 0x00000078, 0xD7E36876},
    /*254*/{ 0x00000078, 0xD7E36876},
    /*255*/{ 0x00000078, 0xD7E36876},
};

#endif


