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
*                                           NETWORK DEFINES
*
* Filename : net_def.h
* Version  : V3.06.00
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*********************************************************************************************************
*                                               MODULE
*********************************************************************************************************
*********************************************************************************************************
*/

#ifndef  NET_DEF_MODULE_PRESENT
#define  NET_DEF_MODULE_PRESENT


/*
*********************************************************************************************************
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*********************************************************************************************************
*/

#include  "lib_def.h"
// #include  <lib_def.h>


/*
*********************************************************************************************************
*********************************************************************************************************
*                                           NETWORK DEFINES
*********************************************************************************************************
*********************************************************************************************************
*/



#define  NET_FLAG_NONE                                   DEF_BIT_NONE

#define  NET_TS_NONE                                       0u



#define  NET_TIMEOUT_MIN_SEC                          DEF_INT_32U_MIN_VAL
#define  NET_TIMEOUT_MIN_mS                           DEF_INT_32U_MIN_VAL
#define  NET_TIMEOUT_MIN_uS                           DEF_INT_32U_MIN_VAL

#define  NET_TIMEOUT_MAX_SEC                          DEF_INT_32U_MAX_VAL
#define  NET_TIMEOUT_MAX_mS                           DEF_INT_32U_MAX_VAL
#define  NET_TIMEOUT_MAX_uS                           DEF_INT_32U_MAX_VAL

/*
*********************************************************************************************************
*                                    NETWORK PROTOCOL TYPE DEFINES
*
* Note(s) : (1) See 'net.h  Note #2'.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*********************************************************************************************************
*                                NETWORK MODULE & LAYER GLOBAL DEFINES
*********************************************************************************************************
*********************************************************************************************************
*
* Note(s) : (1) These module & layer global #define's are required PRIOR to network configuration.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                   NETWORK INTERFACE LAYER DEFINES
*
* Note(s) : (1) See specific 'net_if_&&&.h  NETWORK INTERFACE HEADER DEFINES'.
*********************************************************************************************************
*/

#define  NET_IF_NBR_IF_RESERVED               1
#define  NET_IF_NBR_IF_TOT                 (NET_IF_CFG_MAX_NBR_IF + NET_IF_NBR_IF_RESERVED)


#define  NET_IF_NONE_HW_ADD_LEN                            0
#define  NET_IF_802x_HW_ADDR_LEN                           6

/* See Note #1.                                         */
#define  NET_IF_HDR_SIZE_LOOPBACK                         NET_IF_NONE_HW_ADD_LEN
#define  NET_IF_HDR_SIZE_ETHER                            14


#define  NET_IF_HW_ADDR_LEN_MAX                           NET_IF_802x_HW_ADDR_LEN


#define  NET_IF_LOOPBACK_SIZE_MIN                          0


/*
*********************************************************************************************************
*                                       CACHE MGR LAYER DEFINES
*
* Note(s) : (1) See 'net_cache.h  ARP HARDWARE & PROTOCOL DEFINES  Note #1'.
*********************************************************************************************************
*/

#define  NET_CACHE_HW_ADDR_LEN_ETHER                       6



/*
*********************************************************************************************************
*                                          ARP LAYER DEFINES
*
* Note(s) : (1) See 'net_arp.h  ARP HARDWARE & PROTOCOL DEFINES  Note #1'.
*********************************************************************************************************
*/

#define  NET_ARP_HDR_SIZE_MIN                             12
#define  NET_ARP_HDR_SIZE_MAX                             68
#define  NET_ARP_HDR_SIZE_BASE                             8
/* See Note #1a.                                        */
#define  NET_ARP_HDR_SIZE                               (NET_ARP_HDR_SIZE_BASE        + \
                                                        (NET_IF_HW_ADDR_LEN_MAX  * 2) + \
                                                        (NET_IPv4_ADDR_SIZE      * 2))


/*
*********************************************************************************************************
*                                          IP LAYER DEFINES
*
* Note(s) : (1) See 'net_ip.h  IP MULTICAST SELECT DEFINES  Note #1'.
*********************************************************************************************************
*/
/* ------------------ IP VERSION SEL ------------------ */
/* See Note #1.                                         */
#define  NET_IP_VER_SEL_IPv4                               0u
#define  NET_IP_VER_SEL_IPv6                               1u
#define  NET_IP_VER_SEL_IPv4_IPv6                          2u


