#include "EpollTcpServer.h"
#include "TcpSocket.h"

CEpollTcpServer::CEpollTcpServer(CHostAddress addr, unsigned int maxEvent, int timeout)
:CEpollBase(maxEvent, timeout)
{
    m_tcpServer.Bind(addr);
    m_tcpServer.Listen(100);
	
    AddEvent(m_tcpServer.GetSocket(), EPOLLIN);// 10086 epoll
}

CEpollTcpServer::~CEpollTcpServer()
{

}

void CEpollTcpServer::Run(int nevent)
{
    for (int i = 0; i < nevent; i++)
    {
        if ((m_retEvents+i)->data.fd == m_tcpServer.GetSocket())
        {
            OnNewConnection();
        }
        else if ((m_retEvents+i)->events & EPOLLIN)
        {
            OnReadData((m_retEvents+i)->data.fd);
        }
    }
}

void CEpollTcpServer::OnNewConnection()
{
    CTcpSocket client = m_tcpServer.Accept();
    AddEvent(client.GetSocket(), EPOLLIN);
}

void CEpollTcpServer::OnClientClose(int fd)
{
    DelEvent(fd, EPOLLIN);
    close(fd);
}

void CEpollTcpServer::OnReadData(int fd)
{
    //派生类决定
    //buf
    printf("Client[%d] read event\n", fd);
}
// CEpollTcpServer epollsever(CHostAddress addr("127.0.0.1",10086));
// epollsever.Start();
// epollsever.Stop();




