///////////////////////////////////////////////////////////////////////////////
// File:        Thread.h
// Author:      671643387@qq.com
// Date:        2015年12月16日 下午1:47:29
// Description:
///////////////////////////////////////////////////////////////////////////////

#ifndef SHARED_THREAD_H_
#define SHARED_THREAD_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif

#include <boost/noncopyable.hpp>
#include <ctime>
#if defined(__WINDOWS__)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <process.h>
#elif defined(__LINUX__)
#include <pthread.h>
#endif // __WINDOWS__

class Thread : public boost::noncopyable
{
public:
#if defined(__WINDOWS__)
    typedef DWORD       tid_t;
#elif defined(__LINUX__)
    typedef pthread_t   tid_t;
#endif // __WINDOWS__

    enum State
    {
        NOTREADY,
        READY,
        RUNNING,
        EXIT
    };

public:
    Thread(void);
    virtual ~Thread(void);

    bool start(void);
    virtual void stop(void) = 0;

    void join(void);

    tid_t id(void) const { return id_; }
    State state(void) const { return state_; }
    static tid_t this_id(void);
    static void sleep(time_t msec);

protected:
    virtual void run(void) = 0;

private:
#if defined(__WINDOWS__)
    static unsigned int __stdcall proc_(void* arg);
#elif defined(__LINUX__)
    static void* proc_(void* arg);
#endif

private:
    tid_t       id_;
    State       state_;
#if defined(__WINDOWS__)
    uintptr_t   handle_;
#endif // __WINDOWS__
};

#endif /* SHARED_THREAD_H_ */
