#include "PThreadPool.h"
#include <pthread.h>
#include <sys/prctl.h>
#include <sys/syscall.h>


#define THRADPOOL_DBG

CThreadPool::CThreadPool(unsigned int max/* =100 */,unsigned int min/* =10 */,unsigned int wait/* =60 */)
{
	m_count = 0;
	m_waitcount = 0;
	m_mincount = min;
	m_maxcount = max;
	m_waitsec = wait;
	m_bStop = false;
	m_cond.lock();
	for (unsigned i = 0; i < m_mincount; i++)
	{
		createThread();
	}
	m_cond.unlock();
}

CThreadPool::~CThreadPool()
{
	destroy();
}

void CThreadPool::addTask(CTask *task)
{
	if (m_bStop)	//是否销毁线程池
	{
		return;
	}
	
	m_taskCond.lock();
#ifdef THRADPOOL_DBG	
	printf("Add task %p\n", task);
#endif		
	m_taskList.push_back(task);
	m_taskCond.unlock();
	
	m_cond.lock();
	if(m_waitcount)
	{
		m_cond.signal();
	}
	else if(m_count < m_maxcount)
	{
		createThread();
	}
	m_cond.unlock();
}

void CThreadPool::createThread()
{
	pthread_t tid;
	int ret = pthread_create(&tid, NULL, taskThread, (void *)this);
	if (ret < 0)
	{
		perror("pthread create error");
	}
	else
	{
		m_count++;
#ifdef THRADPOOL_DBG
		printf("New pthread[0x%x], count=%d\n", (int)tid, m_count);
#endif			
	}
}

void CThreadPool::start(unsigned NumCreate)
{
	m_cond.lock();
	//printf("%d\n", m_maxcount);
	for (unsigned i = 0; i < NumCreate && i < m_maxcount-2; i++)
	{
		createThread();
	}
	m_cond.unlock();
}

void CThreadPool::setTaskName(char* acName)
{
	char acThreadName[128] = {0};
	if(NULL != acName)
		snprintf(acThreadName, sizeof(acThreadName), "%s(%ld)", acName, syscall(SYS_gettid));	
	else
		snprintf(acThreadName, sizeof(acThreadName), "taskThread(%ld)", syscall(SYS_gettid));	
	prctl(PR_SET_NAME, acThreadName, 0, 0, 0);	
}


// Work thread
void * CThreadPool::taskThread(void *arg)
{
	pthread_detach(pthread_self()); // detach itself when exit.
	CThreadPool *pool=(CThreadPool *)arg;
	pool->setTaskName(NULL); //set task thead name
	while(1)
	{
		//check task, if hasn't task, this thread turns to idle thread.
		pool->m_cond.lock();
		if (pool->m_taskList.empty()) 
		{
			if(pool->m_bStop) 
			{
				pool->m_count--; // Sub thread nums.
#ifdef THRADPOOL_DBG	
				printf("thread 0x%x is stop. count = %d\n", (int)pthread_self(), pool->m_count);
#endif
				pool->m_cond.unlock();
				pthread_exit(NULL);
			}
			
			pool->m_waitcount++;
			bool bSignal = pool->m_cond.timewait(pool->m_waitsec); //wait new task and signal.
			pool->m_waitcount--;
			
			// Delete some thread when hasn't task and thread nums above the min nums.
			if (!bSignal && pool->m_count > pool->m_mincount) 
			{
				pool->m_count--; // Sub thread nums.
#ifdef THRADPOOL_DBG
				printf("thread 0x%x is wait timedout.exit. count = %d\n", (int)pthread_self(), pool->m_count);
#endif	
				pool->m_cond.unlock();
				pthread_exit(NULL);
			}
		}
		pool->m_cond.unlock();
		
		if (!pool->m_taskList.empty())
		{
			//Get one task and do it.
			pool->m_taskCond.lock();
			CTask *t = pool->m_taskList.front();
			pool->m_taskList.pop_front(); // Remove from task list.
			pool->m_taskCond.unlock(); //unlock before run task.

			t->run(); // do task.
			delete t;
		}
	}
	pthread_exit(NULL);
}

// 
void CThreadPool::destroy()
{
	printf("destroy?\n");
	
#if 0// wait all task deal end.
	list_task::iterator it = m_taskList.begin();
	for (; it! = m_taskList.end(); it++)
	{
		CTask *t = *it;
		delete t;

		t = NULL;
	}
	m_taskList.clear();
#endif	
	
	m_bStop = true;
	while (m_count > 0)
	{
		m_cond.lock();
		m_cond.broadcast();
		m_cond.unlock();

		// Wait all task run over.
		sleep(1);
	}
}



