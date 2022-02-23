/* *
 *  $Id: rtsp.h 395 2006-10-05 16:08:10Z shawill $
 *
 *  This file is part of Fenice
 *
 *  Fenice -- Open Media Server
 *
 *  Copyright (C) 2004 by
 *
 *	- Giampaolo Mancini	<giampaolo.mancini@polito.it>
 *	- Francesco Varano	<francesco.varano@polito.it>
 *	- Marco Penno		<marco.penno@polito.it>
 *	- Federico Ridolfo	<federico.ridolfo@polito.it>
 *	- Eugenio Menegatti 	<m.eu@libero.it>
 *	- Stefano Cau
 *	- Giuliano Emma
 *	- Stefano Oldrini
 *
 *  Fenice is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  Fenice is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Fenice; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * */

#ifndef _RTSPH
#define _RTSPH


#include "../fenice_config.h"
#include "utils.h"
#include "socket.h"
#include "rtp.h"
#include "rtcp.h"
#include "mediainfo.h"
#include "schedule.h"

#define RTSP_BUFFERSIZE 1472

/* Stati della macchina a stati del server rtsp */
#define INIT_STATE      0
#define READY_STATE     1
#define PLAY_STATE      2

#define RTSP_VER "RTSP/1.0"

#define RTSP_EL "\r\n"
#define RTSP_RTP_AVP "RTP/AVP"

typedef enum _RTSP_proto {
    TCP,
    SCTP
} RTSP_proto;

typedef struct _RTSP_interleaved {
    int rtp_fd; //!< output rtp local socket
    int rtcp_fd; //!< output rtcp local socket
    union {
        struct {
            uint16 rtp_ch;
            uint16 rtcp_ch;
        } tcp;
        struct {
            uint16 rtp_st;
            uint16 rtcp_st;
        } sctp;
    } proto;
    struct _RTSP_interleaved *next;
} RTSP_interleaved;

typedef struct _RTSP_session {
    //tsocket fd;
    int cur_state;
    int session_id;
    RTP_session *rtp_session;
    struct _RTSP_session *next;
} RTSP_session;

typedef struct _RTSP_buffer {
    tsocket fd;
    struct sockaddr_in dest_addr;
    RTSP_proto proto;
    unsigned int port;
    // Buffers
    char in_buffer[RTSP_BUFFERSIZE];
    size_t in_size;
    char out_buffer[RTSP_BUFFERSIZE + MAX_DESCR_LENGTH];
    size_t out_size;
    size_t out_sent;
    /* vars for interleaved data:
     * interleaved pkts will be present only at the beginning of out_buffer.
     * this size is used to remenber how much data should be grouped in one
     * pkt with  MSG_MORE flag.
     * */
    RTSP_interleaved *interleaved;
    // Run-Time
    unsigned int rtsp_cseq;
    char descr[MAX_DESCR_LENGTH];	// La descrizione
    RTSP_session *session_list;
    struct _RTSP_buffer *next;
} RTSP_buffer;

// Interfacce

void RTSP_state_machine(RTSP_buffer *rtsp, int method_code);
//Commuta di stato sulla base di rtsp->current_state e gestisce lo stato stesso.

int RTSP_describe(RTSP_buffer *rtsp);
//Non alloca nessuna risorsa. Risponde al client con la descrizione SDP della URL.

int RTSP_setup(RTSP_buffer *rtsp, RTSP_session **new_session);
//Alloca le risorse di uno stream.

int RTSP_play(RTSP_buffer *rtsp);
// Riproduce uno stream sulla base dei parametri indicati.

int RTSP_pause(RTSP_buffer *rtsp);
//Interrompe momentaneamente uno stream.

int RTSP_teardown(RTSP_buffer *rtsp);
// Termina uno stream

int RTSP_options(RTSP_buffer *rtsp);

int RTSP_handler(RTSP_buffer *rtsp);
// RTSP input buffer parser routine. Return -1 on error.

// RTSP parsing helper functions
#define RTSP_not_full 0
#define RTSP_method_rcvd 1
#define RTSP_interlvd_rcvd 2
int RTSP_full_msg_rcvd(RTSP_buffer *rtsp, int *hdr_len, int *body_len);
#define RTSP_msg_len RTSP_full_msg_rcvd
void RTSP_discard_msg(RTSP_buffer *rtsp);
void RTSP_remove_msg(int len, RTSP_buffer *rtsp);
//      void RTSP_msg_len(int *hdr_len,int *body_len,RTSP_buffer *rtsp);
int RTSP_valid_response_msg(unsigned short *status, char *msg,
                            RTSP_buffer *rtsp);
int RTSP_validate_method(RTSP_buffer *rtsp);
void RTSP_initserver(RTSP_buffer *rtsp, tsocket fd, struct sockaddr_in *dest_addr, RTSP_proto proto);

// DESCRIBE
int send_describe_reply(RTSP_buffer *rtsp, char *object,
                        description_format descr_format, char *descr);

// REDIRECT 3xx
uint32 send_redirect_3xx(RTSP_buffer *, char *);

// SETUP
int send_setup_reply(RTSP_buffer *rtsp, RTSP_session *session,
                     SD_descr *descr, RTP_session *sp2);

// TEARDOWN
int send_teardown_reply(RTSP_buffer *rtsp, long session_id, long cseq);

// PLAY
int send_play_reply(RTSP_buffer *rtsp, char *object,
                    RTSP_session *rtsp_session);

// PAUSE
int send_pause_reply(RTSP_buffer *rtsp, RTSP_session *rtsp_session);

// OPTIONS
int send_options_reply(RTSP_buffer *rtsp, long cseq);

// messages functions
int max_connection();
char *get_stat(int err);
int send_reply(int err, char *addon, RTSP_buffer *rtsp);
int bwrite(char *buffer, unsigned short len, RTSP_buffer *rtsp);
void add_time_stamp(char *b, int crlf);
// low level funcntions
ssize_t RTSP_send(RTSP_buffer *rtsp);

#endif
