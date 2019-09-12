#ifndef EPOLLBASE_H_
#define EPOLLBASE_H_

#include <sys/epoll.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
/*
 1.共同点可以提取成基类，不变
	创建 ，增，改，删，设置定时，等待（epoll_wait）
 2.可变
	int nevent  = epoll_wait之后的处理
	Run
 3.固定的操作
	start
	wait
	run
	stop
*/
class CEpollBase
{
public:
    CEpollBase(unsigned int maxEvent = 20, int timeout = -1);
    virtual ~CEpollBase();
    bool SetNonBlock(int fd, bool flag);
    bool AddEvent(int fd, unsigned int events);
    bool ModEvent(int fd, unsigned int events);
    bool DelEvent(int fd, unsigned int events);
    void SetTimeout(const int timeout);
    int  GetTimeout()const;
    int  Wait();
    void Start();
    void Stop();
protected:
    struct epoll_event *m_retEvents;
    //struct epoll_event m_retEvent[maxEvent];
    int		m_maxEvent;
    bool	m_bStop;
    int		m_timeout;
    
private:
    virtual void Run(int nevent) = 0;
    int		m_epfd;
    struct  epoll_event m_event;
    
    
};

#endif //_EPOLLBASE_H_
