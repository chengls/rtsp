#include "logimp.h"
#include "common.h"

#ifdef _DEBUG //ʵ��д��־�ļ��ӿ�
#ifdef _LOGFILE
extern FILE  *__logfp; //��com_bql_MediaPlayer_Native.cpp�ﶨ��/��ʼ��, ��JNI_load.cpp���ͷ�
extern CMutex __logmutex;
///////////////////////////////////////////////////////////////////
void __file_log_write(int level, const char *n, const char *t, ...)
{// ֧�ֶ��̵߳���
	if( __logfp == 0 ) return;

	struct timeval v;
	gettimeofday(&v, 0);
	struct tm *p = localtime(&v.tv_sec);
	char fmt[256]; //����t����̫��
	sprintf(fmt, "%04d/%02d/%02d %02d:%02d:%02d,%03d %d %s %s %s\n", 1900 + p->tm_year, 1 + p->tm_mon, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec, (int)(v.tv_usec/1000), gettid(),
			level==ANDROID_LOG_VERBOSE? "TRACE":(level==ANDROID_LOG_DEBUG? "DEBUG":(level==ANDROID_LOG_INFO? "INFO":(level==ANDROID_LOG_WARN? "WARN":"ERROR"))), n, t);

	__logmutex.Enter();
	va_list params;
	va_start(params, t);
	vfprintf(__logfp, fmt, params);
	va_end(params);
	fflush(__logfp);
	__logmutex.Leave();
}

#endif
#endif