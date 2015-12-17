///////////////////////////////////////////////////////////////////////////////
// File:        IniFile.h
// Author:      671643387@qq.com
// Date:        2015年12月16日 下午5:43:24
// Description:
///////////////////////////////////////////////////////////////////////////////

#ifndef SHARED_UTIL_INIFILE_H_
#define SHARED_UTIL_INIFILE_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif

#include <string>
#include <boost/noncopyable.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

class IniFile : public boost::noncopyable
{
public:
    IniFile(void);
    IniFile(const std::string &file);

public:
    void open(const std::string &file);

    template<typename T>
    T get(const std::string &name)
    {
        return file_.get<T>(name);
    }

private:
    boost::property_tree::ptree file_;
};

#endif /* SHARED_UTIL_INIFILE_H_ */
