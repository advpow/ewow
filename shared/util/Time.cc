///////////////////////////////////////////////////////////////////////////////
// File:        Time.cc
// Author:      671643387@qq.com
// Date:        2015年12月17日 下午5:40:58
// Description:
///////////////////////////////////////////////////////////////////////////////

#include "util/Time.h"

TIME_t fromStartup(void)
{
#if defined(__WINDOWS__)
    return ::GetTickCount();
#elif defined(__LINUX__)
#endif
}
