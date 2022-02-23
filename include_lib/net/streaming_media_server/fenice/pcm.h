/* *
 *  $Id$
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

#ifndef _PCMH
#define _PCMH

#include "types.h"
#include "mediainfo.h"

typedef struct {
    uint32 bytes_left;
    uint32 read_size;
    uint32 stored;
    uint32 read_ag;
    uint32 frame_left;
} PCM_WORK;

int load_L16(media_entry *me);
int read_PCM(media_entry *me, uint8 *buffer, uint32 *buffer_size,
             uint64 *mtime, int *recallme, uint8 *marker);
int read_PCMA(media_entry *me, uint8 *buffer, uint32 *buffer_size,
              uint64 *mtime, int *recallme, uint8 *marker);
int reload_L16(media_entry *p, int samp_rate, int channel_num, int bit_per_samp);
int free_L16(void *stat);

#endif
