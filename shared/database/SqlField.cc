///////////////////////////////////////////////////////////////////////////////
// File:        SqlField.cc
// Author:      671643387@qq.com
// Date:        2015年12月16日 下午4:37:48
// Description:
///////////////////////////////////////////////////////////////////////////////

#include "database/SqlField.h"

#if defined(__WINDOWS__)
#pragma warning(disable:4996)
#endif

SqlField::SqlField(void)
: value_(NULL)
, type_(DB_TYPE_UNKNOWN)
{

}

SqlField::SqlField(const char* value, enum DataTypes type)
: value_(value)
, type_(type)
{

}

SqlField::~SqlField(void)
{

}

uint64_t SqlField::getUInt64() const
{
    uint64_t value = 0;
    if (!value_ || sscanf(value_, "%I64u", &value) == -1)
    {
        return 0;
    }
    return value;
}
int64_t SqlField::getInt64() const
{
    int64_t value = 0;
    if (!value_ || sscanf(value_, "%I64d", &value) == -1)
        return 0;

    return value;
}