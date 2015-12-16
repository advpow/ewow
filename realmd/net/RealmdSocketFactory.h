///////////////////////////////////////////////////////////////////////////////
// File:        RealmdSocketFactory.h
// Author:      671643387@qq.com
// Date:        2015年12月16日 下午2:01:34
// Description:
///////////////////////////////////////////////////////////////////////////////

#ifndef REALMD_NET_REALMDSOCKETFACTORY_H_
#define REALMD_NET_REALMDSOCKETFACTORY_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif

#include "net/SocketManager.h"

class RealmdSocketFactory : public SocketFactory
{
public:
    RealmdSocketFactory(void);
    virtual ~RealmdSocketFactory(void);

public:
    virtual SocketPtr create(void);
};

#endif /* REALMD_NET_REALMDSOCKETFACTORY_H_ */
