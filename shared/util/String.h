///////////////////////////////////////////////////////////////////////////////
// File:        String.h
// Author:      671643387@qq.com
// Date:        2015年12月16日 下午5:02:46
// Description:
///////////////////////////////////////////////////////////////////////////////

#ifndef SHARED_UTIL_STRING_H_
#define SHARED_UTIL_STRING_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif

#include <string>
#include <vector>

typedef std::vector<std::string>    Tokens;

Tokens StrSplit(const std::string& src, const std::string& sep);

#endif /* SHARED_UTIL_STRING_H_ */
