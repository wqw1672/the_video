#ifndef EPOLLBASE_H_
#define EPOLLBASE_H_

#include <sys/epoll.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
/*
 1.��ͬ�������ȡ�ɻ��࣬����
	���� �������ģ�ɾ�����ö�ʱ���ȴ���epoll_wait��
 2.�ɱ�
	int nevent  = epoll_wait֮��Ĵ���
	Run
 3.�̶��Ĳ���
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
