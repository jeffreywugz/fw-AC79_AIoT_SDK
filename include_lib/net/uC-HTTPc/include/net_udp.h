/*
*********************************************************************************************************
*                                              uC/TCP-IP
*                                      The Embedded TCP/IP Suite
*
*                    Copyright 2004-2020 Silicon Laboratories Inc. www.silabs.com
*
*                                 SPDX-License-Identifier: APACHE-2.0
*
*               This software is subject to an open source license and is distributed by
*                Silicon Laboratories Inc. pursuant to the terms of the Apache License,
*                    Version 2.0 available at www.apache.org/licenses/LICENSE-2.0.
*
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                          NETWORK UDP LAYER
*                                      (USER DATAGRAM PROTOCOL)
*
* Filename : net_udp.h
* Version  : V3.06.00
*********************************************************************************************************
* Note(s)  : (1) Supports User Datagram Protocol as described in RFC #768.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*********************************************************************************************************
*                                               MODULE
*********************************************************************************************************
*********************************************************************************************************
*/

#ifndef  NET_UDP_MODULE_PRESENT
#define  NET_UDP_MODULE_PRESENT


/*
*********************************************************************************************************
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*********************************************************************************************************
*/

#include  "net_cfg_net.h"
#include  "net_def.h"
#include  "net_type.h"
#include  "net_err.h"
#include  <cpu.h>


/*
*********************************************************************************************************
*********************************************************************************************************
*                                               DEFINES
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                          UDP APPLICATION-PROTOCOL-INTERFACE SELECT DEFINES
*
* Note(s) : (1) The following UDP values MUST be pre-#define'd in 'net_def.h' PRIOR to 'net_cfg.h' so
*               that the developer can configure UDP for the desired application receive demultiplex
*               selection (see 'net_def.h  UDP LAYER DEFINES  Note #1') :
*
*                   NET_UDP_APP_API_SEL_SOCK
*                   NET_UDP_APP_API_SEL_APP
*                   NET_UDP_APP_API_SEL_SOCK_APP
*********************************************************************************************************
*/

#if 0                                                           /* See Note #1.                                         */
#define  NET_UDP_APP_API_SEL_NONE                          0u
#define  NET_UDP_APP_API_SEL_SOCK                          1u
#define  NET_UDP_APP_API_SEL_APP                           2u
#define  NET_UDP_APP_API_SEL_SOCK_APP                      3u
#endif


/*
*********************************************************************************************************
*                                         UDP HEADER DEFINES
*
* Note(s) : (1) The following UDP values MUST be pre-#define'd in 'net_def.h' PRIOR to 'net_buf.h' so that
*               the Network Buffer Module can configure minimum/maximum buffer header sizes (see 'net_def.h
*               UDP LAYER DEFINES' & 'net_buf.h  NETWORK BUFFER INDEX & SIZE DEFINES  Note #1a') :
*
*               (a) NET_UDP_HDR_SIZE_MIN                   8
*               (b) NET_UDP_HDR_SIZE_MAX                   8
*********************************************************************************************************
*/


#if 0                                                           /* See Note #1.                                         */
#define  NET_UDP_HDR_SIZE_MIN                            NET_UDP_HDR_SIZE
#define  NET_UDP_HDR_SIZE_MAX                            NET_UDP_HDR_SIZE
#endif


#define  NET_UDP_PSEUDO_HDR_SIZE                          12    /*  = sizeof(NET_UDP_PSEUDO_HDR)                        */


#define  NET_UDP_PORT_NBR_RESERVED                       NET_PORT_NBR_RESERVED
#define  NET_UDP_PORT_NBR_NONE                           NET_UDP_PORT_NBR_RESERVED


#define  NET_UDP_HDR_CHK_SUM_POS_ZERO                 0x0000u
#define  NET_UDP_HDR_CHK_SUM_NEG_ZERO                 0xFFFFu
#define  NET_UDP_HDR_CHK_SUM_NONE                        NET_UDP_HDR_CHK_SUM_POS_ZERO


/*
*********************************************************************************************************
*                                    UDP DATA/TOTAL LENGTH DEFINES
*
* Note(s) : (1) (a) UDP total length #define's (NET_UDP_TOT_LEN)  relate to the total size of a complete
*                   UDP packet, including the packet's UDP header.  Note that a complete UDP packet MAY
*                   be fragmented in multiple Internet Protocol packets.
*
*               (b) UDP data  length #define's (NET_UDP_DATA_LEN) relate to the data  size of a complete
*                   UDP packet, equal to the total UDP packet length minus its UDP header size.  Note
*                   that a complete UDP packet MAY be fragmented in multiple Internet Protocol packets.
*********************************************************************************************************
*/

/* See Notes #1a & #1b.                 */
#define  NET_UDP_DATA_LEN_MIN                              0

