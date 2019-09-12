/*************************************************************************
	> File Name: HostAddress.cpp
	> Author: eric
	> Mail:
	> Created Time: 2017年11月28日 星期二 03时37分07秒
 ************************************************************************/

#include <iostream>
#include "HostAddress.h"
using namespace std;


CHostAddress::CHostAddress()
{
    memset(&m_addr, 0, sizeof(m_addr));
    m_length = sizeof(m_addr);
    m_addr.sin_family = AF_INET;
    m_addr.sin_addr.s_addr = htonl(INADDR_ANY);
}

CHostAddress::CHostAddress(const char *ip, unsigned short port)
{
    memset(&m_addr, 0, sizeof(m_addr));
    m_length = sizeof(m_addr);
    m_addr.sin_family = AF_INET;
    if (ip == NULL)
        m_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    else
        m_addr.sin_addr.s_addr = inet_addr(ip);
    m_addr.sin_port = htons(port);
}

CHostAddress::~CHostAddress()
{

}

void CHostAddress::SetIp(const char *ip)
{
    if (ip == NULL)
        m_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    else
        m_addr.sin_addr.s_addr = inet_addr(ip);
}

const char* CHostAddress::GetIp()
{
    return inet_ntoa(m_addr.sin_addr);
}

void CHostAddress::SetPort(unsigned short port)
{
    m_addr.sin_port = htons(port);
}

const unsigned short CHostAddress::GetPort()
{
    return ntohs(m_addr.sin_port);
}

struct sockaddr *CHostAddress::Address()
{
    return (struct sockaddr*)&m_addr;
}

struct sockaddr_in *CHostAddress::InAddress()
{
    return &m_addr;
}

int CHostAddress::Length()
{
    return m_length;
}

socklen_t* CHostAddress::LengthPtr()
{
    return (socklen_t*)&m_length;
}


