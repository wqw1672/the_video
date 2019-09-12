#ifndef PTHREADBASE_H_
#define PTHREADBASE_H_

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

class CThreadBase
{
public:
	CThreadBase();  //��ʱ��û�д����̣߳�ֻ�������
	CThreadBase(bool detachable);
	virtual ~CThreadBase();
	
	virtual int run() = 0;  //�û��Զ��崦���������������б���ʵ��
	void start(); //�����̲߳������߳�
	void join(); //�������̻߳���
	void setName(const char *acName);
	
protected:
	pthread_t m_tid;  //�߳�ID
	bool m_bRun;      // ���б��
	bool m_bDetach;	  //�Ƿ����
	const char *m_name;     // �߳�����
private:
	void setName();
	static void * rountine(void *arg);   //�߳��ദ�������
};

#endif