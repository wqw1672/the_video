#ifndef __QQSEND_THREAD_H_
#define __QQSEND_THREAD_H_

#include "PThreadCond.h"
#include "PThreadBase.h"
#include "TcpSocket.h"
#include "QQPacket.h"
#include "ShmFIFO.h"


//QQ回包的线程
class CQQSendThread : public CThreadBase
{
public:

	CQQSendThread();
	~CQQSendThread();

	int run(void);
private:
	CShmFIFO &m_fifo;
	CQQPacket m_packet;
};


#endif /*  __QQSEND_THREAD_H_ */