#define  NET_UDP_TOT_LEN_MIN                            (NET_UDP_HDR_SIZE      + NET_UDP_DATA_LEN_MIN )
#if    defined(NET_IPv4_MODULE_EN)
#define  NET_UDP_TOT_LEN_MAX                            (NET_IPv4_TOT_LEN_MAX  - NET_IPv4_HDR_SIZE_MIN)
#define  NET_UDP_DATA_LEN_MAX                           (NET_UDP_TOT_LEN_MAX   - NET_UDP_HDR_SIZE     )
#elif  defined(NET_IPv6_MODULE_EN)
#define  NET_UDP_TOT_LEN_MAX                            (NET_IPv6_TOT_LEN_MAX  - NET_IPv6_HDR_SIZE)
#define  NET_UDP_DATA_LEN_MAX                           (NET_UDP_TOT_LEN_MAX   - NET_UDP_HDR_SIZE     )
#endif


/*
*********************************************************************************************************
*                                          UDP FLAG DEFINES
*********************************************************************************************************
*/

/* ------------------ NET UDP FLAGS ------------------- */
#define  NET_UDP_FLAG_NONE                        DEF_BIT_NONE

/* ------------------ UDP TX  FLAGS ------------------- */
#define  NET_UDP_FLAG_TX_CHK_SUM_DIS              DEF_BIT_00    /* DISABLE tx        chk sums.                          */
#define  NET_UDP_FLAG_TX_BLOCK                    DEF_BIT_07

/* ------------------ UDP RX  FLAGS ------------------- */
#define  NET_UDP_FLAG_RX_CHK_SUM_NONE_DISCARD     DEF_BIT_08    /* Discard rx'd NULL chk sum UDP datagrams.             */
#define  NET_UDP_FLAG_RX_DATA_PEEK                DEF_BIT_09
#define  NET_UDP_FLAG_RX_BLOCK                    DEF_BIT_15


/*
*********************************************************************************************************
*********************************************************************************************************
*                                             DATA TYPES
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                      UDP PORT NUMBER DATA TYPE
*********************************************************************************************************
*/

typedef  NET_PORT_NBR  NET_UDP_PORT_NBR;                        /* Defines UDP port nbr size.                           */


/*
*********************************************************************************************************
*                                         UDP FLAGS DATA TYPE
*********************************************************************************************************
*/

typedef  NET_FLAGS  NET_UDP_FLAGS;


/*
*********************************************************************************************************
*                                             UDP HEADER
*
* Note(s) : (1) See RFC #768, Section 'Format' for UDP datagram header format.
*********************************************************************************************************
*/

/* ------------------- NET UDP HDR -------------------- */
typedef  struct  net_udp_hdr {
    NET_UDP_PORT_NBR        PortSrc;                            /* UDP datagram src  port.                              */
    NET_UDP_PORT_NBR        PortDest;                           /* UDP datagram dest port.                              */
    CPU_INT16U              DatagramLen;                        /* UDP datagram msg len.                                */
    NET_CHK_SUM             ChkSum;                             /* UDP datagram chk sum.                                */
} NET_UDP_HDR;


/*
*********************************************************************************************************
*                                          UDP PSEUDO-HEADER
*
* Note(s) : (1) See RFC #768, Section 'Fields : Checksum' for UDP datagram pseudo-header format.
*********************************************************************************************************
*/

/* ---------------- NET UDP PSEUDO-HDR ---------------- */
typedef  struct  net_udp_pseudo_hdr {
    NET_IPv4_ADDR           AddrSrc;                            /* UDP datagram src  addr.                              */
    NET_IPv4_ADDR           AddrDest;                           /* UDP datagram dest addr.                              */
    CPU_INT08U              Zero;                               /* Field MUST be zero'd; i.e. ALL bits clr'd.           */
    CPU_INT08U              Protocol;                           /* UDP datagram protocol.                               */
    CPU_INT16U              DatagramLen;                        /* UDP datagram tot len.                                */
} NET_UDP_PSEUDO_HDR;


/*
*********************************************************************************************************
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                     EXTERNAL C LANGUAGE LINKAGE
*
* Note(s) : (1) C++ compilers MUST 'extern'ally declare ALL C function prototypes & variable/object
*               declarations for correct C language linkage.
*********************************************************************************************************
*/

