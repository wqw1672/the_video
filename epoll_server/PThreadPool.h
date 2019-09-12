#ifndef PTHREADPOOL_H
#define PTHREADPOOL_H

#include <unistd.h>
#include <list>
#include "PThreadCond.h"

using namespace std;


class CTask
{
public:
	CTask(){}
	virtual ~CTask(){}
	virtual int run()=0;
};

typedef list<CTask *> list_task;
class CThreadPool
{
public:
	CThreadPool(unsigned int max=77,unsigned int min=77,unsigned int wait=60);
	~CThreadPool();
	void addTask(CTask *task);	// Add a new task.
	void createThread();		// Create a thread.
	void start(unsigned NumCreate);	// Create a num of thread
	void destroy();				// Destroy all threads
	
//private:
	void setTaskName(char* acName);
	static void *taskThread(void *arg);// work thread
	unsigned int m_maxcount;		// max thread
	unsigned int m_mincount; 		// min thread
	unsigned int m_count;	 		// current nums
	unsigned int m_waitcount; 		// idle thread nums
	unsigned int m_waitsec;			// timeout delay.
	list_task	 m_taskList;
	CPThreadCond m_taskCond;
	CPThreadCond m_cond;
	bool m_bStop;
};





#endif
