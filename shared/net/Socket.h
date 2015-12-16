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
#include <boost/shared_ptr.hpp>
#include <event2/event.h>
#include "Typedef.h"
#include "util/CircleBuffer.h"

class SocketManager;
class SocketWorker;

class Socket : public boost::noncopyable
{
public:
    Socket(void);
    virtual ~Socket(void);

    virtual bool open(ev_uintptr_t fd);
    virtual void close(void);

    int getId(void) const { return id_; }

    std::size_t size(void) { return recvBuf_.size(); }
    std::size_t recv(BYTE_t *buf, std::size_t len) { return recvBuf_.read(buf, len); }
    std::size_t peek(BYTE_t *buf, std::size_t len) { return recvBuf_.peek(buf, len); }
    std::size_t skip(std::size_t len) { return recvBuf_.skip(len); }

public:
    static bool setnonblocking(ev_uintptr_t fd, bool on = true);
    static void close(ev_uintptr_t fd);

protected:
    virtual void onRead(void);
    virtual void onClose(void);

private:
    int _recv(BYTE_t *buf, int len);
    int _send(const BYTE_t *buf, int len);

private:
    friend class SocketManager;

    void _doClose(void);
    void _doRecv(void);
    static void _Recv(evutil_socket_t sock, short event, void* arg);

private:
    friend class SocketWorker;

    int id_;
    int ownerId_;
    SocketManager* sockMgr_;
    ev_uintptr_t fd_;
    struct event *evRecv_;
    struct event *evSend_;
    struct event_base *evBase_;
    struct sockaddr_in addr_;
    CircleBuffer recvBuf_;
};
typedef boost::shared_ptr<Socket> SocketPtr;

#endif /* SHARED_NET_SOCKET_H_ */
