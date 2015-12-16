template<typename T>
SqlDatabase<T>::SqlDatabase(void)
: conn_(new T())
{

}

template<typename T>
SqlDatabase<T>::~SqlDatabase(void)
{

}

template<typename T>
bool SqlDatabase<T>::initialize(const std::string& info)
{
    assert(conn_);
    return conn_->open(info);
}

template<typename T>
void SqlDatabase<T>::close(void)
{
    if (conn_)
    {
        conn_->close();
        conn_.reset();
    }
}

template<typename T>
void SqlDatabase<T>::escapeString(std::string& s)
{
    assert(conn_);
}

template<typename T>
SqlResultSetPtr SqlDatabase<T>::pquery(const std::string &sql, ...)
{
    char lsql[MAX_QUERY_LEN];
    std::va_list varg;
    va_start(varg, sql);
    int ret = vsnprintf(lsql, MAX_QUERY_LEN, sql.c_str(), varg);
    va_end(varg);
    if (ret == -1)
    {
        //ERROR_LOG("SQL Query truncated (and not execute) for format: %s", sql.c_str());
        return SqlResultSetPtr();
    }
    return conn_->query(lsql);
}

template<typename T>
bool SqlDatabase<T>::pexecute(const std::string &sql, ...)
{
    char lsql[MAX_QUERY_LEN];
    std::va_list varg;
    va_start(varg, sql);
    int ret = vsnprintf(lsql, MAX_QUERY_LEN, sql.c_str(), varg);
    va_end(varg);
    if (ret == -1)
    {
        //ERROR_LOG("SQL Query truncated (and not execute) for format: %s", sql.c_str());
        return false;
    }
    return conn_->execute(lsql);
}