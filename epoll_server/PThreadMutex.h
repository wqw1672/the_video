#ifndef PTHREADMUTEX_H
#define PTHREADMUTEX_H

#include <stdio.h>
#include <pthread.h>

class CPThreadMutex
{
public:
	CPThreadMutex();
	virtual ~CPThreadMutex();
	bool lock();
	bool unlock();
	bool trylock();
protected:
	pthread_mutex_t m_mutex;
};

#endif