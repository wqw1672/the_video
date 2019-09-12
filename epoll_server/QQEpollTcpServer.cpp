#include <sys/types.h>
#include <sys/ipc.h>
#include "QQEpollTcpServer.h"
#include "TcpSocket.h"

CShmFIFO  m_sendFIFO;
CShmFIFO  m_recvFIFO;

CQQEpollTcpServer::CQQEpollTcpServer( CHostAddress addr, unsigned int maxEvent, int timeout)
	:CEpollTcpServer(addr, maxEvent, timeout)
{
	m_packet = new CQQPacket();
	m_sendFIFO.init(KEY_FIFO_SEND, m_packet->getSize(), 100);
	m_recvFIFO.init(KEY_FIFO_RECV, m_packet->getSize(), 100);

	//创建收发的线程，用于接收后置服务的包
	//再返回给邋QQ客户端
	m_sendThread = new CQQSendThread();
}

CQQEpollTcpServer::~CQQEpollTcpServer()
{
	m_mclients.clear();				//删除信息表
	m_sendFIFO.destroy();
	m_recvFIFO.destroy();
	delete m_packet;
}

void CQQEpollTcpServer::Start()
{
    this->m_bStop = false;
	//服务器启动后创建socket回送线程
		m_sendThread->start();
    while (!this->m_bStop)
    {
        int nevent = this->Wait();
        if(nevent)
        {
        	this->Run(nevent);
        }
        OnTimeMonit(m_timeout, 60);//30 sencond timeout
    }
}

void CQQEpollTcpServer::Run(int nevent)
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

void CQQEpollTcpServer::OnNewConnection()
{
    CTcpSocket client = m_tcpServer.Accept();
    AddEvent(client.GetSocket(), EPOLLIN);

    // Add to map
    QQCLIENT_T newclient;
    newclient.fd = client.GetSocket();
    newclient.delay = 10;
    newclient.addr = client.GetRemoteAddr();
    m_mclients.insert(make_pair(client.GetSocket(), newclient));//Add to map
}

void CQQEpollTcpServer::OnClientClose(int fd)
{
    DelEvent(fd, EPOLLIN);
    close(fd);
      OnResetTime(fd);
    // delete from map
	MP_CLIENT::iterator it;
	it = m_mclients.find(fd);
	if (it != m_mclients.end())
	{
		m_mutex.lock();
		m_mclients.erase(it);				//remove from map
		m_mutex.unlock();
	}

}

void CQQEpollTcpServer::OnReadData(int fd)
{
	CTcpSocket client(fd);

    int readBytes = client.Readn(m_packet->getData(), m_packet->getSize());
    if (readBytes == -1)
    {
        printf("client[%d]connect closed\n", fd);
        OnClientClose(fd);
    }
    else if ((unsigned)readBytes != m_packet->getSize())
    {
        printf("client[%d] receive error\n", fd);
        OnClientClose(fd);
    }
    else
    {
    	OnResetTime(fd);
    	m_packet->unpack();
			m_packet->getHeader()->fd = fd; //更新客户端fd
			m_packet->pack(); //重新打包回去
    	switch(m_packet->getHeader()->funid)
    	{
      case 0x08: //心跳包
		 { 
                   static int i=0;
		  re_heart* pheart = (re_heart *)(m_packet->getBody());
    		  printf("%d  client[%d]->FrontServer: is_connecting ret= %d\n",i,fd,pheart->ret);
		  	if(i>10)
			{
			//    OnClientClose(fd);
			    //i=0;
			}
			i++;
				  return;
		}
    	case 0x01: //login
    		{
    		requesrt_login *plogin = (requesrt_login*) (m_packet->getBody());
        	printf("client[%d] recv:%s:%s\n", fd, plogin->user, plogin->pwd);
    		break;
				}
    	case 0x03: //type
    		{

    		client_type *mytype = (client_type *) (m_packet->getBody());
        	printf("client[%d] type_id:%d\n", fd, mytype->type);
    		break;
				}
    	}
			    	
    	m_recvFIFO.write(m_packet->getData());

		/*
		int writeBytes = client.Writen(m_packet->getData(), m_packet->getSize());
		if (writeBytes == -1)
		{
			printf("client[%d]connect closed\n", fd);
			OnClientClose(fd);
		}
		else if ((unsigned)writeBytes != m_packet->getSize())
		{
			printf("client[%d] send error\n", fd);
			OnClientClose(fd);
		}*/
    }
    //printf("Client[%d] read event\n", fd);
}

void CQQEpollTcpServer::OnTimeMonit(int Sec, int MaxTime)
{
	MP_CLIENT::iterator it = m_mclients.begin();

	for (; it != m_mclients.end(); it++)
	{
		if (it->second.delay+Sec >= MaxTime)
		{
			printf("time %d || %d\n",it->second.delay+Sec,MaxTime);
			//OnClientClose(it->first);	
						//删除信息
		}
		else
		{
			m_mutex.lock();
			it->second.delay += Sec;

			m_mutex.unlock();
		}
	}
}

bool CQQEpollTcpServer::OnResetTime(int fd)
{
	MP_CLIENT::iterator it = m_mclients.begin();
	
	it = m_mclients.find(fd);
	if (it != m_mclients.end())
	{
		m_mutex.lock();
		it->second.delay = 0;
		m_mutex.unlock();
		return true;
	}
	
	return false;
}