/* ----------------- IP MULTICAST SEL ----------------- */
/* See Note #2.                                         */
#define  NET_IGMP_MCAST_SEL_NONE                         0u
#define  NET_IGMP_MCAST_SEL_TX                           1u
#define  NET_IGMP_MCAST_SEL_TX_RX                        2u



#define  NET_IP_HW_ADDR_LEN                                NET_IF_HW_ADDR_LEN_MAX


/*
*********************************************************************************************************
*                                         IPv4 LAYER DEFINES
*
* Note(s) : (1) See 'net_ipv4.h  IPv4 MULTICAST SELECT DEFINES  Note #1'.
*********************************************************************************************************
*/

#define  NET_IPv4_HDR_LEN_MIN                              5
#define  NET_IPv4_HDR_LEN_MAX                             15
#define  NET_IPv4_HDR_LEN_WORD_SIZE                      CPU_WORD_SIZE_32

#define  NET_IPv4_HDR_SIZE_MIN                          (NET_IPv4_HDR_LEN_MIN * NET_IPv4_HDR_LEN_WORD_SIZE)

#define  NET_IPv4_HDR_SIZE_MAX                            60    /* See 'net_ipv4.h  IPv4 HEADER DEFINES  Note #1'.      */


/*
*********************************************************************************************************
*                                         IPv6 LAYER DEFINES
*
* Note(s) : (1) See 'net_ipv6.h  IPv6 MULTICAST SELECT DEFINES  Note #1'.
*********************************************************************************************************
*/

#define  NET_IPv6_HDR_SIZE_MAX                            40    /* See 'net_ipv6.h  IPv6 HEADER DEFINES  Note #1'.      */

#define  NET_IPv6_CFG_MLDP_VER_V1                         1u
#define  NET_IPv6_CFG_MLDP_VER_V2                         2u


#define  NET_IPv6_HDR_SIZE                               40u


/*
*********************************************************************************************************
*                                       TRANSPORT LAYER DEFINES
*
* Note(s) : (1) The following transport layer values are pre-#define'd in 'net_type.h' (see 'net_type.h
*               NETWORK TRANSPORT LAYER PORT NUMBER DATA TYPE  Note #1') :
*
*               (a) NET_PORT_NBR_MAX                     DEF_INT_16U_MAX_VAL
*********************************************************************************************************
*/

/* ------------------ NET PORT NBRS ------------------- */
#define  NET_PORT_NBR_RESERVED                             0u
#define  NET_PORT_NBR_NONE                               NET_PORT_NBR_RESERVED

#define  NET_PORT_NBR_MIN                                  1u
#if 0                                                           /* See Note #1a.                                        */
#define  NET_PORT_NBR_MAX                                DEF_INT_16U_MAX_VAL
#endif


/*
*********************************************************************************************************
*                                          UDP LAYER DEFINES
*
* Note(s) : (1) See 'net_udp.h  UDP APPLICATION-PROTOCOL-INTERFACE SELECT DEFINES  Note #1'.
*********************************************************************************************************
*/

#define  NET_UDP_HDR_SIZE_MIN                              8    /* See 'net_udp.h  UDP HEADER DEFINES  Note #1a'.       */
#define  NET_UDP_HDR_SIZE_MAX                              8    /* See 'net_udp.h  UDP HEADER DEFINES  Note #1b'.       */

#define  NET_UDP_HDR_SIZE                                  8

/* ----------------- UDP/APP API SEL ------------------ */
/* See Note #1.                                         */
#define  NET_UDP_APP_API_SEL_NONE                          0u
#define  NET_UDP_APP_API_SEL_SOCK                          1u
#define  NET_UDP_APP_API_SEL_APP                           2u
#define  NET_UDP_APP_API_SEL_SOCK_APP                      3u


/*
*********************************************************************************************************
*                                          IGMP LAYER DEFINES
*********************************************************************************************************
*/

#define  NET_IGMP_HDR_SIZE                                 8
#define  NET_IGMP_MSG_SIZE_MIN             NET_IGMP_HDR_SIZE    /* See Note #1.                                         */



/*
*********************************************************************************************************
*                                          TCP LAYER DEFINES
*********************************************************************************************************
*/

