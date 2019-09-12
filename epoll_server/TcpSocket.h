#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#pragma once
#include "SocketBase.h"
#include "HostAddress.h"

//客户端TCP socket 实现基类Create 自己多了 connect
class CTcpSocket :public CSocketBase
{
public:
	CTcpSocket(void);
	CTcpSocket(const CTcpSocket &other);
	CTcpSocket(int fd);
	virtual ~CTcpSocket(void);
	
	CTcpSocket & operator = (const CTcpSocket &other);
	CHostAddress GetRemoteAddr() const;
	void SetRemoteAddr(const CHostAddress remote_addr);
	bool Connect(CHostAddress addr);
	int Read(char *buf, const int buf_len);
	int Read(char *buf, const int buf_len, struct timeval *tv);
	int Readn(char *buf, const int buf_len);
	int Write(char *buf, const int buf_len);
	int Write(char *buf, const int buf_len, struct timeval *tv);
	int Writen(char *buf, const int buf_len);
protected:
	void Create();
	CHostAddress m_remote_addr; //对方的连接地址
};


#endif

