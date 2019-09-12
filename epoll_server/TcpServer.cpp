#include "TcpServer.h"

CTcpServer::CTcpServer()
{
	Create();
}

CTcpServer::~CTcpServer()
{
	close(m_fd);
}

void CTcpServer::Create()
{
	CSocketBase::Create(AF_INET, SOCK_STREAM, 0);
}

bool CTcpServer::Listen(int backlog)
{
	int ret = listen(m_fd,backlog);
	if (ret < 0)
	{
		perror("listen error:");
	}
	m_stream.SetNonBlock(true);
	return ret==0 ? true:false;
}

CTcpSocket CTcpServer::Accept()
{
	CHostAddress remote_addr;
	int fd = accept(m_fd, remote_addr.Address(), remote_addr.LengthPtr());
	
	CTcpSocket tcpclient(fd);
	tcpclient.SetRemoteAddr(remote_addr);
	
	return tcpclient;
}


