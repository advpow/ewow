﻿///////////////////////////////////////////////////////////////////////////////
// File:        Socket.cc
// Author:      671643387@qq.com
// Date:        2015年12月15日 上午10:47:25
// Description:
///////////////////////////////////////////////////////////////////////////////

#include <cassert>
#if defined(__LINUX__)
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif
#include "net/Socket.h"
#include "net/SocketManager.h"

#define SOCKET_RECV_BUFF_SIZE   0x1000

Socket::Socket(void)
: id_(-1)
, ownerId_(-1)
, sockMgr_(NULL)
, fd_(-1)
, evRecv_(NULL)
, evSend_(NULL)
, evBase_(NULL)
, recvBuf_(SOCKET_RECV_BUFF_SIZE)
, sendPending_(false)
{

}

Socket::~Socket(void)
{

}

bool Socket::open(ev_uintptr_t fd)
{
    assert(fd != -1);
    // 获取套接字远程地址
    socklen_t addrlen = sizeof(addr_);
    if (0 != ::getpeername(fd, (struct sockaddr*)&addr_, &addrlen))
        return false;
    // 设置套接字非阻塞模式
    if (!Socket::setnonblocking(fd))
        return false;
    fd_ = fd;
    return true;
}

void Socket::close(void)
{
    assert(sockMgr_);
    _doClose();
    sockMgr_->_closeSocket(this);
}

void Socket::send(const BYTE_t *buf, std::size_t len)
{
    SendBufferPtr sendBuf(new SendBuffer(buf, len));
    if (sendBuf)
    {
        {
            // 将数据包添加到发送队列末尾
            boost::lock_guard<boost::mutex> guard(sendQueueLOCK_);
            sendQueue_.push_back(sendBuf);
        }

        // 尝试发送数据
        _doSend();
        return;
    }

    close();
}

std::string Socket::getPeerAddress(void)
{
    return std::string(::inet_ntoa(addr_.sin_addr));
}

int Socket::getPeerPort(void)
{
    return int(::ntohs(addr_.sin_port));
}

bool Socket::setnonblocking(ev_uintptr_t fd, bool on /* = true */)
{
#if defined(__WINDOWS__)
    ::u_long arg = (on) ? 1 : 0;
    return (0 == ::ioctlsocket(fd, FIONBIO, &arg));
#elif defined(__LINUX__)
    int flags = ::fcntl(fd, F_GETFL, 0);
    if (on) flags |= O_NONBLOCK;
    else flags &= ~O_NONBLOCK;
    return (-1 != ::fcntl(fd, F_SETFL, flags));
#endif
}

void Socket::close(ev_uintptr_t fd)
{
    assert(fd != -1);
#if defined(__WINDOWS__)
    ::closesocket(fd);
#elif defined(__LINUX__)
    ::close(fd);
#endif
}

void Socket::onRead(void)
{
    
}

void Socket::onClose(void)
{

}

int Socket::_recv(BYTE_t *buf, int len)
{
#if defined(__WINDOWS__)
    int ret = ::recv(fd_, (char*)buf, int(len), 0);
#elif defined(__LINUX__)
    int ret = ::recv(fd_, (char*)buf, len, 0);
#endif
    if (ret == SOCKET_ERROR)
    {
#if defined(__LINUX__)
        if (errno == EWOULDBLOCK)
        {
            return SOCKET_ERROR_WOULDBLOCK;
        }
#elif defined(__WINDOWS__)
        int wsaError = WSAGetLastError();
        if (wsaError == WSAEWOULDBLOCK)
        {
            return SOCKET_ERROR_WOULDBLOCK;
        }
#endif
    }
    return ret;
}

int Socket::_send(const BYTE_t *buf, int len)
{
#if defined(__WINDOWS__)
    int ret = ::send(fd_, (const char*)buf, int(len), 0);
#elif defined(__LINUX__)
    int ret = ::send(fd_, (const char*)buf, len, 0);
#endif
    if (ret == SOCKET_ERROR)
    {
#if defined(__LINUX__)
        if (errno == EWOULDBLOCK)
        {
            return SOCKET_ERROR_WOULDBLOCK;
        }
#elif defined(__WINDOWS__)
        int wsaError = WSAGetLastError();
        if (wsaError == WSAEWOULDBLOCK)
        {
            return SOCKET_ERROR_WOULDBLOCK;
        }
#endif
    }
    return ret;
}

bool Socket::_notifySend(void)
{
    assert(evBase_);
    if (!evSend_)
    {
        evSend_ = event_new(evBase_, fd_, 
            EV_WRITE, &Socket::_Send, this);
    }

    return (evSend_ && (0 == event_add(evSend_, NULL)));
}

void Socket::_doClose(void)
{
    onClose();

    if (evRecv_)
    {
        event_free(evRecv_);
        evRecv_ = NULL;
    }

    if (evSend_)
    {
        event_free(evSend_);
        evSend_ = NULL;
    }

    if (fd_ != -1)
    {
        Socket::close(fd_);
        fd_ = -1;
    }
}

void Socket::_doRecv(void)
{
    bool fOk = true;
    while (recvBuf_.space() > 0)
    {
        BYTE_t buff[0x1000];
        std::size_t bytesToRecv = __MIN_(recvBuf_.space(), sizeof(buff));
        int nRead = _recv(buff, int(bytesToRecv));
        if (nRead > 0)
            recvBuf_.write(buff, nRead);
        else if (nRead == SOCKET_ERROR_WOULDBLOCK)
            break;
        else
        {
            fOk = false;
            break;
        }
    }
    if (!fOk)
        close();
    else
        onRead();
}

void Socket::_doSend(void)
{
    {
        // 判断是否正在发送数据包
        boost::lock_guard<boost::mutex> guard(sendPendingLOCK_);
        if (sendPending_)
            return;
        sendPending_ = true;
    }
    
    SendBuffer *sendBuf = NULL;
    {
        // 取队列头数据包
        boost::lock_guard<boost::mutex> guard(sendQueueLOCK_);
        if (!sendQueue_.empty())
            sendBuf = sendQueue_.front().get();
    }

    if (!sendBuf)
    {
        sendPending_ = false;
        return;
    }

    // 设置发送完毕事件
    if (!_notifySend())
    {
        close();
        return;
    }

    // 发送数据
    int nSend = _send(sendBuf->contents(), sendBuf->size());
    if (nSend == SOCKET_ERROR)
    {
        close();
        return;
    }

    if (nSend > 0)
        sendBuf->rpos(nSend);

    // 如果数据包数据全部发送，则将其移除
    if (sendBuf->size() <= 0)
    {
        boost::lock_guard<boost::mutex> guard(sendQueueLOCK_);
        sendQueue_.pop_front();
    }
}

void Socket::_Recv(evutil_socket_t sock, short event, void* arg)
{
    Socket *pThis = static_cast<Socket*>(arg);
    assert(pThis);
    pThis->_doRecv();
}

void Socket::_Send(evutil_socket_t sock, short event, void* arg)
{
    Socket *pThis = static_cast<Socket*>(arg);
    assert(pThis);
    {
        // 发送完毕
        boost::lock_guard<boost::mutex> guard(pThis->sendPendingLOCK_);
        pThis->sendPending_ = false;
    }
    // 发送下一个包
    pThis->_doSend();
}
