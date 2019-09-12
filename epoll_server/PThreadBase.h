#ifndef PTHREADBASE_H_
#define PTHREADBASE_H_

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

class CThreadBase
{
public:
	CThreadBase();  //此时还没有创建线程，只有类对象
	CThreadBase(bool detachable);
	virtual ~CThreadBase();
	
	virtual int run() = 0;  //用户自定义处理函数，在派生类中必须实现
	void start(); //创建线程并启动线程
	void join(); //不分离线程回收
	void setName(const char *acName);
	
protected:
	pthread_t m_tid;  //线程ID
	bool m_bRun;      // 运行标记
	bool m_bDetach;	  //是否分离
	const char *m_name;     // 线程名字
private:
	void setName();
	static void * rountine(void *arg);   //线程类处理函数框架
};

#endif