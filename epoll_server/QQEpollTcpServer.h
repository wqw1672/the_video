#ifndef __QQEPOLLTCPSERVER_H
#define __QQEPOLLTCPSERVER_H

#include <iostream>
#include <ext/hash_map>
#include <algorithm>
#include "EpollTcpServer.h"
#include "TcpServer.h"
#include "EpollBase.h"
#include "QQSendThread.h"

//CShmFIFO  m_sendFIFO;
//CShmFIFO  m_recvFIFO;

using namespace __gnu_cxx; //for hash_map;
using namespace std;

typedef struct qqClient_st
{
	int fd; //client fd
	int delay; //delay time. for heartbeat bag.
	CHostAddress addr; //remote_addr 

}QQCLIENT_T;

typedef hash_map<int, QQCLIENT_T> MP_CLIENT;
//typedef pair<int, QQCLIENT_T> FD_Pair;



class CQQEpollTcpServer : public CEpollTcpServer
{
public:
    CQQEpollTcpServer(CHostAddress addr, unsigned int maxEvent = 10000, int timeout = 3);
    ~CQQEpollTcpServer();
     void Start(void);
    
protected:
    void OnReadData(int fd/*, char *buf, int bufLen*/);
    void OnNewConnection();
    void OnClientClose(int fd);
    void OnTimeMonit(int Sec, int MaxTime);
    bool OnResetTime(int fd);
    void Run(int nevent);
    
private:    
    MP_CLIENT m_mclients;
    CQQPacket *m_packet;
//    CShmFIFO  m_sendFIFO;
//    CShmFIFO  m_recvFIFO;
    
    CPThreadMutex m_mutex; // for client

    CQQSendThread *m_sendThread;
};

#endif //_QQEPOLLTCPSERVER_H_

