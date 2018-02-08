#ifndef __INCLUDE_TCP_SOCK_H__
#define __INCLUDE_TCP_SOCK_H__

#include "Def.h"
#include "Sock.h"

//TCP�׽���
class CTcpSock : public CSock
{
public:
	CTcpSock();
	~CTcpSock();
public:
	int Connect( const char* ip, int port );
	//����
	//������buf���������ݣ�len�����ͳ���
	//����ֵ: <0ʧ�ܣ�>=0���ͳ���
	int Send( const char* buf, int len );
	//����
	//������buf�����ջ��棬len�����ջ��泤��
	//����ֵ: <0ʧ�ܣ�>=0���ճ���
	int Recv( char* buf, int len );
};

#endif
