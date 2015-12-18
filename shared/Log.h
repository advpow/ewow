///////////////////////////////////////////////////////////////////////////////
// File:        Log.h
// Author:      671643387@qq.com
// Date:        2015年12月16日 下午5:29:19
// Description:
///////////////////////////////////////////////////////////////////////////////

#ifndef SHARED_LOG_H_
#define SHARED_LOG_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif

#include <cstdarg>

class Log
{
public:
    enum ConsoleColor
    {
        ForegroundWhite = 0x01,
        ForegroundBlue = 0x02,
        ForegroundRed = 0x04,
        ForegroundGreen = 0x08,
    };

public:
    Log(void);
    ~Log(void);

public:
    void basicLog(const char *fmt, ...);
    void errorLog(const char *fmt, ...);
    void debugLog(const char *fmt, ...);

    void clear(void);

private:
    void setForegroundColor(ConsoleColor color, bool intensity = false);

private:
    void _printLog(const char *level, const char *fmt, std::va_list vargs);
};

extern Log sLog;

#define BASIC_LOG sLog.basicLog
#define ERROR_LOG sLog.errorLog
#define DEBUG_LOG sLog.debugLog

#endif /* SHARED_LOG_H_ */
