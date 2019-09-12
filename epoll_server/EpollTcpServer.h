#ifndef __EPOLLTCPSERVER_H
#define __EPOLLTCPSERVER_H


#include "TcpServer.h"
#include "EpollBase.h"

class CEpollTcpServer : public CEpollBase
{
public:
    CEpollTcpServer(CHostAddress addr, unsigned int maxEvent = 20, int timeout = -1);
    ~CEpollTcpServer();
protected:

    CTcpServer m_tcpServer;
    
    virtual void OnReadData(int fd/*, char *buf, int bufLen*/);
    virtual void OnNewConnection();
    virtual void OnClientClose(int fd);
    void Run(int nevent);
};

#endif //_EPOLLTCPSERVER_H_

