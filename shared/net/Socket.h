///////////////////////////////////////////////////////////////////////////////
// File:        Socket.h
// Author:      671643387@qq.com
// Date:        2015年12月15日 上午10:47:19
// Description:
///////////////////////////////////////////////////////////////////////////////

#ifndef SHARED_NET_SOCKET_H_
#define SHARED_NET_SOCKET_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif

#include <boost/noncopyable.hpp>
#include <boost/function.hpp>
#include <event2/event.h>
#include "Typedef.h"

class Socket : public boost::noncopyable
{
public:
    Socket(void);
    virtual ~Socket(void);

    virtual bool open(ev_uintptr_t fd);
    virtual void close(void);

public:
    static bool setnonblocking(ev_uintptr_t fd, bool on = true);
    static void close(ev_uintptr_t fd);

protected:
    int id_;
    ev_uintptr_t fd_;
    struct sockaddr_in addr_;
};

#endif /* SHARED_NET_SOCKET_H_ */
