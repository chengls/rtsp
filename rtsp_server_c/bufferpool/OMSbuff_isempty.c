/* * 
 *  $Id: OMSbuff_isempty.c 324 2006-03-23 12:02:22Z shawill $
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

#include <stdio.h>

#include <fenice/bufferpool.h>

#define RETURN(x) { \
                                OMSbuff_unlock(cons->buffer); \
                                return x; \
                                }
/*! Check if buffer is empty for a given consumer. 
 *
 * Checks in this function are taken from <tt>OMSbuff_read()</tt> and not
 * optimized
 *
 * \param Consumer to be checked.
 * \return 1 if buffer is empty, 0 if not.
 * \return -1 on error.
 *
 * \see OMSbuff_read
 * */
int OMSbuff_isempty(OMSConsumer *cons)
{
    OMSSlot *last_read;
    OMSSlot *next;

    OMSbuff_lock(cons->buffer);

    OMSbuff_shm_refresh(cons->buffer);

    last_read = OMStoSlot(cons->buffer, cons->last_read_pos);
    next = &cons->buffer->slots[cons->read_pos];

    if ( !next->refs || (next->slot_seq < cons->last_seq) )
    {
        /*added some slots*/    
        if ( last_read && cons->buffer->slots[last_read->next].refs  \
               && (cons->buffer->slots[last_read->next].slot_seq > cons->last_seq) )
            RETURN(0)
        else
            RETURN(1)
    }
    else if (last_read && ( cons->buffer->slots[last_read->next].slot_seq < next->slot_seq ) )
        RETURN(0)

    OMSbuff_unlock(cons->buffer);

    return 0;
}
#undef RETURN
