//----------------------------------------------------------------------------//
/**
 ******************************************************************************
 * @file    ntp.h
 * @author
 * @version
 * @brief   This file provides the api of ntp client.
 ******************************************************************************
 * @attention
 *
 * Copyright(c) 2017, ZhuHai JieLi Technology Co.Ltd. All rights reserved.
 ******************************************************************************
 */

#ifndef NTP_H_
#define NTP_H_


#include <time.h>

/*! \addtogroup NTP
 *  @ingroup NETWORK_LIB
 *  @brief	NTP client api
 *  @{
 */

#define NTP_DBUG_INFO_ON	0		/*!< NTP client debug information on/off */

#define NTP_CLIENT_THREAD_PRIO	22		/*!< The priority of NTP client thread  */

#define NTP_CLIENT_THREAD_STACK_SIZE	2048	/*!< The stack size of NTP client thread */

/*----------------------------------------------------------------------------*/
/**@brief  Set the query interval time of the ntp client
   @param   min: The query interval time
   @note
*/
/*----------------------------------------------------------------------------*/
void ntp_set_query_interval_min(unsigned int min);

/*----------------------------------------------------------------------------*/
/**@brief  Set the time zone
   @param   zone: The time zone
   @note
*/
/*----------------------------------------------------------------------------*/
void ntp_set_timezone(unsigned int zone);

/*----------------------------------------------------------------------------*/
/**@brief  Set the callback function called after get the time from ntp server
   @param  cb: The callback function
   @note
*/
/*----------------------------------------------------------------------------*/
void ntp_set_time_cb(void (*cb)(struct tm *t));

/*----------------------------------------------------------------------------*/
/**@brief  Insert a ntp server host name to the host list
   @param   name: NTP server host name
   @return 0: Success
   @return -1: Memory not enough
   @note
*/
/*----------------------------------------------------------------------------*/
int ntp_add_host_name(char *name);

/*----------------------------------------------------------------------------*/
/**@brief  Remove a ntp server host name to the host list
   @return 0: Success
   @return -1: Not find host name
   @note
*/
/*----------------------------------------------------------------------------*/
int ntp_remove_host_name(char *name);

/*----------------------------------------------------------------------------*/
/**@brief  Initialize ntp client
   @note
*/
/*----------------------------------------------------------------------------*/
void ntp_client_init(void);

/*----------------------------------------------------------------------------*/
/**@brief  Start ntp client
   @return 0: Start ntp client sucessfully
   @return other: Create thread error
   @note
*/
/*----------------------------------------------------------------------------*/
int ntp_client_start(void);

/*----------------------------------------------------------------------------*/
/**@brief  Uninstall ntp client
   @note
*/
/*----------------------------------------------------------------------------*/
void ntp_client_uninit(void);


/*----------------------------------------------------------------------------*/
/**@brief  Get time from ntp server
   @param  host: NTP server host name
   @param  s_tm: Save time information
   @param  recv_to: The value of socket receive timeout(ms)
   @return 0: sucess
   @return -1: fail
   @note	This api is independent, not need to call ntp_client_init and ntp_client_uninit, it only get once
*/
/*----------------------------------------------------------------------------*/
int ntp_client_get_time_once(const char *host, struct tm *s_tm, int recv_to);

/*! @}*/

#endif















