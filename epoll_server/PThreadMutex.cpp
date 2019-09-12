#include "PThreadMutex.h"
#include <errno.h>
CPThreadMutex::CPThreadMutex()
{
	if(pthread_mutex_init(&m_mutex,NULL)!=0)
	{
		perror("pthread mutex init error");
	}
}

CPThreadMutex::~CPThreadMutex()
{
 	if(pthread_mutex_destroy(&m_mutex)!=0)
 	{
 		perror("pthread mutex destroy error");
 	}
}

bool CPThreadMutex::lock()
{
	int ret=pthread_mutex_lock(&m_mutex);
	if (ret !=0)
	{
		perror("pthread mutex lock error");
	}
	return ret==0 ? true: false;
}

bool CPThreadMutex::unlock()
{
	int ret=pthread_mutex_unlock(&m_mutex);
	if (ret !=0)
	{
		perror("pthread mutex unlock error");
	}
	return ret==0 ? true: false;
}

bool CPThreadMutex::trylock()
{
	int ret=pthread_mutex_trylock(&m_mutex);
	if (ret !=0)
	{
		perror("pthread mutex trylock error");
	}
	return ret==0 ? true: false;
}
