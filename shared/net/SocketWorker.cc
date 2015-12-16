///////////////////////////////////////////////////////////////////////////////
// File:        SocketWorker.cc
// Author:      671643387@qq.com
// Date:        2015年12月15日 上午10:48:25
// Description:
///////////////////////////////////////////////////////////////////////////////

#include <boost/bind.hpp>
#include "net/SocketWorker.h"

SocketWorker::SocketWorker(void)
: id_(-1)
, evBase_(NULL)
, evTick_(NULL)
, load_(0)
{

}

SocketWorker::~SocketWorker(void)
{
    if (evTick_)
        event_free(evTick_);
    if (evBase_)
        event_base_free(evBase_);
}

bool SocketWorker::open(void)
{
    struct timeval tv = { 1, 0 };
    evBase_ = event_base_new();
    if (evBase_)
    {
        evTick_ = event_new(evBase_, -1, 
            EV_TIMEOUT | EV_PERSIST, &SocketWorker::_Tick, this);
        if (evTick_)
        {
            if (0 == event_add(evTick_, &tv))
            {
                if (start())
                    return true;
            }

            event_free(evTick_);
            evTick_ = NULL;
        }
        event_base_free(evBase_);
        evBase_ = NULL;
    }
    return false;
}

bool SocketWorker::addSocket(SocketPtr& s)
{
    assert(s);
    s->evRecv_ = event_new(evBase_, s->fd_, 
        EV_READ | EV_PERSIST, &Socket::_Recv, s.get());
    if (s->evRecv_)
    {
        if (0 == event_add(s->evRecv_, NULL))
        {
            ++load_;
            s->ownerId_ = id_;
            s->evBase_ = evBase_;
            return true;
        }
        event_free(s->evRecv_);
        s->evRecv_ = NULL;
    }
    return false;
}

void SocketWorker::run(void)
{
    event_base_dispatch(evBase_);
}

void SocketWorker::_Tick(evutil_socket_t sock, short event, void* arg)
{
    // DO NOTHING
}