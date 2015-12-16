///////////////////////////////////////////////////////////////////////////////
// File:        RealmdSocket.cc
// Author:      671643387@qq.com
// Date:        2015年12月16日 下午2:01:59
// Description:
///////////////////////////////////////////////////////////////////////////////

#include "net/RealmdSocket.h"

RealmdSocket::RealmdSocket(void)
{

}

RealmdSocket::~RealmdSocket(void)
{

}

///////////////////////////////////////////////////////////////////////////////

void RealmdSocket::onRead(void)
{
    printf("RealmdSocket::onRead, %d, %d bytes in buffer\n", getId(),
        size());
}

void RealmdSocket::onClose(void)
{

}