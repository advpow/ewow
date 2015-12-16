///////////////////////////////////////////////////////////////////////////////
// File:        MysqlResultSet.h
// Author:      671643387@qq.com
// Date:        2015年12月16日 下午4:55:36
// Description:
///////////////////////////////////////////////////////////////////////////////

#ifndef SHARED_DATABASE_MYSQLRESULTSET_H_
#define SHARED_DATABASE_MYSQLRESULTSET_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif

#include <mysql.h>
#include "database/SqlField.h"
#include "database/SqlResultSet.h"

class MysqlResultSet : public SqlResultSet
{
public:
    MysqlResultSet(MYSQL_RES* res);
    virtual ~MysqlResultSet(void);

public:
    virtual bool nextRow(void);
    virtual void close(void);

    SqlField::DataTypes convertTypes(enum_field_types mysqlType) const;

private:
    MYSQL_RES*      my_;
};

#endif /* SHARED_DATABASE_MYSQLRESULTSET_H_ */