#define  NET_TCP_HDR_LEN_MIN                               5u
#define  NET_TCP_HDR_LEN_MAX                              15
#define  NET_TCP_HDR_LEN_WORD_SIZE                       CPU_WORD_SIZE_32

#define  NET_TCP_HDR_SIZE_MIN                           (NET_TCP_HDR_LEN_MIN * NET_TCP_HDR_LEN_WORD_SIZE)
#define  NET_TCP_HDR_SIZE_MAX                             60    /* See 'net_tcp.h  TCP HEADER DEFINES  Note #1'.        */


/*
*********************************************************************************************************
*                                BSD 4.x & NETWORK SOCKET LAYER DEFINES
*
* Note(s) : (1) (a) See 'net_sock.h  NETWORK SOCKET FAMILY & PROTOCOL DEFINES  Note #1' &
*                       'net_sock.h  NETWORK SOCKET ADDRESS DEFINES'.
*
*               (b) See 'net_sock.h  NETWORK SOCKET BLOCKING MODE SELECT DEFINES  Note #1'.
*
*           (2) Ideally, AF_&&& constants SHOULD be defined as unsigned constants since AF_&&& constants
*               are used with the unsigned socket address family data type (see 'net_bsd.h  BSD 4.x SOCKET
*               DATA TYPES  Note #2a1A').  However, since PF_&&& constants are typically defined to their
*               equivalent AF_&&& constants BUT PF_&&& constants are used with the signed socket protocol
*               family data types; AF_&&& constants are defined as signed constants.
*
*           (3) 'NET_SOCK_PROTOCOL_MAX' abbreviated to 'NET_SOCK_PROTO_MAX' to enforce ANSI-compliance of
*                31-character symbol length uniqueness.
*********************************************************************************************************
*/


#define  NET_SOCK_ADDR_LEN_IP_V4        6u                      /* TCP/IPv6 sock addr len =                             */
/*             2-octet TCP/UDP port val                 */
/*             4-octet IP      addr val                 */


#define  NET_SOCK_ADDR_LEN_IP_V6       26u                      /* TCP/IPv6 sock addr len =                             */
/*             2-octet TCP/UDP port val                 */
/*             4-octet Flow Label   val                 */
/*            16-octet IP      addr val                 */
/*             4-octet Scope ID     val                 */


/* ------------------- TCP/IP SOCKS ------------------- */
/* See Note #2.                                         */
#ifdef   AF_UNSPEC
#undef   AF_UNSPEC
#endif
#define  AF_UNSPEC                                         0

#ifdef   AF_INET
#undef   AF_INET
#endif
#define  AF_INET                                           2

#ifdef   PF_INET
#undef   PF_INET
#endif
#define  PF_INET                                     AF_INET




/* ------------------ TCP/IPv6 SOCKS ------------------ */
#ifdef   AF_INET6
#undef   AF_INET6
#endif
#define  AF_INET6                                          6

#ifdef   PF_INET6
#undef   PF_INET6
#endif
#define  PF_INET6                                    AF_INET6



/* ------------------ SOCK BLOCK SEL ------------------ */
#define  NET_SOCK_BLOCK_SEL_NONE                           0u
#define  NET_SOCK_BLOCK_SEL_DFLT                           1u   /* Sock ops block by dflt (see Note #1b).               */
#define  NET_SOCK_BLOCK_SEL_BLOCK                          2u
#define  NET_SOCK_BLOCK_SEL_NO_BLOCK                       3u



/* -------------------- SOCK TYPES -------------------- */
#ifdef   SOCK_STREAM
#undef   SOCK_STREAM
#endif
#define  SOCK_STREAM                                       1

#ifdef   SOCK_DGRAM
#undef   SOCK_DGRAM
#endif
#define  SOCK_DGRAM                                        2


/* -------------- TCP/IP SOCK PROTOCOLS --------------- */
#ifdef   IPPROTO_TCP
#undef   IPPROTO_TCP
#endif
#define  IPPROTO_TCP                                       6    /*  = NET_IPv4_HDR_PROTOCOL_TCP                         */

#ifdef   IPPROTO_UDP
#undef   IPPROTO_UDP
#endif
#define  IPPROTO_UDP                                      17    /*  = NET_IPv4_HDR_PROTOCOL_UDP                         */

