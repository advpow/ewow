///////////////////////////////////////////////////////////////////////////////
// File:        MysqlConnection.h
// Author:      671643387@qq.com
// Date:        2015年12月16日 下午4:55:16
// Description:
///////////////////////////////////////////////////////////////////////////////

#ifndef SHARED_DATABASE_MYSQLCONNECTION_H_
#define SHARED_DATABASE_MYSQLCONNECTION_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif

#include <mysql.h>
#include "database/SqlConnection.h"

class MysqlConnection : public SqlConnection
{
public:
    MysqlConnection(void);
    virtual ~MysqlConnection(void);

public:
    virtual bool open(const std::string &info);
    virtual void close(void);

    virtual SqlResultSetPtr query(const std::string &sql);
    virtual bool execute(const std::string &sql);

private:
    MYSQL*  my_;
};

#endif /* SHARED_DATABASE_MYSQLCONNECTION_H_ */
