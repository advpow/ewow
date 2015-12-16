///////////////////////////////////////////////////////////////////////////////
// File:        RealmdSocketFactory.cc
// Author:      671643387@qq.com
// Date:        2015年12月16日 下午2:01:41
// Description:
///////////////////////////////////////////////////////////////////////////////

#include "net/RealmdSocketFactory.h"
#include "net/RealmdSocket.h"

RealmdSocketFactory::RealmdSocketFactory(void)
{

}

RealmdSocketFactory::~RealmdSocketFactory(void)
{

}

SocketPtr RealmdSocketFactory::create(void)
{
    return SocketPtr(new RealmdSocket());
}