///////////////////////////////////////////////////////////////////////////////
// File:        SqlConnection.h
// Author:      671643387@qq.com
// Date:        2015年12月16日 下午4:39:15
// Description:
///////////////////////////////////////////////////////////////////////////////

#ifndef SHARED_DATABASE_SQLCONNECTION_H_
#define SHARED_DATABASE_SQLCONNECTION_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif

#include <string>
#include <boost/shared_ptr.hpp>
#include "database/SqlResultSet.h"

class SqlConnection
{
public:
    SqlConnection(void);
    virtual ~SqlConnection(void);

public:
    virtual bool open(const std::string &info) = 0;
    virtual void close(void) = 0;

    virtual SqlResultSetPtr query(const char *sql) = 0;
    virtual bool execute(const char *sql) = 0;
};
typedef boost::shared_ptr<SqlConnection>    SqlConnectionPtr;

#endif /* SHARED_DATABASE_SQLCONNECTION_H_ */
