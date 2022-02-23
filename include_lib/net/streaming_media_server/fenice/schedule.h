/* *
 *  $Id: schedule.h 387 2006-09-26 15:12:00Z shawill $
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

#ifndef _SCHEDULEH
#define _SCHEDULEH



#include "types.h"
#include "rtp.h"
#include "prefs.h"
#include "debug.h"


typedef struct _play_args {
    struct tm playback_time;
    short playback_time_valid;
    uint32 start_time;	// In secondi, anche frazionari
    short start_time_valid;
    uint32 end_time;
} play_args;

typedef struct _schedule_list {
    int valid;
    RTP_session *rtp_session;
    //RTSP_session *rtsp_session;
    RTP_play_action play_action;
} schedule_list;

int schedule_init();


void schedule_do(void *nothing);

int schedule_add(RTP_session *rtp_session /*,RTSP_session *rtsp_session */);
int32 schedule_start(int id, play_args *args);
void schedule_stop(int id);
int schedule_remove(int id);
int schedule_resume(int id, play_args *args);

#endif
