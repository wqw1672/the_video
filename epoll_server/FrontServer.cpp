
#include "QQEpollTcpServer.h"

extern CShmFIFO  m_sendFIFO;
extern CShmFIFO  m_recvFIFO;

/*使用epoll模型接收多客户端数据，并应答*/
int main()
{
	
	printf("\n/*-----------------FrontServer-----------------*/\n");
  printf("FrontServer:start!\n");
  
	CHostAddress addr(NULL, 8005);
	CQQEpollTcpServer epollsever(addr);
	epollsever.Start();
	//epollsever.Stop();
	
	 //Exit
  printf("FrontServer：exit!\n");
  printf("/*--------------------------------------------*/\n\n");
  
  return 0;
}

