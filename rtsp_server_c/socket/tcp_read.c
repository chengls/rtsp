/* * 
 *  $Id: tcp_read.c 351 2006-06-01 17:58:07Z shawill $
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

#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <fenice/socket.h>
#include <fenice/fnc_log.h>

int tcp_read(tsocket fd, void *buffer, int nbytes)
{
    int n;
    struct sockaddr_storage name;
    socklen_t namelen = sizeof(name);
    char addr_str[128];   /* Unix domain is largest */

#ifndef WIN32
    n=read(fd,buffer,nbytes);
#else
    n=recv(fd,buffer,nbytes,0);
#endif

    if(n>0)
    {
        if( getpeername(fd, (struct sockaddr*)&name, &namelen) < 0 )
            fnc_log(FNC_LOG_CLIENT,"- - - ");
        else
        {
            #ifdef RTSP_METHOD_LOG
            fnc_log(FNC_LOG_CLIENT,"%s - - ", sock_ntop_host((struct sockaddr *)&name, namelen, addr_str, sizeof(addr_str)) );
            #endif
        }
    }

    return n;
}

