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

typedef std::uint8_t    BYTE_t;

///////////////////////////////////////////////////////////////////////////////

#if defined(__LINUX__)
#define SOCKET_ERROR            -1
#endif

#define SOCKET_ERROR_WOULDBLOCK -500

///////////////////////////////////////////////////////////////////////////////
#define __MIN_(x, y)    ((x) > (y) ? (y) : (x))

#endif /* SHARED_TYPEDEF_H_ */
