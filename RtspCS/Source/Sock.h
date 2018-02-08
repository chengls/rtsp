#ifndef __INCLUDE_SOCK_H__
#define __INCLUDE_SOCK_H__

#include "Def.h"
#ifdef _WIN32
#include <Ws2tcpip.h>
#else	//_WIN32
#include <arpa/inet.h>
#endif	//_WIN32

//�׽��ֻ���
class CSock{
public:
	CSock();
	virtual ~CSock();
public:
	//�ر��׽��֣���������ʱ���Զ�����
	void Close();
	//��ȡ�׽���fd
	//����ֵ��-1��ʾ���׽����಻����
	int GetFd() const;
	//��socket fd
	//������fd��socket fd
	//����ֵ��0�ɹ���-1ʧ��
	int AttachFd( int fd );
	//�����
	//����ֵ��֮ǰ�󶨵�fd
	int DetachFd();
	//����
	//������buf���������ݣ�len�����ͳ���
	//����ֵ: <0ʧ�ܣ�>=0���ͳ���
	virtual int Send( const char* buf, int len ){ return -1; }
	//����
	//������buf�����ջ��棬len�����ջ��泤��
	//����ֵ: <0ʧ�ܣ�>=0���ճ���
	virtual int Recv( char* buf, int len ){ return -1; }
protected:
	int open( int sock_type );
	int bind( const char* ip, int port );
	int get_addr( const char* ip, int port, sockaddr_in& addr );
	int set_addr_reuse();
	int set_block_opt( bool is_block );
protected:
	enum{
		INVALID_SOCK_FD = -1,
	};
	int m_fd;
};

#endif
