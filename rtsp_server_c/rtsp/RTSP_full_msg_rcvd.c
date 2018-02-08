/* * 
 *  $Id: RTSP_full_msg_rcvd.c 350 2006-05-31 18:22:15Z shawill $
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
#include <string.h>

#include <fenice/rtsp.h>
#include <fenice/utils.h>
#include <fenice/fnc_log.h>

/*���ݻ����������ݣ������������������,��黺��������Ϣ��������
 * return -1 on ERROR
 * return RTSP_not_full (0) if a full RTSP message is NOT present in the in_buffer yet.
 * return RTSP_method_rcvd (1) if a full RTSP message is present in the in_buffer and is
 *                     ready to be handled.
 * return RTSP_interlvd_rcvd (2) if a complete RTP/RTCP interleaved packet is present.  
 * terminate on really ugly cases.
 */
int RTSP_full_msg_rcvd(RTSP_buffer *rtsp, int *hdr_len, int *body_len)
/* This routine is from OMS.*/
{
    int eomh;    /* end of message header found */
    int mb;       /* message body exists */
    int tc;         /* terminator count */
    int ws;        /* white space */
    unsigned int ml;              /* total message length including any message body */
    int bl;                           /* message body length */
    char c;                         /* character */
    int control;
    char *p;

    /*�Ƿ���ڽ����ȡ�Ķ�����rtp/rtcp���ݰ����ο�RFC2326-10.12*/
    if (rtsp->in_buffer[0] == '$') 
    {
        uint16 *intlvd_len = (uint16 *)&rtsp->in_buffer[2];   /*����ͨ����־��*/

        /*ת��Ϊ�����ֽ�����Ϊ�����������ֽ���*/
        if ( (bl = ntohs(*intlvd_len)) <= rtsp->in_size)           
        {
            // fnc_log(FNC_LOG_DEBUG,"Interleaved RTP or RTCP packet arrived (len: %hu).\n", bl);
            if (hdr_len)
                *hdr_len = 4;
            if (body_len)
                *body_len = bl;
            return RTSP_interlvd_rcvd;
        }
        else
        {
            /*������������ȫ�������*/
            fnc_log(FNC_LOG_DEBUG,"Non-complete Interleaved RTP or RTCP packet arrived.\n");
            return RTSP_not_full;
        }
        
    }

    
    eomh = mb = ml = bl = 0;
    while (ml <= rtsp->in_size)
    {
        /* look for eol. */
        /*���㲻�����س����������ڵ������ַ���*/
        control = strcspn(&(rtsp->in_buffer[ml]), "\r\n");
        if(control > 0)
            ml += control;
        else
            return ERR_GENERIC;

        /* haven't received the entire message yet. */
        if (ml > rtsp->in_size)
            return RTSP_not_full;        

            
        /* �����ս�����ж��Ƿ�����Ϣͷ�Ľ���*/
        tc = ws = 0;
        while (!eomh && ((ml + tc + ws) < rtsp->in_size)) 
        {
            c = rtsp->in_buffer[ml + tc + ws];
            /*ͳ�ƻس�����*/
            if (c == '\r' || c == '\n')
                tc++;                                         
            else if ((tc < 3) && ((c == ' ') || (c == '\t')))
            {
                ws++;                           /*�س�������֮��Ŀո����TAB��Ҳ�ǿ��Խ��ܵ� */
            }
            else
            {
                    break;
            }
        }
        
        /*
         *һ�Իس������з�������ͳ��Ϊһ�����ս��
         * ˫�п��Ա����ܣ���������Ϊ����Ϣͷ�Ľ�����ʶ
         * ����RFC2068�е�����һ�£��ο�rfc2068 19.3
         *���򣬶����е�HTTP/1.1����Э����ϢԪ����˵��
         *�س������б���Ϊ�ǺϷ������ս��
         */

        /* must be the end of the message header */
        if ((tc > 2) || ((tc == 2) && (rtsp->in_buffer[ml] == rtsp->in_buffer[ml + 1])))
            eomh = 1;                                
        ml += tc + ws;

        if (eomh)
        {
            ml += bl;   /* ������Ϣ�峤�� */
            if (ml <= rtsp->in_size)
                break;  /* all done finding the end of the message. */
        }

        if (ml >= rtsp->in_size)
            return RTSP_not_full;   /* ��û����ȫ������Ϣ */

        /*���ÿһ�еĵ�һ���Ǻţ�ȷ���Ƿ�����Ϣ����� */
        if (!mb)
        {          
            /* content length token not yet encountered. */
            if (!strncasecmp(&(rtsp->in_buffer[ml]), HDR_CONTENTLENGTH, strlen(HDR_CONTENTLENGTH))) 
            {
                mb = 1;                        /* ������Ϣ��. */
                ml += strlen(HDR_CONTENTLENGTH);

                /*����:�Ϳո��ҵ������ֶ�*/
                while (ml < rtsp->in_size) 
                {
                    c = rtsp->in_buffer[ml];
                    if ((c == ':') || (c == ' '))
                        ml++;                  
                    else
                        break;
                }

                if (sscanf(&(rtsp->in_buffer[ml]), "%d", &bl) != 1) 
                {
                    fnc_log(FNC_LOG_ERR,"RTSP_full_msg_rcvd(): Invalid ContentLength encountered in message.\n");
                    return ERR_GENERIC;
                }
            }
        }
    }

    if (hdr_len)
        *hdr_len = ml - bl;

    if (body_len) {
    /*
     * go through any trailing nulls.  Some servers send null terminated strings
     * following the body part of the message.  It is probably not strictly
     * legal when the null byte is not included in the Content-Length count.
     * However, it is tolerated here.
     */
        for (tc = rtsp->in_size - ml, p = &(rtsp->in_buffer[ml]); tc && (*p == '\0'); p++, bl++, tc--);
            *body_len = bl;
    }

    return RTSP_method_rcvd;
}
