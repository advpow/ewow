///////////////////////////////////////////////////////////////////////////////
// File:        SqlDatabase.h
// Author:      671643387@qq.com
// Date:        2015年12月16日 下午4:40:46
// Description:
///////////////////////////////////////////////////////////////////////////////

#ifndef SHARED_DATABASE_SQLDATABASE_H_
#define SHARED_DATABASE_SQLDATABASE_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif

#include <cassert>
#include <cstdarg>
#include "database/SqlField.h"
#include "database/SqlConnection.h"
#include "database/SqlResultSet.h"

#if defined(__WINDOWS__)
#pragma warning(disable:4996)
#endif

template<typename T>
class SqlDatabase
{
private:
    const static int MAX_QUERY_LEN = 32 * 1024;

public:
    SqlDatabase(void);
    ~SqlDatabase(void);

public:
    bool initialize(const std::string& info);
    void close(void);

    void escapeString(std::string& s);

    SqlResultSetPtr pquery(const std::string &sql, ...);
    bool pexecute(const std::string &sql, ...);

private:
    SqlConnectionPtr conn_;
};

#include "database/SqlDatabase.inl"

#include "database/MysqlConnection.h"
typedef SqlDatabase<MysqlConnection>    MysqlDatabase;

#endif /* SHARED_DATABASE_SQLDATABASE_H_ */
