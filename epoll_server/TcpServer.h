#ifndef TCPSERVER_H
#define TCPSERVER_H

#pragma once

#include "SocketBase.h"
#include "TcpSocket.h"

class CTcpServer : public CSocketBase
{
public:
	CTcpServer();
	virtual ~CTcpServer();

	bool Listen(int backlog);
	CTcpSocket Accept();
	
private:
	void Create();
};


#endif