#ifdef   IPPROTO_IP
#undef   IPPROTO_IP
#endif
// #define  IPPROTO_IP                                   0x0800    [>  = NET_ARP_PROTOCOL_TYPE_IP_V4                       <]
/*修改： 与lwip对应*/
#define  IPPROTO_IP                                   0x00    /*  = NET_ARP_PROTOCOL_TYPE_IP_V4                       */


/*
*********************************************************************************************************
*                                 BSD 4.x RETURN CODE / ERROR DEFINES
*
* Note(s) : (1) IEEE Std 1003.1, 2004 Edition states ... :
*
*               (a) ... to "return 0" :
*
*                   (1) "Upon successful completion" of the following BSD socket functions :
*
*                       (A) close()
*                       (B) shutdown()
*                       (C) bind()
*                       (D) connect()
*                       (E) listen()
*
*                   (2) "If no messages are available ... and the peer has performed an orderly shutdown"
*                        for the following BSD socket functions :
*
*                       (A) recvfrom()
*                       (B) recv()
*
*                   (3) Stevens/Fenner/Rudoff, UNIX Network Programming, Volume 1, 3rd Edition, 6th Printing,
*                       Section 6.3, Page 161 states that BSD select() function "returns ... 0 on timeout".
*
*               (b) "Otherwise, -1 shall be returned and 'errno' set to indicate the error" ... for the
*                    following BSD functions :
*
*                   (1) socket()
*                   (2) close()
*                   (3) shutdown()
*                   (4) bind()
*                   (5) listen()
*                   (6) accept()
*                   (7) connect()
*                   (8) recvfrom()
*                   (9) recv()
*                  (10) sendto()
*                  (11) send()
*                  (12) select()
*                  (13) inet_addr()
*                  (14) inet_ntoa()
*
*                  'errno' NOT currently supported (see 'net_bsd.h  Note #1b').
*********************************************************************************************************
*/

#define  NET_BSD_ERR_NONE                                  0                    /* See Note #1a.                        */
#define  NET_BSD_ERR_DFLT                                 -1                    /* See Note #1b.                        */

#define  NET_BSD_RTN_CODE_OK                             NET_BSD_ERR_NONE       /* See Note #1a1.                       */
#define  NET_BSD_RTN_CODE_TIMEOUT                        NET_BSD_RTN_CODE_OK    /* See Note #1a3.                       */
#define  NET_BSD_RTN_CODE_CONN_CLOSED                    NET_BSD_RTN_CODE_OK    /* See Note #1a2.                       */


/*
*********************************************************************************************************
*                                BSD 4.x SOCKET SHUTDOWN TYPE DEFINES
*
* Note(s) : (1) Some socket options NOT currently supported.
*********************************************************************************************************
*/

#ifdef   SHUT_RD
#undef   SHUT_RD
#endif
#define  SHUT_RD                                           0

#ifdef   SHUT_WR
#undef   SHUT_WR
#endif
#define  SHUT_WR                                           1

#ifdef   SHUT_RDWR
#undef   SHUT_RDWR
#endif
#define  SHUT_RDWR                                         2


/*
*********************************************************************************************************
*                                   BSD 4.x SOCKET OPTION DEFINES
*
* Note(s) : (1) Some socket options NOT currently supported.
*********************************************************************************************************
*/
/* ---------------------------------------------------- */
/*                     SOCKET LEVEL                     */
/* ---------------------------------------------------- */
#ifdef   SO_DEBUG
#undef   SO_DEBUG
#endif
#define  SO_DEBUG                                     0x0001    /*  = NET_SOCK_OPT_DEBUG                                */

#ifdef   SO_ACCEPTCONN
#undef   SO_ACCEPTCONN
#endif
#define  SO_ACCEPTCONN                                0x0002    /*  = NET_SOCK_OPT_ACCEPT_CONN                          */

#ifdef   SO_REUSEADDR
#undef   SO_REUSEADDR
#endif
#define  SO_REUSEADDR                                 0x0004    /*  = NET_SOCK_OPT_REUSE_ADDR                           */

#ifdef   SO_KEEPALIVE
#undef   SO_KEEPALIVE
#endif
#define  SO_KEEPALIVE                                 0x0008    /*  = NET_SOCK_OPT_KEEP_ALIVE                           */

