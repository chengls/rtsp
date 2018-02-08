/* * 
 *  $Id: RTSP_play.c 338 2006-04-27 16:45:52Z shawill $
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
#include <netinet/in.h>

#include <fenice/bufferpool.h>
#include <fenice/rtsp.h>
#include <fenice/utils.h>
#include <fenice/prefs.h>
#include <fenice/fnc_log.h>

#include "../config.h"
/*
****************************************************************
*PLAY �����Ĵ���
****************************************************************
*/

int RTSP_play(RTSP_buffer * rtsp)
{
    DEBUG_PRINT_INFO("prepare to play!\n");
    
    /*�ֲ�����*/
    int url_is_file;
    char object[255], server[255], trash[255];
    char url[255];
    unsigned short port;
    char *p = NULL, *q = NULL;
    long int session_id;
    RTSP_session *ptr;
    RTP_session *ptr2;
    play_args args;
    int time_taken = 0;

    /*����Ƿ�������к�ͷ*/
    if ((p = strstr(rtsp->in_buffer, HDR_CSEQ)) == NULL) 
    {
        send_reply(400, 0, rtsp);   /* Bad Request */
        return ERR_NOERROR;
    } 
    else 
    {
        if (sscanf(p, "%254s %d", trash, &(rtsp->rtsp_cseq)) != 2) 
        {
            send_reply(400, 0, rtsp);    /* Bad Request */
            return ERR_NOERROR;
        }
    }
    
    /*��ȡ���ŷ�Χ*/
    args.playback_time_valid = 0;
    args.start_time_valid = 0;

    /*���Rangeͷ*/
    if ((p = strstr(rtsp->in_buffer, HDR_RANGE)) != NULL) 
    {
        q = strstr(p, "npt");   /*����NPT*/
        if (q == NULL) 
        {
            q = strstr(p, "smpte");  /*����smpte*/
            if (q == NULL) 
            {
                q = strstr(p, "clock");
                
                if (q == NULL) 
                {
                    /*û��ָ����ʽ��ʹ�� NeMeSI   ��ʽ*/                                   
                    if ((q = strstr(p, "time")) == NULL) 
                    {
                         
                        double t;
                        q = strstr(p, ":");
                        sscanf(q + 1, "%lf", &t);
                        
                        args.start_time = t * 60 * 60;
                        
                        /*Min*/
                        q = strstr(q + 1, ":");
                        sscanf(q + 1, "%lf", &t);
                        args.start_time += (t * 60);
                        
                        /*Sec*/ 
                        q = strstr(q + 1, ":");
                        sscanf(q + 1, "%lf", &t);
                        args.start_time += t;
                        args.start_time_valid = 1;
                    }
                    else      /*���� time*/
                    {
                        args.start_time = 0;
                        args.end_time = 0;
                        time_taken = 1;
                    }
                } 
                else 
                {
                    /*��֧��clock��ʹ��Ĭ��ֵ*/
                    args.start_time = 0;
                    args.end_time = 0;
                }
            } else
            {
                /*snmpte, ��֧�֣�ʹ��Ĭ��*/
                args.start_time = 0;
                args.end_time = 0;
            }
        }
        else
        {
            /*����npt*/
            if ((q = strchr(q, '=')) == NULL) 
            {
                send_reply(400, 0, rtsp);/* Bad Request */
                return ERR_NOERROR;
            }
            sscanf(q + 1, "%f", &(args.start_time));          /*��ʼʱ��*/
            
            if ((q = strchr(q, '-')) == NULL) 
            {
                send_reply(400, 0, rtsp);/* Bad Request */
                return ERR_NOERROR;
            }
            if (sscanf(q + 1, "%f", &(args.end_time)) != 1)      /*����ʱ��*/
            {
                args.end_time = 0;                 /*play all the media,until ending ,yanf*/
            }
        }

        /*���ҿ�ʼ�ط�ʱ��*/
        if ((q = strstr(p, "time")) == NULL) 
        {
            /*û��ָ���������ط�*/
            memset(&(args.playback_time), 0, sizeof(args.playback_time));
        } 
        else 
        {
            if (!time_taken) 
            {
                q = strchr(q, '=');
                /*���ʱ��*/
                if (get_UTC_time(&(args.playback_time), q + 1) != ERR_NOERROR)
                {
                    memset(&(args.playback_time), 0, sizeof(args.playback_time));
                }
                args.playback_time_valid = 1;
            }
        }
    } 
    else
    {
        args.start_time = 0;
        args.end_time = 0;
        memset(&(args.playback_time), 0, sizeof(args.playback_time));
    }
    
    /*���к�*/
    if ((p = strstr(rtsp->in_buffer, HDR_CSEQ)) == NULL) 
    {
        send_reply(400, 0, rtsp);   /* Bad Request */
        return ERR_NOERROR;
    }
    
    /* If we get a Session hdr, then we have an aggregate control*/
    if ((p = strstr(rtsp->in_buffer, HDR_SESSION)) != NULL) 
    {
        if (sscanf(p, "%254s %ld", trash, &session_id) != 2) 
        {
            send_reply(454, 0, rtsp);/* Session Not Found */
            return ERR_NOERROR;
        }
    } 
    else 
    {
        send_reply(400, 0, rtsp);/* bad request */
        return ERR_NOERROR;
    }

    
    /*�����URL */
    if (!sscanf(rtsp->in_buffer, " %*s %254s ", url)) 
    {
        send_reply(400, 0, rtsp);/* bad request */
        return ERR_NOERROR;
    }
    
    /* ��֤URL����ȷ��*/
    switch (parse_url(url, server, sizeof(server), &port, object, sizeof(object)))
    {
        case 1: 
            send_reply(400, 0, rtsp);
            return ERR_NOERROR;
            break;
            
        case -1:
            send_reply(500, 0, rtsp);
            return ERR_NOERROR;
            break;
            
        default:
            break;
    }

    
    if (strcmp(server, prefs_get_hostname()) != 0) 
    {
    }

    /*��ֹ�������·������*/
    if (strstr(object, "../")) 
    {
        send_reply(403, 0, rtsp);   /* Forbidden */
        return ERR_NOERROR;
    }
    if (strstr(object, "./")) 
    {
        send_reply(403, 0, rtsp); /* Forbidden */
        return ERR_NOERROR;
    }


    p = strrchr(object, '.');
    url_is_file = 0;
    if (p == NULL) 
    {
        send_reply(415, 0, rtsp);   /* Unsupported media type */
        return ERR_NOERROR;
    } 
    else 
    {
        url_is_file = is_supported_url(p);
    }
    q = strchr(object, '!');
    /*����!*/

    if (q == NULL) 
    {
        /* PLAY <file.sd>*/
        ptr = rtsp->session_list;
        if (ptr != NULL) 
        {
            if (ptr->session_id == session_id) 
            {
                /*����RTP session*/ 
                for (ptr2 = ptr->rtp_session; ptr2 != NULL; ptr2 = ptr2->next) 
                {
                    if (ptr2->current_media->description.priority == 1)
                    {
                        /*����������ʾ*/
                        if (!ptr2->started) 
                        {
                            /*��ʼ�µĲ���*/ 
                            DEBUG_PRINT_INFO("+++++++++++++++++++++")
                            DEBUG_PRINT_INFO("start to play now!");
                            DEBUG_PRINT_INFO("+++++++++++++++++++++")
                            
                            if (schedule_start(ptr2->sched_id, &args) == ERR_ALLOC)
                                return ERR_ALLOC;
                        } 
                        else
                        {
                            /*�ָ���ͣ������*/ 
                            if (!ptr2->pause) 
                            {
                                //fnc_log(FNC_LOG_INFO,"PLAY: already playing\n");
                            } 
                            else 
                            {
                                schedule_resume(ptr2->sched_id, &args);
                            }
                        }
                    }
                }
            } 
            else 
            {
                send_reply(454, 0, rtsp);	/* Session not found*/
                return ERR_NOERROR;
            }
        } 
        else 
        {
            send_reply(415, 0, rtsp);  /* Internal server error*/
            return ERR_GENERIC;
        }
    }
    else
    {
        if (url_is_file) 
        {
            /*PLAY <file.sd>!<file>    */                     
            ptr = rtsp->session_list;
            if (ptr != NULL) 
            {
                if (ptr->session_id != session_id) 
                {
                    send_reply(454, 0, rtsp);   /*Session not found*/ 
                    return ERR_NOERROR;
                }
                /*����RTP�Ự*/
                for (ptr2 = ptr->rtp_session; ptr2 != NULL; ptr2 = ptr2->next) 
                {
                    if (strcmp(ptr2->current_media->filename, q + 1) == 0) 
                    {
                        break;
                    }
                }
                
                if (ptr2 != NULL) 
                {
                    if (schedule_start(ptr2->sched_id, &args) == ERR_ALLOC)
                        return ERR_ALLOC;
                } 
                else 
                {
                    send_reply(454, 0, rtsp);/* Session not found*/
                    return ERR_NOERROR;
                }
            } 
            else
            {
                send_reply(415, 0, rtsp);	/*Internal server error*/ 
                return ERR_GENERIC;
            }
        } 
        else 
        {
            // PLAY <file.sd>!<aggr>
            ptr = rtsp->session_list;
            if (ptr != NULL) 
            {
                if (ptr->session_id != session_id) 
                {
                    send_reply(454, 0, rtsp);/* Session not found*/
                    return ERR_NOERROR;
                }
                /*���Ƽ����������е�RTP*/ 
                for (ptr2 = ptr->rtp_session; ptr2 != NULL; ptr2 = ptr2->next) 
                {
                    if (!ptr2->started)
                    {
                        /*��ʼ�µĲ���*/ 
                        DEBUG_PRINTF("#----++++++++++++++++--------schedule to playing now --------------+++++++++++\n");
                        if (schedule_start(ptr2->sched_id, &args) == ERR_ALLOC)
                            return ERR_ALLOC;
                    } 
                    else
                    {
                        /*�ָ���ͣ������*/ 
                        if (!ptr2->pause) 
                        {
                            /*fnc_log(FNC_LOG_INFO,"PLAY: already playing\n");*/
                        } 
                        else 
                        {
                            schedule_resume(ptr2->sched_id, &args);
                        }
                    }
                }
            } 
            else 
            {
                send_reply(415, 0, rtsp);	// Internal server error
                return ERR_GENERIC;
            }
        }
    }

    /*���ͻظ���Ϣ*/ 
    send_play_reply(rtsp, object, ptr);
    
    #ifdef RTSP_METHOD_LOG   
    fnc_log(FNC_LOG_INFO,"PLAY %s RTSP/1.0 ",url); 
    // See User-Agent 
    if ((p=strstr(rtsp->in_buffer, HDR_USER_AGENT))!=NULL) 
    {
        char cut[strlen(p)];
        strcpy(cut,p);
        p=strstr(cut, "\n");
        cut[strlen(cut)-strlen(p)-1]='\0';
        fnc_log(FNC_LOG_CLIENT,"%s\n",cut);
    }
    else
        fnc_log(FNC_LOG_CLIENT,"- \n");
    #endif
        
    return ERR_NOERROR;
}
