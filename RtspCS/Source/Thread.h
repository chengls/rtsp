#ifndef __INCLUDE_THREAD_H__
#define __INCLUDE_THREAD_H__

#include "Def.h"
#ifdef _WIN32
#include <WinSock2.h>
#else //_WIN32
#include <pthread.h>
#endif	//_WIN32

//�߳��࣬�û��ɼ̳�CThreadʵ��thread_proc����thread_proc�д����¼���
//Ҳ�ɲ��̳�CThread��ͨ������ThreadFun�Դ����¼���
class CThread
{
public:
	CThread();
	virtual ~CThread();
public:
	//�̺߳�������
	//������thread���̶߳���ִָ�룬user_info���û���Ϣ
	typedef void (*ThreadFun)( CThread* thread, long user_info );
	//����
	//������thread_name���߳�����user_info���û���Ϣ��
	//		fun���̺߳���,NULL�������߳��е���thread_proc���������fun
	//����ֵ��0�ɹ���-1ʧ��
	int Create( const char* thread_name, long user_info, ThreadFun fun = NULL );
	//����
	void Destroy();
	//�ȴ��˳��̣߳����ȵ���Destroy
	void WaitExit();
	//�Ƿ��Ѿ����٣������̴߳����������ж��Ƿ��Ѿ����٣����������践��
	//����ֵ��true�������٣�false��δ����
	bool IsDestroyed();
	//���߳��Ƿ���������
	//����ֵ��true���������У�false��δ����
	bool IsRuning();
	//��ȡ��ǰ�߳�ID
	//����ֵ����ǰ�߳�ID
	static int GetCurThreadId();
private:
	//���߳�ִ�д����������û��̳�ʵ��
	//������user_info���û���Ϣ
	virtual void thread_proc( long user_info ){ return; }
private:
#ifdef _WIN32
	static DWORD WINAPI thread_fun( LPVOID arg );
#else
	static void* thread_fun( void *arg );
#endif
private:
	bool m_is_destroyed;
	bool m_is_runing;
#ifdef _WIN32
	HANDLE m_handle;
#else
	pthread_t m_handle;
#endif
	bool m_is_exited;
	ThreadFun m_fun;
	long m_user_info;
	char m_thread_name[128];
};

#endif