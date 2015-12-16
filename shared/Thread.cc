///////////////////////////////////////////////////////////////////////////////
// File:        Thread.cc
// Author:      671643387@qq.com
// Date:        2015年12月16日 下午1:47:48
// Description:
///////////////////////////////////////////////////////////////////////////////

#include <cassert>
#include "Thread.h"

Thread::Thread(void)
: id_(0)
, state_(Thread::NOTREADY)
#if defined(__WINDOWS__)
, handle_(-1L)
#elif defined(__LINUX__)
#endif
{

}

Thread::~Thread(void)
{
#if defined(__WINDOWS__)
    if (handle_ != -1L)
        ::CloseHandle(HANDLE(handle_));
#endif
}

bool Thread::start(void)
{
    assert(state_ == Thread::NOTREADY);
    bool fOk = false;
#if defined(__WINDOWS__)
    handle_ = ::_beginthreadex(NULL, 0, Thread::proc_,
        this, 0, (unsigned int*)&id_);
    fOk = (handle_ != -1L);
#elif defined(__LINUX__)
    fOk = (::pthread_create(&id_, NULL, Thread::proc_,
        this) == 0);
#endif
    if (fOk) state_ = Thread::READY;
    return fOk;
}

void Thread::join(void)
{
#if defined(__WINDOWS__)
    assert(handle_ != -1L);
    ::WaitForSingleObject(HANDLE(handle_), INFINITE);
    ::CloseHandle(HANDLE(handle_));
    handle_ = -1L;
#elif defined(__LINUX__)
    ::pthread_join(id_, NULL);
#endif
}

Thread::tid_t Thread::this_id(void)
{
#if defined(__WINDOWS__)
    return ::GetCurrentThreadId();
#elif defined(__LINUX__)
    return ::pthread_self();
#endif
}

void Thread::sleep(time_t msec)
{
#if defined(__WINDOWS__)
    ::Sleep(DWORD(msec));
#elif defined(__LINUX__)
    ::usleep(int(msec * 1000));
#endif
}

#if defined(__WINDOWS__)
unsigned int Thread::proc_(void* arg)
#elif defined(__LINUX__)
void* Thread::proc_(void* arg)
#endif
{
    assert(arg);
    Thread* pThread = static_cast<Thread*>(arg);
    pThread->state_ = Thread::RUNNING;
    pThread->run();
    pThread->state_ = Thread::EXIT;
    return 0;
}