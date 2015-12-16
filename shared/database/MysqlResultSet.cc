///////////////////////////////////////////////////////////////////////////////
// File:        MysqlResultSet.cc
// Author:      671643387@qq.com
// Date:        2015年12月16日 下午4:55:40
// Description:
///////////////////////////////////////////////////////////////////////////////

#include <cassert>
#include "database/SqlField.h"
#include "database/MysqlResultSet.h"

MysqlResultSet::MysqlResultSet(MYSQL_RES* res)
: SqlResultSet()
, my_(res)
{
    assert(res);

    nfield_ = mysql_num_fields(res);
    if (nfield_ == 0) return;
    row_ = new SqlField[nfield_];
    if (!row_) return;

    MYSQL_FIELD* rfields = mysql_fetch_fields(res);
    for (unsigned int x = 0; x < nfield_; ++x)
        row_[x].setType(convertTypes(rfields[x].type));
}

MysqlResultSet::~MysqlResultSet(void)
{
    close();
}

bool MysqlResultSet::nextRow(void)
{
    assert(my_);
    MYSQL_ROW row = mysql_fetch_row(my_);
    if (!row)
    {
        close();
        return false;
    }

    for (unsigned int x = 0; x < nfield_; ++x)
        row_[x].setValue(row[x]);

    return true;
}

void MysqlResultSet::close(void)
{
    if (my_)
    {
        mysql_free_result(my_);
        my_ = NULL;
    }

    if (row_)
    {
        delete[] row_;
        row_ = NULL;
    }
}

SqlField::DataTypes MysqlResultSet::convertTypes(enum_field_types mysqlType) const
{
    switch (mysqlType)
    {
    case FIELD_TYPE_TIMESTAMP:
    case FIELD_TYPE_DATE:
    case FIELD_TYPE_TIME:
    case FIELD_TYPE_DATETIME:
    case FIELD_TYPE_YEAR:
    case FIELD_TYPE_STRING:
    case FIELD_TYPE_VAR_STRING:
    case FIELD_TYPE_BLOB:
    case FIELD_TYPE_SET:
    case FIELD_TYPE_NULL:
        return SqlField::DB_TYPE_STRING;
    case FIELD_TYPE_TINY:
    case FIELD_TYPE_SHORT:
    case FIELD_TYPE_LONG:
    case FIELD_TYPE_INT24:
    case FIELD_TYPE_LONGLONG:
    case FIELD_TYPE_ENUM:
        return SqlField::DB_TYPE_INTEGER;
    case FIELD_TYPE_DECIMAL:
    case FIELD_TYPE_FLOAT:
    case FIELD_TYPE_DOUBLE:
        return SqlField::DB_TYPE_FLOAT;
    default:
        return SqlField::DB_TYPE_UNKNOWN;
    }
}