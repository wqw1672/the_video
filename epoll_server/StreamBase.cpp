/*************************************************************************
	> File Name: StreamBase.cpp
	> Author: eric
	> Mail:
	> Created Time: 2017年11月28日 星期二 06时32分02秒
 ************************************************************************/

#include <iostream>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include "StreamBase.h"

using namespace std;

CStreamBase::CStreamBase()
{

}

CStreamBase::CStreamBase(int fd)
{
    m_fd = fd;
}

CStreamBase::~CStreamBase(void)
{

}

void CStreamBase::SetFd(int fd)
{
    m_fd = fd;
}

int CStreamBase::GetFd() const
{
    return m_fd;
}

bool CStreamBase::SetNonBlock(bool flag)
{
	int opts;
	opts = fcntl(m_fd, F_GETFL);/*fcntl函数有5种功能：

　　　　 1.复制一个现有的描述符（cmd=F_DUPFD）.

　　       2.获得／设置文件描述符标记(cmd=F_GETFD或F_SETFD).

            3.获得／设置文件状态标记(cmd=F_GETFL或F_SETFL).

            4.获得／设置异步I/O所有权(cmd=F_GETOWN或F_SETOWN).

            5.获得／设置记录锁(cmd=F_GETLK,F_SETLK或F_SETLKW).*/
	if(opts < 0)
	{
		perror("fcntl(sock,GETFL)");
		return false;
	}
	if(flag == true)
		opts |= O_NONBLOCK;
	else
		opts &= ~O_NONBLOCK;
	if(fcntl(m_fd, F_SETFL, opts) < 0)
	{
		perror("fcntl(sock,SETFL,opts)");
		return false;
	}
	
	return true;
}

int CStreamBase::Read(char *buf, const int buf_len)
{
    int rsize = read(m_fd, buf, buf_len);
    if (rsize < 0)
    {
        perror("read");
    }
    return rsize;
}

int CStreamBase::Read(char *buf, const int buf_len, struct timeval *tv)
{
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(m_fd, &rfds);
    switch (select(m_fd+1, &rfds, NULL, NULL, tv))
    {
    case 0:
        printf("select timeout!\n");
        return 0;
    case -1:
        perror("select");
        return -1;
    default:
        if (FD_ISSET(m_fd, &rfds))
            return Read(buf, buf_len);
        else
            return -1;
    }
    return 0;
}

int CStreamBase::Read(char *buf, const int buf_len, CHostAddress &remote_addr)
{
    int rsize = recvfrom(m_fd, buf, buf_len, 0, remote_addr.Address(), remote_addr.LengthPtr());
    if (rsize < 0)
    {
        perror("recvfrom");
    }
    return rsize;
}

int CStreamBase::Read(char *buf, const int buf_len, CHostAddress &remote_addr, struct timeval *tv)
{
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(m_fd, &rfds);
    switch (select(m_fd+1, &rfds, NULL, NULL, tv))
    {
    case 0:
        printf("select timeout!\n");
        return 0;
    case -1:
        perror("select");
        return -1;
    default:
        if (FD_ISSET(m_fd, &rfds))
            return Read(buf, buf_len, remote_addr);
        else
            return -1;
    }
}

int CStreamBase::Write(const char *buf, const int buf_len)
{
    int wdsize=write(m_fd,buf,buf_len);
    if (wdsize<=0)
    {
        perror("send data error:");
    }
    return wdsize;
}

int CStreamBase::Write(const char *buf, const int buf_len, struct timeval *tv/*int timeout == -1*/)
{
    fd_set wfds;
    FD_ZERO(&wfds);
    FD_SET(m_fd, &wfds);
    switch (select(m_fd+1, NULL, &wfds, NULL, tv))
    {
    default:
        if (FD_ISSET(m_fd, &wfds))
            return Write(buf,buf_len);
        else
            return -1;
    case 0:
        printf("select timeout!\n");
        return 0;
    case -1:
        perror("select");
        return -1;
    }
}

int CStreamBase::Write(const char *buf, const int buf_len, CHostAddress remote_addr)
{
    int wdsize=sendto(m_fd,buf,buf_len,0,remote_addr.Address(),remote_addr.Length());
    if (wdsize<=0)
    {
        perror("sendto data error:");
    }
    return wdsize;
}

int CStreamBase::Write(const char *buf, const int buf_len, CHostAddress remote_addr, struct timeval *tv)
{
    fd_set wfds;
    FD_ZERO(&wfds);
    FD_SET(m_fd, &wfds);
    switch (select(m_fd+1, NULL, &wfds, NULL, tv))
    {
    default:
        if (FD_ISSET(m_fd, &wfds))
            return Write(buf,buf_len, remote_addr);
        else
            return -1;
    case 0:
        printf("select timeout!\n");
        return 0;
    case -1:
        perror("select");
        return -1;
    }
}


int CStreamBase::Readn(char *buf, const int buf_len)
{
    int nLeft = buf_len;//Ê£ÏÂ¶àÉÙÒª¶Á
    int nRead = 0;
    char *pBuf = (char *)buf;

    while (nLeft > 0)
    {
        nRead = ::read(m_fd, pBuf, nLeft);
        if (nRead < 0)
        {
            if (errno == EINTR)  //Èç¹û¶ÁÈ¡²Ù×÷ÊÇ±»ÐÅºÅŽò¶ÏÁË, ÔòËµÃ÷»¹¿ÉÒÔŒÌÐø¶Á
                continue;
            else             //·ñÔòŸÍÊÇÆäËûŽíÎó
                return -1;
        }
        else if (nRead == 0)     //¶Ô·œ¹Ø±ÕÁ¬œÓ
            return buf_len-nLeft; //ÊµŒÊ¶ÁµÄ³€¶È
        else  //Õý³£¶ÁÈ¡
        {
            nLeft -= nRead;
            pBuf += nRead;
        }
    }
    return buf_len;
}

int CStreamBase::Writen(const char *buf, const int buf_len)
{
    int nLeft = buf_len;
    int nWritten = 0;
    char *pBuf = (char *)buf;

    while (nLeft > 0)
    {
        nWritten = ::write(m_fd, pBuf, nLeft);
        if ( nWritten < 0)
        {
            if (errno == EINTR)  //Èç¹ûÐŽÈë²Ù×÷ÊÇ±»ÐÅºÅŽò¶ÏÁË, ÔòËµÃ÷»¹¿ÉÒÔŒÌÐøÐŽÈë
                continue;
            else                 //·ñÔòŸÍÊÇÆäËûŽíÎó
                return -1;
        }
        else if (nWritten == 0) //Èç¹û ==0ÔòËµÃ÷ÊÇÊ²ÃŽÒ²Ã»ÐŽÈë, ¿ÉÒÔŒÌÐøÐŽ
            continue;
        else   //Õý³£ÐŽÈë
        {
            nLeft -= nWritten;
            pBuf += nWritten;
        }
    }
    return buf_len;
}

