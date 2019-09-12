#include "PThreadCond.h"
#include <sys/time.h>
#include <errno.h>

CPThreadCond::CPThreadCond()
:CPThreadMutex(),m_btimeout(false)
{
	if(pthread_cond_init(&m_cond,NULL)!=0)
	{
		perror("pthread cond init error");
	}
}

CPThreadCond::~CPThreadCond()
{
	if(pthread_cond_destroy(&m_cond)!=0)
	{
		perror("pthread cond destroy error");
	}
}

bool CPThreadCond::wait()
{
	int ret=pthread_cond_wait(&m_cond,&m_mutex);
	if (ret !=0)
	{
		perror("pthread cond wait error");
	}
	return ret==0 ? true: false;
}

bool CPThreadCond::timewait(unsigned int sec) // 
{
	struct timespec abstime;
	
	clock_gettime(CLOCK_REALTIME, &abstime);
	abstime.tv_sec += sec;
	m_btimeout=false;
	int ret=pthread_cond_timedwait(&m_cond,&m_mutex,&abstime);
	if (ret==ETIMEDOUT)
	{
		m_btimeout=true;
		return false;
	}
	else if (ret!=0)
	{
		perror("pthread cond timedwait error");
	}
	return ret==0 ? true: false;
}

bool CPThreadCond::signal()
{
	int ret=pthread_cond_signal(&m_cond);
	if (ret !=0)
	{
		perror("pthread cond signal error");
	}
	return ret==0 ? true: false;
}

bool CPThreadCond::broadcast()
{
	int ret=pthread_cond_broadcast(&m_cond);
	if (ret !=0)
	{
		perror("pthread cond broadcast error");
	}
	return ret==0 ? true: false;
}

bool CPThreadCond::isTimeout()
{
	return m_btimeout;
}