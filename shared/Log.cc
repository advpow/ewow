///////////////////////////////////////////////////////////////////////////////
// File:        Log.cc
// Author:      671643387@qq.com
// Date:        2015年12月18日 下午1:41:52
// Description:
///////////////////////////////////////////////////////////////////////////////

#if defined(__WINDOWS__)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#include <iostream>
#include "Log.h"
#include "Thread.h"
#include "util/Time.h"

#if defined(__WINDOWS__)
#pragma warning(disable:4996)
#endif

#define LOG_BUFF_SIZE   0x10000

Log sLog;

Log::Log(void)
{

}

Log::~Log(void)
{

}

void Log::basicLog(const char *fmt, ...)
{
    va_list varg;
    va_start(varg, fmt);
    _printLog("BASIC", fmt, varg);
    va_end(varg);
}

void Log::errorLog(const char *fmt, ...)
{
    setForegroundColor(ConsoleColor::ForegroundRed, true);
    va_list varg;
    va_start(varg, fmt);
    _printLog("ERROR", fmt, varg);
    va_end(varg);
    setForegroundColor(ConsoleColor::ForegroundWhite);
}

void Log::debugLog(const char *fmt, ...)
{
    setForegroundColor(ConsoleColor::ForegroundGreen);
    va_list varg;
    va_start(varg, fmt);
    _printLog("DEBUG", fmt, varg);
    va_end(varg);
    setForegroundColor(ConsoleColor::ForegroundWhite);
}

void Log::_printLog(const char *level, const char *fmt, std::va_list vargs)
{
    char buf[LOG_BUFF_SIZE];
    vsnprintf(buf, LOG_BUFF_SIZE, fmt, vargs);

    std::cout << now() << ", " << Thread::this_id() << ", "
        << level << ", " << buf << std::endl;
}

void Log::clear(void)
{

}

void Log::setForegroundColor(ConsoleColor color, bool intensity /* = false */)
{
#if defined(__WINDOWS__)
    WORD wcolor = 0;
    switch (color)
    {
    case Log::ForegroundWhite:
        wcolor = WORD(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
        break;
    case Log::ForegroundBlue:
        wcolor = WORD(FOREGROUND_BLUE);
        break;
    case Log::ForegroundRed:
        wcolor = WORD(FOREGROUND_RED);
        break;
    case Log::ForegroundGreen:
        wcolor = WORD(FOREGROUND_GREEN);
        break;
    default:
        break;
    }
    if (intensity) wcolor |= FOREGROUND_INTENSITY;
    ::SetConsoleTextAttribute(::GetStdHandle(STD_OUTPUT_HANDLE), wcolor);
#endif
}