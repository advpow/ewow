///////////////////////////////////////////////////////////////////////////////
// File:        IniFile.cc
// Author:      671643387@qq.com
// Date:        2015年12月16日 下午5:45:04
// Description:
///////////////////////////////////////////////////////////////////////////////

#include "util/IniFile.h"

IniFile::IniFile(void)
{

}

IniFile::IniFile(const std::string &file)
{
    boost::property_tree::ini_parser::read_ini(file, file_);
}

///////////////////////////////////////////////////////////////////////////////

void IniFile::open(const std::string &file)
{
    boost::property_tree::ini_parser::read_ini(file, file_);
}