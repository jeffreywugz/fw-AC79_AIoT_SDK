/* *
 *  $Id: eventloop.h 397 2006-10-06 15:08:29Z dario $
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

#ifndef _EVENTLOOPH
#define _EVENTLOOPH

#include "socket.h"
#include "rtsp.h"

#define MAX_FDS 800

typedef int (*event_function)(void *data);

void eventloop(tsocket main_fd, tsocket main_sctp_fd);
int rtsp_server(RTSP_buffer *rtsp, fd_set *rset, fd_set *wset, fd_set *xset);
void add_client(RTSP_buffer **rtsp_list, tsocket fd, struct sockaddr_in *dest_addr, RTSP_proto proto);
void schedule_connections(RTSP_buffer **rtsp_list, int *conn_count, fd_set *rset, fd_set *wset, fd_set *xset);
void rtsp_set_fdsets(RTSP_buffer *rtsp, int *max_fd, fd_set *rset, fd_set *wset, fd_set *xset);

#endif
