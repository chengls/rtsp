#ifndef __INCLUDE_MUTEX_H__
#define __INCLUDE_MUTEX_H__

#include "Def.h"
#ifdef _WIN32
#include <WinSock2.h>
#else	//_WIN32
#include <pthread.h>
#endif	//_WIN32

//�̻߳�������
class CMutex
{
public:
	CMutex();
	~CMutex();
public:
	//����
	void Enter();
	//����
	void Leave();
private:
#ifdef _WIN32
	CRITICAL_SECTION m_mutex;
#else
	pthread_mutex_t m_mutex;
#endif
};

/////////////
//����ʱ����������ʱ����
class CGuard
{
public:
	CGuard( CMutex& mutex ):m_mutex(mutex){ m_mutex.Enter(); }
	~CGuard(){ m_mutex.Leave(); }
private:
	CMutex& m_mutex;
};

#endif
