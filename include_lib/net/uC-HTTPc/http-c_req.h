/*
*********************************************************************************************************
*                                               uC/HTTP
*                                     Hypertext Transfer Protocol
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
*                                     HTTP CLIENT REQUEST MODULE
*
* Filename : http-c_req.h
* Version  : V3.01.00
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*********************************************************************************************************
*                                               MODULE
*
* Note(s) : (1) This main network protocol suite header file is protected from multiple pre-processor
*               inclusion through use of the HTTPc module present pre-processor macro definition.
*********************************************************************************************************
*********************************************************************************************************
*/

#ifndef  HTTPc_REQ_MODULE_PRESENT                          /* See Note #1.                                         */
#define  HTTPc_REQ_MODULE_PRESENT


/*
*********************************************************************************************************
*********************************************************************************************************
*                                         HTTPc INCLUDE FILES
*********************************************************************************************************
*********************************************************************************************************
*/

#include  <cpu.h>

#include  "include/lib_def.h"
#include  "include/lib_str.h"
#include  "include/lib_ascii.h"

#include  "include/net.h"
#include  "include/net_cfg_net.h"
#include  "http-c_cfg.h"
#include  "http.h"
#include  "http-c.h"


/*
*********************************************************************************************************
*********************************************************************************************************
*                                                DEFINES
*********************************************************************************************************
*********************************************************************************************************
*/

#define  HTTPc_STR_BOUNDARY                  "rgifovj80325n"

#define  HTTPc_STR_BOUNDARY_LEN              (sizeof(HTTPc_STR_BOUNDARY) -1)

#define  HTTPc_STR_BOUNDARY_START            "--" HTTPc_STR_BOUNDARY

#define  HTTPc_STR_BOUNDARY_END              "--" HTTPc_STR_BOUNDARY "--"

#define  HTTPc_STR_BOUNDARY_START_LEN        (sizeof(HTTPc_STR_BOUNDARY_START) - 1)

#define  HTTPc_STR_BOUNDARY_END_LEN          (sizeof(HTTPc_STR_BOUNDARY_END) - 1)


/*
*********************************************************************************************************
*********************************************************************************************************
*                                          FUNCTION PROTOTYPES
*********************************************************************************************************
*********************************************************************************************************
*/

void          HTTPcReq_Prepare(HTTPc_CONN      *p_conn,
                               HTTPc_ERR       *p_err);

CPU_BOOLEAN   HTTPcReq(HTTPc_CONN      *p_conn,
                       HTTPc_ERR       *p_err);

CPU_CHAR     *HTTPcReq_HdrCopyToBuf(CPU_CHAR        *p_buf,                 // TODO put function in HTTP common files ?
                                    CPU_INT16U       buf_len,
                                    CPU_SIZE_T       buf_len_rem,
                                    HTTP_HDR_FIELD   hdr_type,
                                    const  CPU_CHAR        *p_val,
                                    CPU_SIZE_T       val_len,
                                    CPU_BOOLEAN      add_CRLF,
                                    HTTPc_ERR       *p_err);


/*
*********************************************************************************************************
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*********************************************************************************************************
*/

#endif  /* HTTPc_REQ_MODULE_PRESENT  */
