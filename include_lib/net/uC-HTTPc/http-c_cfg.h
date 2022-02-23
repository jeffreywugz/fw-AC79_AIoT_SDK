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
*                                   HTTP CLIENT CONFIGURATION FILE
*
*                                              TEMPLATE
*
* Filename : http-c_cfg.h
* Version  : V3.01.00
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*********************************************************************************************************
*                                           INCLUDE FILES
*********************************************************************************************************
*********************************************************************************************************
*/

#include  "http.h"
#include  "http-c_type.h"


/*
*********************************************************************************************************
*********************************************************************************************************
*                                               MODULE
*********************************************************************************************************
*********************************************************************************************************
*/

#ifndef  HTTPc_CFG_MODULE_PRESENT
#define  HTTPc_CFG_MODULE_PRESENT


/*
*********************************************************************************************************
*********************************************************************************************************
*                                      COMPILE-TIME CONFIGURATION
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                   HTTP ARGUMENT CHECK CONFIGURATION
*
* Note(s) : (1) Configure HTTPc_CFG_ARG_CHK_EXT_EN to enable/disable the HTTP client external argument
*               check feature :
*
*               (a) When ENABLED,  ALL arguments received from any port interface provided by the developer
*                   are checked/validated.
*
*               (b) When DISABLED, NO  arguments received from any port interface provided by the developer
*                   are checked/validated.
*********************************************************************************************************
*/

#define  HTTPc_CFG_ARG_CHK_EXT_EN                 DEF_ENABLED   /* See Note #1.                                         */


/*
*********************************************************************************************************
*                                    HTTP CLIENT TASK CONFIGURATION
*
* Note(s) : (1) Configure HTTPc_CFG_MODE_ASYNC_TASK_EN to enable/disable HTTP client task.
*               (a) DEF_DISABLED : No HTTP client task will be created to process the HTTP requests.
*                                  The Blocking HTTPc API will be enabled.
*
*               (b) DEF_ENABLED  : An HTTP client task will be created to process all the HTTP requests.
*                                  The Non-Blocking HTTPc API will be enabled. Therefore, multiple
*                                  connections can be handle by the task simultaneously.
*
*           (2) Configure HTTPc_CFG_MODE_BLOCK_EN to enable/disable the blocking option when the
*               asynchronous HTTPc Task is enabled.
*********************************************************************************************************
*/

/*该模块还不能用，todo*/
// #define  HTTPc_CFG_MODE_ASYNC_TASK_EN           DEF_ENABLED     [> See Note #1.                                         <]
#define  HTTPc_CFG_MODE_ASYNC_TASK_EN           DEF_DISABLED     /* See Note #1.                                         */

#define  HTTPc_CFG_MODE_BLOCK_EN                DEF_ENABLED     /* See Note #2.                                         */


/*
*********************************************************************************************************
*                              HTTP CLIENT PERSISTENT CONNECTION CONFIGURATION
*
* Note(s) : (1) Configure HTTPc_CFG_PERSISTENT_EN to enable/disable Persistent Connection support.
*********************************************************************************************************
*/

/*该模块还不能用，todo*/
// #define  HTTPc_CFG_PERSISTENT_EN                DEF_ENABLED     [> See Note #1.                                         <]
#define  HTTPc_CFG_PERSISTENT_EN                DEF_DISABLED     /* See Note #1.                                         */


/*
*********************************************************************************************************
*                                HTTP CLIENT CHUNKED TRANSFER CONFIGURATION
*
* Note(s) : (1) Configure HTTPc_CFG_CHUNK_TX_EN to enable/disable Chunked Transfer support in Transmission.
*
*           (2) Chunked Transfer in Reception is always enabled.
*********************************************************************************************************
*/

#define  HTTPc_CFG_CHUNK_TX_EN                  DEF_ENABLED     /* See Note #1.                                         */


/*
*********************************************************************************************************
*                                 HTTP CLIENT QUERY STRING CONFIGURATION
*
* Note(s) : (1) Configure HTTPc_CFG_QUERY_STR_EN to enable/disable Query String support in URL.
*********************************************************************************************************
*/

#define  HTTPc_CFG_QUERY_STR_EN                 DEF_ENABLED     /* See Note #1.                                         */


/*
*********************************************************************************************************
*                                HTTP CLIENT HEADER FIELD CONFIGURATION
*
* Note(s) : (1) Configure HTTPc_CFG_HDR_RX_EN to enable/disable header field processing in reception
*               (i.e for headers received in the HTTP response.
*
*           (2) Configure HTTPc_CFG_HDR_TX_EN to enable/disable header field processing in transmission
*               (i.e for headers to include in the HTTP request.
*********************************************************************************************************
*/

#define  HTTPc_CFG_HDR_RX_EN                    DEF_ENABLED     /* Configure Header feature in RX (see Note #1):        */


#define  HTTPc_CFG_HDR_TX_EN                    DEF_ENABLED     /* Configure Header feature in TX (see Note #2):        */


/*
*********************************************************************************************************
*                                     HTTP CLIENT FORM CONFIGURATION
*
* Note(s) : (1) Configure HTTPc_CFG_FORM_EN to enable/disable HTTP form creation source code.
*********************************************************************************************************
*/

#define  HTTPc_CFG_FORM_EN                      DEF_ENABLED     /* See Note #1.                                         */


/*
*********************************************************************************************************
*                                   HTTP CLIENT USER DATA CONFIGURATION
*
* Note(s) : (1) Configure HTTPc_CFG_USER_DATA_EN         to enable/disable user data pointer in HTTPc_CONN
*                                                        and HTTPc_REQ structure.
*********************************************************************************************************
*/

#define  HTTPc_CFG_USER_DATA_EN                 DEF_ENABLED     /* See Note #1.                                         */


/*
*********************************************************************************************************
*                                   HTTP CLIENT WEBSOCKET CONFIGURATION
*
* Note(s) : (1) Configure HTTPc_CFG_WEBSOCKET_EN        to enable/disable the Websocket feature.
*********************************************************************************************************
*/

/*该模块还不能用，todo*/
// #define  HTTPc_CFG_WEBSOCKET_EN                 DEF_ENABLED     [> See Note #1.                                         <]
#define  HTTPc_CFG_WEBSOCKET_EN                 DEF_DISABLED     /* See Note #1.                                         */


/*
*********************************************************************************************************
*********************************************************************************************************
*                                      RUN-TIME CONFIGURATION
*********************************************************************************************************
*********************************************************************************************************
*/

extern  const  HTTP_TASK_CFG   HTTPc_TaskCfg;
extern  const  HTTPc_CFG       HTTPc_Cfg;


/* =============================================== END =============================================== */
#endif  /* HTTPc_CFG_MODULE_PRESENT */
