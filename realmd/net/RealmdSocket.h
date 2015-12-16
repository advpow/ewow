///////////////////////////////////////////////////////////////////////////////
// File:        RealmdSocket.h
// Author:      671643387@qq.com
// Date:        2015年12月16日 下午2:01:53
// Description:
///////////////////////////////////////////////////////////////////////////////

#ifndef REALMD_NET_REALMDSOCKET_H_
#define REALMD_NET_REALMDSOCKET_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif

#include "net/Socket.h"

class RealmdSocket : public Socket
{
public:
    RealmdSocket(void);
    virtual ~RealmdSocket(void);

protected:
    virtual void onRead(void);
    virtual void onClose(void);
};

#endif /* REALMD_NET_REALMDSOCKET_H_ */
