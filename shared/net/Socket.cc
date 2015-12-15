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

Socket::Socket(void)
: id_(-1)
, fd_(-1)
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
#else
    ::close(fd);
#endif
}