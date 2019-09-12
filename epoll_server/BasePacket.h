/*************************************************************************
	> File Name: BasePacket.h
	> Author: eric
	> Mail: 
	> Created Time: 2017年11月24日 星期五 22时33分02秒
 ************************************************************************/

#ifndef _BASE_PACKET_H
#define _BASE_PACKET_H

#include <iostream>

class CBasePacket
{
public:
    CBasePacket(int size);
    CBasePacket(const CBasePacket &packet);
    virtual ~CBasePacket();
    virtual bool pack()=0;
    virtual bool unpack()=0;
    CBasePacket & operator = (const CBasePacket &packet);
    char *getData(); //获取的数据
    unsigned int getSize(); //获取包长度
   
protected:
    char    *m_data;
    unsigned m_size;
};


#endif
