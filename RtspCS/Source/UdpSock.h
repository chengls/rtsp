#ifndef __INCLUDE_UDP_SOCK_H__
#define __INCLUDE_UDP_SOCK_H__

#include "Def.h"
#include "Sock.h"

//UDP�׽�����
class CUdpSock : public CSock
{
public:
	CUdpSock();
	~CUdpSock();
public:
	//�򿪴��׽���
	int Open( const char* ip, int port );
	//����Զ�˵�ַ
	//������remote��Զ�������ַ
	//����ֵ��0�ɹ���-1ʧ��
	int SetRemote( const char* ip, int port );
	//����
	//������buf���������ݣ�len�����ͳ���
	//����ֵ��0�ɹ���-1ʧ��
	int Send( const char* buf, int len );
	//����
	//������buf�����ջ��棬len�����泤��
	//����ֵ��0�ɹ���-1ʧ��
	int Recv( char* buf, int len );
public:
	//���öಥ���ͱ��ĵ�TTL
	//����ֵ�� -1 ʧ�� 0 �ɹ�
	int SetMulticastTTL( uint8_t TTL );
	//�����Ƿ��ֹ�鲥���ݻ��ͣ�true-���ͣ�false-�����͡�
	//����ֵ: -1 ʧ�� 0 �ɹ�
	int SetMulticastLoop( bool isloop );
private:
	struct sockaddr_in m_remote_addr;
};

#endif