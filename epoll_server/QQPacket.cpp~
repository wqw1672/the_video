/*************************************************************************
	> File Name: QQPacke.cpp
	> Author: eric
	> Mail: 
	> Created Time: 2017年11月24日 星期五 23时41分03秒
 ************************************************************************/

#include <iostream>
#include <string.h>
#include "QQPacket.h"
#include <stdio.h>

using namespace std;

CQQPacket::CQQPacket()
	:CBasePacket(sizeof(packet_all_st))
{
    memset(&m_head, 0, sizeof(m_head));
}

CQQPacket::~CQQPacket()
{
    
}

bool CQQPacket::pack()
{
   memcpy(m_data, &this->m_head, sizeof(packet_header_t));
   memcpy(m_data+sizeof(packet_header_t), this->m_Body, sizeof(m_Body));
   return true;
}

bool CQQPacket::unpack()
{
   memcpy(&this->m_head, m_data, sizeof(packet_header_t));     
   memcpy(this->m_Body, m_data+sizeof(packet_header_t), sizeof(m_Body));    
   return true;
}
//心跳包
bool CQQPacket::keeppack()
{
	  m_head.funid = 0x08;
	  m_head.optid = 1;
	  m_head.usrlenth = sizeof(m_Body);
	  int syn = 0;
	  
	  return this->pack();
}
bool CQQPacket::Login()
{
    //填充包头
    m_head.funid = 0x01; //login
    m_head.optid = 0x00; //登录请求
    m_head.usrlenth = sizeof(m_Body);
    m_head.syn = 0;
    
    //填充包体
    requesrt_login *body = (requesrt_login*)m_Body;
    strcpy(body->user, "111111");
    strcpy(body->pwd, "111111");

    return this->pack();
}

//登陆应答包
bool CQQPacket::replyLogin(char *name,int state)
{
	m_head.funid = 0x01;
	m_head.optid = 1;
	m_head.usrlenth = sizeof(m_Body);
	int syn = 0;
	
	strcpy(((reply_login *)m_Body)->usr,name);
	((reply_login *)m_Body)->login_reply = state;
	
	return this->pack();
}

bool CQQPacket::retype(int type,int type_id,int the_choice)
{
       switch(the_choice)
	{
		case 1:
		      m_head.funid = 0x03;
		break;
		case 2:
		      m_head.funid = 0x04;
		break;
		case 3:
		      m_head.funid = 0x05;
		break;
		case 4:
		      m_head.funid = 0x06;
		break;
	}

	m_head.optid = 1;
	m_head.usrlenth = sizeof(m_Body);
	int syn = 0;
	
	((reply_type *)m_Body)->arrayvideoid[type]=type_id;
	type++;
	printf("the number %d   the id =%d\n",type,((reply_type *)m_Body)->arrayvideoid[type]);
	for(;type<10;type++)
	{
	((reply_type *)m_Body)->arrayvideoid[type]=0;
	}
	return this->pack();
}

//视频列表应答包
bool CQQPacket::replyList(reply_list *video)
{
	m_head.funid = 0x05;
	m_head.optid = 1;
	m_head.usrlenth = sizeof(m_Body);
	int syn = 0;
	
	reply_list *body = (reply_list *)m_Body;
	body = (reply_list *)((char *)(m_Body + (sizeof(reply_list))));
	strcpy(body->VideoName,video->VideoName);
	return this->pack();
	//	int e = 0;
	/*for(vector<reply_list>::iterator it = video->begin(); it != video->end(); it++)
	{
		body = (reply_list *)((char *)(m_Body + (e*sizeof(reply_list))));
		printf("2222\n");
		body->VideoCount = (*it).VideoCount;
		body->VideoId = (*it).VideoId;
		body->RegionId = (*it).RegionId;
		body->VideoTime = (*it).VideoTime;
		body->TypeIds[0] = (*it).TypeIds[0];
		body->ChannelId = (*it).ChannelId;
		body->PlayCount = (*it).PlayCount;
		strcpy(body->VideoName,(*it).VideoName);
		//strcpy(body->VideoImage,(*it).VideoImage);
		//strcpy(body->VideoAddress,(*it).VideoAddress);
		e++;
		printf("e = %d\n",e);
	}*/
	//strcpy(body->VideoAddress,VideoAddress);
	
	return this->pack();
}
bool CQQPacket::replay(char *addr,int state)
{

	m_head.funid = 0x05;
	m_head.optid = 0;
	m_head.usrlenth = sizeof(m_Body);
	int syn = 0;
	
	strcpy(((reply_play *)m_Body)->updata_flowid,addr);
	strcat(((reply_play *)m_Body)->updata_flowid,".mkv");
	((reply_play *)m_Body)->state = state;
	return this->pack();

	//printf(" 发送 链接 url = %s\n 状态 =  state = %d\n",m_Body->updata_flowid,m_Body->state);
}
bool CQQPacket::the_result(int state)
{
	m_head.funid = 0x07;
	m_head.optid = 0;
	m_head.usrlenth = sizeof(m_Body);
      ((res_VideoRecord_t *)m_Body)->results=state;

	return this->pack();
}

bool CQQPacket::re_percent(int video_id,float percent)
{
	m_head.funid = 0x09;
	m_head.optid = 0;
	m_head.usrlenth = sizeof(m_Body);
      ((res_PlayHistory_t *)m_Body)->video_seek=percent;
      ((res_PlayHistory_t *)m_Body)->video_id=video_id;

	return this->pack();
}

packet_header_t* CQQPacket::getHeader(void)
{
	return &m_head;
}

void* CQQPacket::getBody(void)
{
	return m_Body;
}


