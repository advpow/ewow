///////////////////////////////////////////////////////////////////////////////
// File:        SocketManager.h
// Author:      671643387@qq.com
// Date:        2015年12月15日 上午10:47:38
// Description:
///////////////////////////////////////////////////////////////////////////////

#ifndef SHARED_NET_SOCKETMANAGER_H_
#define SHARED_NET_SOCKETMANAGER_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif

#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/unordered_map.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/lock_guard.hpp>
#include "net/Socket.h"
#include "net/SocketWorker.h"

class SocketFactory : public boost::noncopyable
{
public:
    SocketFactory(void){}
    virtual ~SocketFactory(void){}

    virtual SocketPtr create(void)
    {
        return SocketPtr(new Socket());
    }
};
typedef boost::shared_ptr<SocketFactory>    SocketFactoryPtr;


class SocketManager : public boost::noncopyable
{
public:
    SocketManager(SocketFactoryPtr factory = SocketFactoryPtr(), bool noThread = false);
    ~SocketManager(void);

    bool open(const char *addr, int port,
        int backlog = 1024, int workers = 0);
    void close(void);
    void join(void);

    void setSocketFactory(SocketFactoryPtr factory);

private:
    friend class Socket;

    SocketWorker *findFreeWorker(void);

    void _closeSocket(Socket *s);
    void _doAccept(void);
    static void _Accept(evutil_socket_t sock, short event, void* arg);

private:
    typedef boost::unordered_map<int, SocketPtr>    SocketMap;
    typedef std::vector<SocketWorkerPtr>            WorkerArray;
    enum State
    {
        READY,
        RUNNING,
        EXITING,
        EXIT
    };

private:
    int sidGen_;
    bool noThread_;
    ev_uintptr_t sListen_;
    struct event *evListen_;
    State state_;
    SocketFactoryPtr sFactory_;
    WorkerArray workers_;
    SocketMap sockets_;
    boost::mutex socketsLOCK_;
};

#endif /* SHARED_NET_SOCKETMANAGER_H_ */