#ifdef   SO_DONTROUTE
#undef   SO_DONTROUTE
#endif
#define  SO_DONTROUTE                                 0x0010    /*  = NET_SOCK_OPT_DONT_ROUTE                           */

#ifdef   SO_REUSEPORT
#undef   SO_REUSEPORT
#endif
#define  SO_REUSEPORT                                 0x000F

#ifdef   SO_BROADCAST
#undef   SO_BROADCAST
#endif
#define  SO_BROADCAST                                 0x0020    /*  = NET_SOCK_OPT_BROADCAST                            */

#ifdef   SO_USELOOPBACK
#undef   SO_USELOOPBACK
#endif
#define  SO_USELOOPBACK                               0x0040    /*  = NET_SOCK_OPT_USE_LOOPBACK                         */

#ifdef   SO_LINGER
#undef   SO_LINGER
#endif
#define  SO_LINGER                                    0x0080    /*  = NET_SOCK_OPT_LINGER                               */

#ifdef   SO_OOBINLINE
#undef   SO_OOBINLINE
#endif
#define  SO_OOBINLINE                                 0x0100    /*  = NET_SOCK_OPT_OOB_INLINE                           */

#ifdef   SO_SNDBUF
#undef   SO_SNDBUF
#endif
#define  SO_SNDBUF                                    0x1001    /*  = NET_SOCK_OPT_SOCK_TX_BUF_SIZE                     */

#ifdef   SO_RCVBUF
#undef   SO_RCVBUF
#endif
#define  SO_RCVBUF                                    0x1002    /*  = NET_SOCK_OPT_SOCK_RX_BUF_SIZE                     */

#ifdef   SO_SNDLOWAT
#undef   SO_SNDLOWAT
#endif
#define  SO_SNDLOWAT                                  0x1003

#ifdef   SO_RCVLOWAT
#undef   SO_RCVLOWAT
#endif
#define  SO_RCVLOWAT                                  0x1004

#ifdef   SO_SNDTIMEO
#undef   SO_SNDTIMEO
#endif
#define  SO_SNDTIMEO                                  0x1005    /*  = NET_SOCK_OPT_SOCK_TX_TIMEOUT                      */

#ifdef   SO_RCVTIMEO
#undef   SO_RCVTIMEO
#endif
#define  SO_RCVTIMEO                                  0x1006    /*  = NET_SOCK_OPT_SOCK_RX_TIMEOUT                      */

#ifdef   SO_ERROR
#undef   SO_ERROR
#endif
#define  SO_ERROR                                     0x1007    /*  = NET_SOCK_OPT_SOCK_ERROR                           */

#ifdef   SO_TYPE
#undef   SO_TYPE
#endif
#define  SO_TYPE                                      0x1008    /*  = NET_SOCK_OPT_SOCK_TYPE                            */

#ifdef   SOL_SOCKET
#undef   SOL_SOCKET
#endif
// #define  SOL_SOCKET                                   0x7FFF
/*修改： 与lwip对应*/
#define  SOL_SOCKET                                   0xFFF

/* ---------------------------------------------------- */
/*                      TCP LEVEL                       */
/* ---------------------------------------------------- */
#ifdef   TCP_NODELAY
#undef   TCP_NODELAY
#endif
#define  TCP_NODELAY                                  0x1009    /*  = NET_SOCK_OPT_TCP_NO_DELAY                         */

#ifdef   TCP_KEEPIDLE
#undef   TCP_KEEPIDLE
#endif
#define  TCP_KEEPIDLE                                 0x100A    /*  = NET_SOCK_OPT_TCP_KEEP_IDLE                        */

#ifdef   TCP_KEEPINTVL
#undef   TCP_KEEPINTVL
#endif
#define  TCP_KEEPINTVL                                0x100B    /*  = NET_SOCK_OPT_TCP_KEEP_INTVL                       */

#ifdef   TCP_KEEPCNT
#undef   TCP_KEEPCNT
#endif
#define  TCP_KEEPCNT                                  0x100C    /*  = NET_SOCK_OPT_TCP_KEEP_CNT                         */


/* ---------------------------------------------------- */
/*                       IP LEVEL                       */
/* ---------------------------------------------------- */
#ifdef   IP_TOS
#undef   IP_TOS
#endif
#define  IP_TOS                                           30    /*  = NET_SOCK_OPT_IP_TOS                               */

