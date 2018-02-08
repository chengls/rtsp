#ifndef __INCLUDE_PRINT_LOG_H__
#define __INCLUDE_PRINT_LOG_H__

#include "Def.h"
#include "Mutex.h"
#include <stdio.h>

#define LogDebug( fmt, ... ) CPrintLog::PrintDebug( __FILE__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__ )
#define LogInfo( fmt, ... ) CPrintLog::PrintInfo( __FILE__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__ )
#define LogError( fmt, ... ) CPrintLog::PrintError( __FILE__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__ )

#define LogPoint() CPrintLog::PrintDebug( __FILE__, __FUNCTION__, __LINE__, "\n" )

class CPrintLog
{
public:
	//����־�ļ�������־�ļ��󽫻Ὣ��־��Ϣ��¼����Ӧ����־�ļ���
	//������file_name���ļ���ǰ׺��file_name_������_ʱ����.log
	//����ֵ��>=0�ɹ���<0ʧ��
	static int OpenLogFile( const char* file_name, const char* path = NULL );
	//�ر���־�ļ�
	static void CloseLogFile();
	//������־�ļ�����С�������ô�С��һ�ļ���Ĭ��1Mb
	//������size����־�ļ���С����λKb
	static void SetLogFileSize( int size );
	enum{
		DEBUG_LEVEL = 0,	//�ü������Debug��Info��Error��Ϣ
		INFO_LEVEL,			//�ü������Info��Error��Ϣ
		ERROR_LEVEL,		//�ü������Error��Ϣ
		NONE_LEVEL,			//�ü�������κ���Ϣ
	};
	//���ô�ӡ����
	//������level����ӡ����
	static void SetPrintLevel( int level );
	//��ӡ��ϢЯ��ʱ�䡢�ļ��������������кš���ӡ������־��Ϣ
	static void PrintDebug( const char* file, const char* fun, int line, const char* fmt, ... );
	static void PrintInfo( const char* file, const char* fun, int line, const char* fmt, ... );
	static void PrintError( const char* file, const char* fun, int line, const char* fmt, ... );
	//��ӡ��Ϣ��Я����־��Ϣ
	static void PrintDebugSimple( const char* fmt, ... );
	static void PrintInfoSimple( const char* fmt, ... );
	static void PrintErrorSimple( const char* fmt, ... );
private:
	static void print_log( const char* file, const char* fun, int line, const char* name, const char* buf );
	static void print_log_simple( const char* buf );
	static int open_file();
private:
	static FILE* m_log_file;
	static int m_log_level;
	static CMutex m_mutex;
	static int m_max_file_size;
	static char m_file_name[128];
	static char m_path[128];
};

#endif
