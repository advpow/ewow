///////////////////////////////////////////////////////////////////////////////
// File:        String.cc
// Author:      671643387@qq.com
// Date:        2015年12月16日 下午5:02:53
// Description:
///////////////////////////////////////////////////////////////////////////////

#include "util/String.h"

Tokens StrSplit(const std::string& src, const std::string& sep)
{
    Tokens r;
    std::string s;
    for (std::string::const_iterator i = src.begin(); i != src.end(); ++i)
    {
        if (sep.find(*i) != std::string::npos)
        {
            if (s.length()) { r.push_back(s); }
            s = "";
        }
        else
            s += *i;
    }
    if (s.length()) { r.push_back(s); }
    return r;
}