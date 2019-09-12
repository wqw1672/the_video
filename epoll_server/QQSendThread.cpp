#include "QQSendThread.h"

extern CShmFIFO  m_sendFIFO;
extern CShmFIFO  m_recvFIFO;

CQQSendThread::CQQSendThread()
	:m_fifo(m_sendFIFO)
{
	//m_fifo.open(KEY_FIFO_SEND, m_packet.getSize(), 100);
}

CQQSendThread::~CQQSendThread()
{
}

int CQQSendThread::run(void)
{
	CTcpSocket client;
	
	//thread while(1)
	while(this->m_bRun)
	{
		//1. read from shared memory fifo
		m_fifo.read(m_packet.getData());

		//2. write to client
		m_packet.unpack();
		client.SetSocket(m_packet.getHeader()->fd);
		client.Writen(m_packet.getData(), m_packet.getSize());
	}

	return 0;
}