#ifdef   IP_TTL
#undef   IP_TTL
#endif
#define  IP_TTL                                           31    /*  = NET_SOCK_OPT_IP_TTL                               */

#ifdef   IP_RECVIF
#undef   IP_RECVIF
#endif
#define  IP_RECVIF                                        32    /*  = NET_SOCK_OPT_IP_RX_IF                             */

#ifdef   IP_RECVDSTADDR
#undef   IP_RECVDSTADDR
#endif
#define  IP_RECVDSTADDR                                   33

#ifdef   IP_OPTIONS
#undef   IP_OPTIONS
#endif
#define  IP_OPTIONS                                       34    /*  = NET_SOCK_OPT_IP_OPT                               */

#ifdef   IP_HDRINCL
#undef   IP_HDRINCL
#endif
#define  IP_HDRINCL                                       35    /*  = NET_SOCK_OPT_IP_HDR_INCL                          */

#ifdef   IP_ADD_MEMBERSHIP
#undef   IP_ADD_MEMBERSHIP
#endif
#define  IP_ADD_MEMBERSHIP                                36    /*  = NET_SOCK_OPT_IP_ADD_MEMBERSHIP                    */

#ifdef   IP_DROP_MEMBERSHIP
#undef   IP_DROP_MEMBERSHIP
#endif
#define  IP_DROP_MEMBERSHIP                               37    /*  = NET_SOCK_OPT_IP_DROP_MEMBERSHIP                   */


/*
*********************************************************************************************************
*                                     BSD 4.x SOCKET FLAG DEFINES
*
* Note(s) : (1) Some socket flags NOT currently supported.
*********************************************************************************************************
*/

#ifdef   MSG_OOB
#undef   MSG_OOB
#endif
#define  MSG_OOB                                  DEF_BIT_00    /* See Note #1.                                         */

#ifdef   MSG_PEEK
#undef   MSG_PEEK
#endif
#define  MSG_PEEK                                 DEF_BIT_01

#ifdef   MSG_DONTROUTE
#undef   MSG_DONTROUTE
#endif
#define  MSG_DONTROUTE                            DEF_BIT_02    /* See Note #1.                                         */

#ifdef   MSG_EOR
#undef   MSG_EOR
#endif
#define  MSG_EOR                                  DEF_BIT_03    /* See Note #1.                                         */

#ifdef   MSG_TRUNC
#undef   MSG_TRUNC
#endif
#define  MSG_TRUNC                                DEF_BIT_04

#ifdef   MSG_CTRUNC
#undef   MSG_CTRUNC
#endif
#define  MSG_CTRUNC                               DEF_BIT_05

#ifdef   MSG_WAITALL
#undef   MSG_WAITALL
#endif
#define  MSG_WAITALL                              DEF_BIT_06    /* See Note #1.                                         */

#ifdef   MSG_DONTWAIT
#undef   MSG_DONTWAIT
#endif
#define  MSG_DONTWAIT                             DEF_BIT_07


/*
*********************************************************************************************************
*                                NETWORK CONNECTION MANAGEMENT DEFINES
*********************************************************************************************************
*/

/* ---------------- CONN FAMILY TYPES ----------------- */
#define  NET_CONN_FAMILY_NONE                              0
/* Net sock cfg :                                       */
#define  NET_CONN_FAMILY_IP_V4_SOCK                      NET_SOCK_FAMILY_IP_V4
#define  NET_CONN_ADDR_LEN_IP_V4_SOCK                    NET_SOCK_ADDR_LEN_IP_V4
#define  NET_CONN_PROTOCOL_MAX_IP_V4_SOCK                NET_SOCK_PROTO_MAX_IP_V4

#define  NET_CONN_FAMILY_IP_V6_SOCK                      NET_SOCK_FAMILY_IP_V6
#define  NET_CONN_ADDR_LEN_IP_V6_SOCK                    NET_SOCK_ADDR_LEN_IP_V6
#define  NET_CONN_PROTOCOL_MAX_IP_V6_SOCK                NET_SOCK_PROTO_MAX_IP_V6

#define  NET_CONN_ID_NONE                                 -1





/*
*********************************************************************************************************
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*********************************************************************************************************
*/

#endif  /* NET_DEF_MODULE_PRESENT */
