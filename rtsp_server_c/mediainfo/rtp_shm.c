/* * 
 *  $Id: rtp_shm.c 324 2006-03-23 12:02:22Z shawill $
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

#include <fenice/mediainfo.h>
#include <fenice/utils.h>
#include <fenice/fnc_log.h>

int load_RTP_SHM(media_entry *p)
{
    fnc_log(FNC_LOG_DEBUG, "loading RTP_SHM...\n");

    if (!(p->description.flags & MED_PKT_LEN)) 
    {
        if (!(p->description.flags & MED_FRAME_RATE)) 
        {
            return ERR_PARSE;
        }
        p->description.pkt_len=1/(double)p->description.frame_rate*1000;
        p->description.flags|=MED_PKT_LEN;
    }
    p->description.delta_mtime=p->description.pkt_len;
    return ERR_NOERROR;
}
 
int read_RTP_SHM(media_entry *me,uint8 *data,uint32 *data_size,double *mtime, int *recallme, uint8 *marker)
{
    fnc_log(FNC_LOG_DEBUG, "Shared memory got empty!\n");
    return ERR_EOF;
}


int free_RTP_SHM(void *stat)
{
    return 0;
}
