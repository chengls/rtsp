/* * 
 *  $Id: RTP_get_port_pair.c 80 2005-01-25 17:51:15Z federico $
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

#include <fenice/rtp.h>
#include <fenice/utils.h>

extern int start_port;
extern int port_pool[ONE_FORK_MAX_CONNECTION];

int RTP_get_port_pair(port_pair *pair)
{
    int i;

    for (i=0; i<ONE_FORK_MAX_CONNECTION; ++i) 
    {
        if (port_pool[i]!=0) 
        {
            pair->RTP=(port_pool[i]-start_port)*2+start_port;
            pair->RTCP=pair->RTP+1;
            port_pool[i]=0;
            return ERR_NOERROR;
        }
    }
    return ERR_GENERIC;
}

