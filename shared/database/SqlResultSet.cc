///////////////////////////////////////////////////////////////////////////////
// File:        SqlResultSet.cc
// Author:      671643387@qq.com
// Date:        2015年12月16日 下午4:38:54
// Description:
///////////////////////////////////////////////////////////////////////////////

#include <cassert>
#include "database/SqlField.h"
#include "database/SqlResultSet.h"

SqlResultSet::SqlResultSet(void)
: row_(NULL)
, nfield_(0)
{

}

SqlResultSet::~SqlResultSet(void)
{

}

SqlField* SqlResultSet::operator[](unsigned int idx)
{
    assert(idx >= 0 && idx < nfield_);
    return &row_[idx];
}

SqlField* SqlResultSet::getField(unsigned int idx)
{
    assert(idx >= 0 && idx < nfield_);
    return &row_[idx];
}

unsigned int SqlResultSet::getFieldCount(void)
{
    return nfield_;
}