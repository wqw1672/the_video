#ifndef PTHREADCOND_H
#define PTHREADCOND_H

#include "PThreadMutex.h"

class CPThreadCond :public CPThreadMutex
{
public:
	CPThreadCond();
	virtual ~CPThreadCond();
	bool wait();
	bool timewait(unsigned int sec);
	bool signal();
	bool broadcast();
	bool isTimeout();
private:
	pthread_cond_t m_cond;
	bool m_btimeout;
};





#endif