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

#include <stdio.h>

#define BASIC_LOG(fmt, ...) printf(fmt, __VA_ARGS__); printf("\n")
#define ERROR_LOG(fmt, ...) printf(fmt, __VA_ARGS__); printf("\n")
#define DEBUG_LOG(fmt, ...) printf(fmt, __VA_ARGS__); printf("\n")

#endif /* SHARED_LOG_H_ */
