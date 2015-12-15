///////////////////////////////////////////////////////////////////////////////
// File:        SocketWorker.h
// Author:      671643387@qq.com
// Date:        2015年12月15日 上午10:48:18
// Description:
///////////////////////////////////////////////////////////////////////////////

#ifndef SHARED_NET_SOCKETWORKER_H_
#define SHARED_NET_SOCKETWORKER_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif

#include <cstdint>
#include <boost/noncopyable.hpp>
#include <boost/atomic.hpp>
#include <boost/thread.hpp>
#include "net/Socket.h"

class SocketManager;

class SocketWorker : public boost::noncopyable
{
public:
    SocketWorker(void);
    ~SocketWorker(void);

    bool open(void);
    void stop(void) { event_base_loopbreak(evBase_); }
    void join(void) { thread_->join(); }

    void setId(int id) { id_ = id; }
    bool addSocket(SocketPtr& s);

    void decLoad(void) { --load_; }
    std::uint32_t getLoad(void) { return load_; }

private:
    void _run(void);
    static void _Tick(evutil_socket_t sock, short event, void* arg);

private:
    friend class SocketManager;

    int id_;
    struct event_base *evBase_;
    struct event *evTick_;
    boost::atomic_uint32_t load_;
    boost::shared_ptr<boost::thread> thread_;
};

typedef boost::shared_ptr<SocketWorker> SocketWorkerPtr;

#endif /* SHARED_NET_SOCKETWORKER_H_ */
