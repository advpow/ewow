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

#include <vector>
#include <deque>
#include <boost/noncopyable.hpp>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/lock_guard.hpp>
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

    void send(const BYTE_t *buf, std::size_t len);

public:
    static bool setnonblocking(ev_uintptr_t fd, bool on = true);
    static void close(ev_uintptr_t fd);

protected:
    virtual void onRead(void);
    virtual void onClose(void);

private:
    int _recv(BYTE_t *buf, int len);
    int _send(const BYTE_t *buf, int len);

    bool _notifySend(void);

private:
    friend class SocketManager;

    void _doClose(void);
    void _doRecv(void);
    void _doSend(void);
    static void _Recv(evutil_socket_t sock, short event, void* arg);
    static void _Send(evutil_socket_t sock, short event, void* arg);

    class SendBuffer
    {
    public:
        SendBuffer(const BYTE_t *data, std::size_t len)
            : data_(len)
            , rpos_(0)
        {
            memcpy(&data_[0], data, len);
        }

        const BYTE_t *contents(void) const
        {
            return &data_[rpos_];
        }

        std::size_t size(void) const
        {
            return (data_.size() - rpos_);
        }

        std::size_t rpos(std::size_t pos)
        {
            rpos_ += pos;
            return rpos_;
        }

    private:
        std::vector<BYTE_t> data_;
        std::size_t rpos_;
    };

    typedef boost::shared_ptr<SendBuffer>   SendBufferPtr;
    typedef std::deque<SendBufferPtr>       SendQueue;

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
    SendQueue sendQueue_;
    boost::mutex sendQueueLOCK_;
    bool sendPending_;
};
typedef boost::shared_ptr<Socket> SocketPtr;

#endif /* SHARED_NET_SOCKET_H_ */