#ifdef  __cplusplus
extern "C" {
#endif


void        NetUDP_Init(void);


/* --------------------- RX FNCTS --------------------- */
void        NetUDP_Rx(NET_BUF           *pbuf,
                      NET_ERR           *p_err);

CPU_INT16U  NetUDP_RxAppData(NET_BUF           *pbuf,
                             void              *pdata_buf,
                             CPU_INT16U         data_buf_len,
                             NET_UDP_FLAGS      flags,
                             void              *pip_opts_buf,
                             CPU_INT08U         ip_opts_buf_len,
                             CPU_INT08U        *pip_opts_len,
                             NET_ERR           *p_err);


/* --------------------- TX FNCTS --------------------- */
CPU_INT16U  NetUDP_TxAppDataIPv4(void              *p_data,
                                 CPU_INT16U         data_len,
                                 NET_IPv4_ADDR      src_addr,
                                 NET_UDP_PORT_NBR   src_port,
                                 NET_IPv4_ADDR      dest_addr,
                                 NET_UDP_PORT_NBR   dest_port,
                                 NET_IPv4_TOS       TOS,
                                 NET_IPv4_TTL       TTL,
                                 NET_UDP_FLAGS      flags_udp,
                                 NET_IPv4_FLAGS     flags_ip,
                                 void              *popts_ip,
                                 NET_ERR           *p_err);

CPU_INT16U  NetUDP_TxAppDataIPv6(void                    *p_data,
                                 CPU_INT16U               data_len,
                                 NET_IPv6_ADDR           *p_src_addr,
                                 NET_UDP_PORT_NBR         src_port,
                                 NET_IPv6_ADDR           *p_dest_addr,
                                 NET_UDP_PORT_NBR         dest_port,
                                 NET_IPv6_TRAFFIC_CLASS   traffic_class,
                                 NET_IPv6_FLOW_LABEL      flow_label,
                                 NET_IPv6_HOP_LIM         hop_lim,
                                 NET_UDP_FLAGS            flags_udp,
                                 NET_ERR                 *p_err);

CPU_INT16U  NetUDP_TxAppDataHandlerIPv4(void               *p_data,
                                        CPU_INT16U          data_len,
                                        NET_IPv4_ADDR       src_addr,
                                        NET_UDP_PORT_NBR    src_port,
                                        NET_IPv4_ADDR       dest_addr,
                                        NET_UDP_PORT_NBR    dest_port,
                                        NET_IPv4_TOS        TOS,
                                        NET_IPv4_TTL        TTL,
                                        NET_UDP_FLAGS       flags_udp,
                                        NET_IPv4_FLAGS      flags_ip,
                                        void               *popts_ip,
                                        NET_ERR            *p_err);

CPU_INT16U  NetUDP_TxAppDataHandlerIPv6(void                    *p_data,
                                        CPU_INT16U               data_len,
                                        NET_IPv6_ADDR           *p_src_addr,
                                        NET_UDP_PORT_NBR         src_port,
                                        NET_IPv6_ADDR           *p_dest_addr,
                                        NET_UDP_PORT_NBR         dest_port,
                                        NET_IPv6_TRAFFIC_CLASS   traffic_class,
                                        NET_IPv6_FLOW_LABEL      flow_label,
                                        NET_IPv6_HOP_LIM         hop_lim,
                                        NET_UDP_FLAGS            flags_udp,
                                        NET_ERR                 *p_err);

/*
*********************************************************************************************************
*                                   EXTERNAL C LANGUAGE LINKAGE END
*********************************************************************************************************
*/

#ifdef  __cplusplus
}
#endif
/*
*********************************************************************************************************
*********************************************************************************************************
*                                        CONFIGURATION ERRORS
*********************************************************************************************************
*********************************************************************************************************
*/


#ifndef  NET_UDP_CFG_RX_CHK_SUM_DISCARD_EN
#error  "NET_UDP_CFG_RX_CHK_SUM_DISCARD_EN        not #define'd in 'net_cfg.h'"
#error  "                                   [MUST be  DEF_DISABLED]           "
#error  "                                   [     ||  DEF_ENABLED ]           "

#elif  ((NET_UDP_CFG_RX_CHK_SUM_DISCARD_EN != DEF_DISABLED) && \
        (NET_UDP_CFG_RX_CHK_SUM_DISCARD_EN != DEF_ENABLED ))
#error  "NET_UDP_CFG_RX_CHK_SUM_DISCARD_EN  illegally #define'd in 'net_cfg.h'"
#error  "                                   [MUST be  DEF_DISABLED]           "
#error  "                                   [     ||  DEF_ENABLED ]           "
#endif



#ifndef  NET_UDP_CFG_TX_CHK_SUM_EN
#error  "NET_UDP_CFG_TX_CHK_SUM_EN                not #define'd in 'net_cfg.h'"
#error  "                                   [MUST be  DEF_DISABLED]           "
#error  "                                   [     ||  DEF_ENABLED ]           "

#elif  ((NET_UDP_CFG_TX_CHK_SUM_EN != DEF_DISABLED) && \
        (NET_UDP_CFG_TX_CHK_SUM_EN != DEF_ENABLED ))
#error  "NET_UDP_CFG_TX_CHK_SUM_EN          illegally #define'd in 'net_cfg.h'"
#error  "                                   [MUST be  DEF_DISABLED]           "
#error  "                                   [     ||  DEF_ENABLED ]           "
#endif


/*
*********************************************************************************************************
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*********************************************************************************************************
*/

#endif /* NET_UDP_MODULE_PRESENT */
