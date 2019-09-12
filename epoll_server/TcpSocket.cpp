#include "TcpSocket.h"

CTcpSocket::CTcpSocket(void)
{
	Create();
}

CTcpSocket::~CTcpSocket(void)
{
	
}

CTcpSocket::CTcpSocket(const CTcpSocket &other)
{
 	CSocketBase::SetSocket(other.GetSocket());
 	SetRemoteAddr(other.GetRemoteAddr());
}

CTcpSocket & CTcpSocket::operator =(const CTcpSocket &other)
{
 	CSocketBase::SetSocket(other.GetSocket());
 	SetRemoteAddr(other.GetRemoteAddr());
 	return *this;
}

CTcpSocket::CTcpSocket(int fd)
{
	CSocketBase::SetSocket(fd);
}

void CTcpSocket::Create()
{
	CSocketBase::Create(AF_INET, SOCK_STREAM, 0);
}

CHostAddress CTcpSocket::GetRemoteAddr() const
{	
    CHostAddress addr;
    memset(&addr, 0, sizeof(addr));
    if (getpeername(m_fd, addr.Address(), addr.LengthPtr()) != 0)
    {
        perror("getpeername error:");
    }
    return addr;
}


void CTcpSocket::SetRemoteAddr(const CHostAddress remote_addr)
{
	m_remote_addr = remote_addr;
}

bool CTcpSocket::Connect(CHostAddress addr)
{
	int ret = connect(m_fd, addr.Address(), addr.Length());
	if (ret == 0)
	{
		SetRemoteAddr(addr);
	}
	else
	{
		perror("connect to server error");
	}
	return ret==0 ? true:false; 
}

int CTcpSocket::Read(char *buf, const int buf_len)
{
	return m_stream.Read(buf,buf_len);
}

int CTcpSocket::Readn(char *buf, const int buf_len)
{
	return m_stream.Readn(buf,buf_len);
}

int CTcpSocket::Read(char *buf, const int buf_len, struct timeval *tv)
{
	return m_stream.Read(buf, buf_len, tv);
}

int CTcpSocket::Write(char *buf, const int buf_len)
{
	return m_stream.Write(buf,buf_len);
}

int CTcpSocket::Writen(char *buf, const int buf_len)
{
	return m_stream.Writen(buf,buf_len);
}

int CTcpSocket::Write(char *buf, const int buf_len, struct timeval *tv)
{
	return m_stream.Write(buf,buf_len,tv);
}

