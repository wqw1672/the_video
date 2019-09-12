#include "PThreadBase.h"
#include <sys/prctl.h>
#include <sys/syscall.h>

CThreadBase::CThreadBase()
	:m_bRun(false),m_bDetach(true)
{
	m_name = "Thread";
}

CThreadBase::CThreadBase(bool detachable)
	:m_bRun(false),m_bDetach(detachable)
{
	m_name = "Thread";
}

CThreadBase::~CThreadBase()
{
	join();
}

void CThreadBase::start()
{
	if (m_bRun == false)	//m_bRunΪ�߳��Ƿ��Ѿ���������
	{
		if( pthread_create(&m_tid,NULL,rountine,(void *)this) != 0)
		{
			perror("create thread error : ");
		}
	}
}

void CThreadBase::join()
{
	m_bRun = false; //exit thread
	if(m_bDetach == false)
	{
		if(pthread_join(m_tid, NULL) != 0)
		{
			perror("pthread_join failed : ");
		}	
	}
	m_tid = -1;
}

void CThreadBase::setName(const char *acName)
{
	if(!acName)
		m_name = "Thread";
	else
		m_name = acName;
	
}

void CThreadBase::setName(void)
{
	char acThreadName[128] = {0};
	snprintf(acThreadName, sizeof(acThreadName), "%s(%ld)", m_name, syscall(SYS_gettid));	
	prctl(PR_SET_NAME, acThreadName, 0, 0, 0);	//�����߳�������
}

void * CThreadBase::rountine(void *arg)
{	
	CThreadBase *thr = (CThreadBase *)arg;
	if( thr->m_bDetach )
	{
		pthread_detach(pthread_self());   //�Է����̣߳����õ���join�����ȴ�
	}
	thr->setName();
	thr->m_bRun = true;
	thr->run();     //�û��Զ���Ĵ�����
	thr->m_bRun = false;
	
    pthread_exit(NULL);
}


