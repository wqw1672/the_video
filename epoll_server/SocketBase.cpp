/*************************************************************************
	> File Name: BaseSocket.cpp
	> Author: eric
	> Mail:
	> Created Time: 2017年11月28日 星期二 07时01分47秒
 ************************************************************************/

#include <iostream>
#include <fcntl.h>
#include "SocketBase.h"
using namespace std;

CSocketBase::CSocketBase()
{
    m_fd = -1;
    m_stream.SetFd(m_fd);
}

CSocketBase::CSocketBase(int type, int sin_farmly /* = AF_INET */, int protocol /* = 0 */)
{
    Create(sin_farmly, type, protocol);
}

CSocketBase::~CSocketBase()
{
}

void CSocketBase::Create(int sin_farmly, int flag, int protocl)
{
    m_fd = socket(sin_farmly, flag, protocl);
    if (m_fd < 0)   //当文件描述符达到上限时会创建失败
    {
        perror("create socket error:");
    }
    m_stream.SetFd(m_fd);
    
  /*  iSockopt = 1;
    int i;
	if((i = setsockopt(m_fd,SOL_SOCKET,SO_REUSEADDR,(const char*)&iSockopt,sizeof(int))) < 0)
		{
			close(m_fd);
			exit(0);
		}
		printf("%d\n",i);
*/
}

void CSocketBase::SetSocket(int fd)
{
    if (m_fd > 0)
    {
        //printf("old fd %d will change to %d\n", m_fd, fd);//为何warning
    }
    if (fd < 0 || m_fd != fd)
    {
        //printf("set fd %d\n", fd);
	    m_fd = fd;
		m_stream.SetFd(m_fd);
		m_stream.SetNonBlock(true);
    }
}

//getsockname函数用于获取与某个套接字关联的本地协议地址 
//getpeername函数用于获取与某个套接字关联的外地协议地址

CHostAddress CSocketBase::GetAddress()
{
    CHostAddress addr;
    memset(&addr, 0, sizeof(addr));
    if (getsockname(m_fd, addr.Address(), addr.LengthPtr()) != 0)//获取监听的地址和端口,看是否连接成功
    {
        perror("getsockname error:");
    }
    return addr;
}

CHostAddress CSocketBase::GetPeerAddress()
{
    CHostAddress addr;
    memset(&addr, 0, sizeof(addr));
    if (getpeername(m_fd, addr.Address(), addr.LengthPtr()) != 0)//获取监听的地址和端口,看是否连接成功
    {
        perror("getpeername error:");
    }
    return addr;
}

bool CSocketBase::Bind(CHostAddress addr)
{
	/////////////////////////////////////
/*	iSockopt = 1;
	if(setsockopt(m_fd,SOL_SOCKET,SO_REUSEADDR,(const char*)&iSockopt,sizeof(int)) < 0)
		{
			close(m_fd);
			return -1;
		}*/
	///////////////////////////////////////
    int ret = bind(m_fd, addr.Address(), addr.Length());
    if (ret < 0)
    {
        perror(" bind error:");
    }
    return  ret==0 ? true : false;
}

bool CSocketBase::Close()
{
    if (m_fd < 0)
    {
        printf("close sockfd fd %d\n",m_fd);
        return false;
    }
    int ret = close(m_fd);
    return  ret==0 ? true : false;
}



