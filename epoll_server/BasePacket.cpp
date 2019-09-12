/*************************************************************************
	> File Name: BasePacket.cpp
	> Author: eric
	> Mail: 
	> Created Time: 2017年11月24日 星期五 22时38分46秒
 ************************************************************************/

#include <iostream>
#include <unistd.h>
#include <string.h>
#include "BasePacket.h"

using namespace std;

CBasePacket::CBasePacket(int size)
:m_size(size)
{
    m_data = new char[m_size];
}

CBasePacket::CBasePacket(const CBasePacket &packet)
{
	m_size = packet.m_size;
	m_data = new char[packet.m_size];
	memcpy(m_data, packet.m_data, packet.m_size);
}

CBasePacket::~CBasePacket()
{
    delete []m_data;
}

char *CBasePacket::getData()
{
    return m_data;
}

unsigned int CBasePacket::getSize()
{
    return m_size;
}

CBasePacket & CBasePacket::operator =(const CBasePacket &packet)
{
	if(m_size != packet.m_size)
	{
		delete []m_data;

		m_size = packet.m_size;
		m_data = new char[packet.m_size];
	}
	memcpy(m_data, packet.m_data, packet.m_size);
	return *this;
}

