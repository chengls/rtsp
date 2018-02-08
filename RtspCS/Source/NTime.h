#ifndef __INCLUDE_N_TIME_H__
#define __INCLUDE_N_TIME_H__

#include "Def.h"

//ϵͳʱ����
class CNTime
{
public:
	//��ȡϵͳ��������ʱ�ĺ���ʱ��
	//����ֵ��ϵͳ��������ʱ�ĺ���ʱ��
	static uint64_t GetCurMs();
	//��ȡϵͳ��������ʱ��΢��ʱ��
	//����ֵ��ϵͳ��������ʱ��΢��ʱ��
	static uint64_t GetCurUs();
#ifdef _WIN32
private:
	static uint64_t get_freq();
private:
	static uint64_t m_freq;
#endif
};

#endif