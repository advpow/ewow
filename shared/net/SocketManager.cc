///////////////////////////////////////////////////////////////////////////////
// File:        SocketManager.cc
// Author:      671643387@qq.com
// Date:        2015年12月15日 上午10:47:48
// Description:
///////////////////////////////////////////////////////////////////////////////

#include <cassert>
#include <memory.h>
#include "net/SocketManager.h"

#define DEFAULT_SOCKET_MAP_SIZE 512

SocketManager::SocketManager(SocketFactoryPtr factory /* = NULL */, 
    bool noThread /* = false */)
: sidGen_(0)
, noThread_(noThread)
, sListen_(-1)
, evListen_(NULL)
, state_(SocketManager::State::READY)
, sFactory_(factory)
, sockets_(DEFAULT_SOCKET_MAP_SIZE)
{

}

SocketManager::~SocketManager(void)
{

}

bool SocketManager::open(const char *addr, int port, 
    int backlog /* = 1024 */, int workers /* = 0 */)
{
    assert(addr && state_ == SocketManager::State::READY);
    bool fOk = false;

    do 
    {
        if (noThread_ && workers != 0)
            break;

        sListen_ = socket(AF_INET, SOCK_STREAM, 0);
        if (sListen_ == -1) break;

        struct sockaddr_in saddr;
        memset(&saddr, 0, sizeof(saddr));
        saddr.sin_family = AF_INET;
#if defined(__WINDOWS__)
        saddr.sin_addr.S_un.S_addr = ::inet_addr(addr);
#elif defined(__LINUX__)
        saddr.sin_addr.s_addr = ::inet_addr(addr);
#endif
        saddr.sin_port = ::htons(u_short(port));

        if (-1 == ::bind(sListen_, (const sockaddr*)&saddr, sizeof(saddr)))
            break;
        if (-1 == ::listen(sListen_, backlog))
            break;
        if (!Socket::setnonblocking(sListen_))
            break;
        
        workers += 1;
        for (int x = 0; x < workers; ++x)
        {
            SocketWorkerPtr worker(new SocketWorker());
            if (worker && worker->open(noThread_))
            {
                worker->setId(x);
                workers_.push_back(worker);
                continue;
            }

            goto do_check;
        }

        evListen_ = event_new(workers_[0]->evBase_, sListen_, 
            EV_READ | EV_PERSIST, &SocketManager::_Accept, this);
        if (!evListen_) break;

        if (0 != event_add(evListen_, NULL))
            break;

        if (!noThread_)
            state_ = SocketManager::State::RUNNING;

        fOk = true;
    } while (false);

do_check:

    if (!fOk)
    {
        if (evListen_)
        {
            event_free(evListen_);
            evListen_ = NULL;
        }

        if (workers_.size() > 0)
        {
            for (WorkerArray::size_type x = 0; 
                x < workers_.size(); ++x)
            {
                workers_[x]->stop();
                workers_[x]->join();
            }
            workers_.clear();
        }

        if (sListen_ != -1)
        {
            Socket::close(sListen_);
            sListen_ = -1;
        }
    }

    return fOk;
}

void SocketManager::close(void)
{
    assert(state_ == SocketManager::State::RUNNING 
        && sListen_ && evListen_);

    state_ = SocketManager::State::EXITING;

    if (evListen_)
    {
        event_free(evListen_);
        evListen_ = NULL;
    }

    if (sListen_ != -1)
    {
        Socket::close(sListen_);
        sListen_ = -1;
    }

    if (sockets_.size() > 0)
    {
        boost::lock_guard<boost::mutex> guard(socketsLOCK_);
        for (SocketMap::iterator it = sockets_.begin();
            it != sockets_.end(); ++it)
        {
            it->second->_doClose();
        }
        sockets_.clear();
    }

    if (workers_.size() > 0)
    {
        for (WorkerArray::size_type x = 0;
            x < workers_.size(); ++x)
        {
            workers_[x]->stop();
        }
    }
}

void SocketManager::join(void)
{
    if (!noThread_)
    {
        assert(state_ == SocketManager::State::RUNNING);
        for (WorkerArray::size_type x = 0;
            x < workers_.size(); ++x)
        {
            workers_[x]->join();
        }
    }
    else
    {
        assert(state_ == SocketManager::State::READY);
        state_ = SocketManager::State::RUNNING;
        workers_[0]->run();
    }

    workers_.clear();
    state_ = SocketManager::State::EXIT;
}

void SocketManager::setSocketFactory(SocketFactoryPtr factory)
{
    assert(state_ == SocketManager::State::READY);
    sFactory_ = factory;
}

SocketWorker *SocketManager::findFreeWorker(void)
{
    WorkerArray::size_type minLoadIdx = -1;
    std::uint32_t minLoad = UINT_MAX;
    for (WorkerArray::size_type x = 0;
        x < workers_.size(); ++x)
    {
        std::uint32_t curLoad = workers_[x]->getLoad();
        if (curLoad < minLoad)
        {
            minLoad = curLoad;
            minLoadIdx = x;
        }
    }
    return workers_[minLoadIdx].get();
}

void SocketManager::_closeSocket(Socket *s)
{
    assert(s);
    
    boost::lock_guard<boost::mutex> guard(socketsLOCK_);
    SocketMap::iterator find = sockets_.find(s->id_);
    if (find != sockets_.end())
    {
        workers_[s->ownerId_]->decLoad();
        sockets_.erase(find);
    }
}

void SocketManager::_doAccept(void)
{
    evutil_socket_t newsock = ::accept(sListen_, NULL, NULL);
    if (newsock == -1)
        return;

    SocketPtr sock = sFactory_->create();
    if (sock && sock->open(newsock))
    {
        sock->sockMgr_ = this;
        SocketWorker *worker = findFreeWorker();
        if (worker->addSocket(sock))
        {
            {
                boost::lock_guard<boost::mutex> guard(socketsLOCK_);
                if (state_ != SocketManager::State::EXITING)
                {
                    while (sockets_.find(++sidGen_) != sockets_.end());
                    sock->id_ = sidGen_ - 1;
                    sockets_[sock->id_] = sock;
                    return;
                }
            }

            worker->decLoad();
        }
        sock->_doClose();
        return;
    }
    Socket::close(newsock);
}

void SocketManager::_Accept(evutil_socket_t sock, short event, void* arg)
{
    SocketManager *pThis = static_cast<SocketManager*>(arg);
    assert(pThis);
    pThis->_doAccept();
}