///////////////////////////////////////////////////////////////////////////////
// File:        Time.cc
// Author:      671643387@qq.com
// Date:        2015年12月17日 下午5:40:58
// Description:
///////////////////////////////////////////////////////////////////////////////

#include <ctime>
#include "util/Time.h"

#if defined(__WINDOWS__)
#pragma warning(disable:4996)
#endif

TIME_t fromStartup(void)
{
#if defined(__WINDOWS__)
    return ::GetTickCount();
#elif defined(__LINUX__)
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    return time_t(time.tv_nsec / 1000000) + time_t(time.tv_sec * 1000);
#endif
}

std::string now(void)
{
    time_t now = time(NULL);
    struct tm *tm = localtime(&now);
    char buf[128] = { 0 };
    sprintf(buf, "%d/%d/%d %02d:%02d:%02d", 
        tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday,
        tm->tm_hour, tm->tm_min, tm->tm_sec);
    return std::string(buf);
}