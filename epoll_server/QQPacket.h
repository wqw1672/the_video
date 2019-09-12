/*************************************************************************
	> File Name: QQPacke.h
	> Author: eric
	> Mail: 
	> Created Time: 2017年11月24日 星期五 23时39分07秒
 ************************************************************************/

#ifndef _QQPACKE_H
#define _QQPACKE_H
#include "BasePacket.h"
#include "QQpublic.h"
#include <vector>
using namespace std; 

//typedef vector<reply_list> VRL;

class CQQPacket: public CBasePacket
{
public:
	CQQPacket();
	~CQQPacket();
	bool pack();
	bool unpack();
	bool Login();
	bool keeppack();//心跳包 
	bool replyLogin(char *name,int state);
	bool replyList(reply_list *video);
	bool replay(char *addr,int state);
	bool retype(int type,int type_id,int the_choice);
	packet_header_t* getHeader(void);
	void* getBody(void);
       bool the_result(int state);
bool re_percent(int video_id,float percent);
private:
    packet_header_t m_head;
    char m_Body[sizeof(packet_all_st) - sizeof(packet_header_t)];

};

#endif
