///////////////////////////////////////////////////////////////////////////////
// File:        Socket.cc
// Author:      671643387@qq.com
// Date:        2015年12月15日 上午10:47:25
// Description:
///////////////////////////////////////////////////////////////////////////////

#include <cassert>
#if defined(__LINUX__)
#include <fcntl.h>
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
{

}

Socket::~Socket(void)
{

}

bool Socket::open(ev_uintptr_t fd)
{
    assert(fd != -1);
    int addrlen = sizeof(addr_);
    if (0 != ::getpeername(fd, (struct sockaddr*)&addr_, &addrlen))
        return false;
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
    int ret = ::send(fd_, (const char*)buf, len, flags);
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

void Socket::_Recv(evutil_socket_t sock, short event, void* arg)
{
    Socket *pThis = static_cast<Socket*>(arg);
    assert(pThis);
    pThis->_doRecv();
}