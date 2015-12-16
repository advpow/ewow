///////////////////////////////////////////////////////////////////////////////
// File:        SqlResultSet.h
// Author:      671643387@qq.com
// Date:        2015年12月16日 下午4:38:42
// Description:
///////////////////////////////////////////////////////////////////////////////

#ifndef SHARED_DATABASE_SQLRESULTSET_H_
#define SHARED_DATABASE_SQLRESULTSET_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif

#include <cstddef>
#include <boost/shared_ptr.hpp>

class SqlField;

class SqlResultSet
{
public:
    SqlResultSet(void);
    virtual ~SqlResultSet(void);

public:
    virtual bool nextRow(void) = 0;
    virtual void close(void) = 0;

    SqlField* operator[](unsigned int idx);
    SqlField* getField(unsigned int idx);
    unsigned int getFieldCount(void);

protected:
    SqlField*       row_;
    unsigned int    nfield_;
};
typedef boost::shared_ptr<SqlResultSet> SqlResultSetPtr;

#endif /* SHARED_DATABASE_SQLRESULTSET_H_ */
