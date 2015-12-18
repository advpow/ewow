///////////////////////////////////////////////////////////////////////////////
// File:        Typedef.h
// Author:      671643387@qq.com
// Date:        2015年12月15日 上午10:58:04
// Description:
///////////////////////////////////////////////////////////////////////////////

#ifndef SHARED_TYPEDEF_H_
#define SHARED_TYPEDEF_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif

#include <cstddef>
#include <cstdint>
#include <string>
#include <event2/event.h>

//#if defined(__WINDOWS__)
//#define WIN32_LEAN_AND_MEAN
//#include <Windows.h>
//#elif defined(__LINUX__)
//#include <time.h>
//#endif

typedef std::uint8_t    BYTE_t;

///////////////////////////////////////////////////////////////////////////////
#if defined(__LINUX__)
#define SOCKET_ERROR            -1
#define INVALID_SOCKET          (ev_uintptr_t)-1
#endif

#define SOCKET_ERROR_WOULDBLOCK -500

#if defined(__WINDOWS__)
typedef int		socklen_t;
#endif

///////////////////////////////////////////////////////////////////////////////
#if defined(__WINDOWS__)
typedef DWORD   TIME_t;
#elif defined(__LINUX__)
typedef long	TIME_t;
#endif

///////////////////////////////////////////////////////////////////////////////
#define __MIN_(x, y)    ((x) > (y) ? (y) : (x))

#endif /* SHARED_TYPEDEF_H_ */
