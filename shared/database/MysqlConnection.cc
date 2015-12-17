///////////////////////////////////////////////////////////////////////////////
// File:        MysqlConnection.cc
// Author:      671643387@qq.com
// Date:        2015年12月16日 下午4:55:25
// Description:
///////////////////////////////////////////////////////////////////////////////

#include "Log.h"
#include "util/String.h"
#include "database/MysqlResultSet.h"
#include "database/MysqlConnection.h"

MysqlConnection::MysqlConnection(void)
: SqlConnection()
, my_(NULL)
{

}

MysqlConnection::~MysqlConnection(void)
{
    if (my_)
        mysql_close(my_);
}

///////////////////////////////////////////////////////////////////////////////

bool MysqlConnection::open(const std::string &info)
{
    my_ = mysql_init(NULL);

    do
    {
        Tokens tokens = StrSplit(info, ";");
        Tokens::iterator iter = tokens.begin();

        std::string host, port_or_socket, user, password, database;
        int port;
        const char* unix_socket;

        if (iter != tokens.end())
            host = *iter++;
        if (iter != tokens.end())
            port_or_socket = *iter++;
        if (iter != tokens.end())
            user = *iter++;
        if (iter != tokens.end())
            password = *iter++;
        if (iter != tokens.end())
            database = *iter++;

        mysql_options(my_, MYSQL_SET_CHARSET_NAME, "utf8");
        mysql_options(my_, MYSQL_OPT_RECONNECT, "1");

#if defined(__WINDOWS__)
        if (host == ".")
        {
            unsigned int opt = MYSQL_PROTOCOL_PIPE;
            mysql_options(my_, MYSQL_OPT_PROTOCOL, &opt);
            port = 0;
            unix_socket = NULL;
        }
        else
        {
            port = atoi(port_or_socket.c_str());
            unix_socket = NULL;
        }
#elif defined(__LINUX__)
        if (host == ".")
        {
            unsigned int opt = MYSQL_PROTOCOL_SOCKET;
            mysql_options(my_, MYSQL_OPT_PROTOCOL, &opt);
            port = 0;
            unix_socket = port_or_socket.c_str();
        }
        else
        {
            port = atoi(port_or_socket.c_str());
            unix_socket = NULL;
        }
#endif

        if (!mysql_real_connect(my_, host.c_str(), user.c_str(),
            password.c_str(), database.c_str(), port, unix_socket, 0))
        {
            ERROR_LOG("could not connect to MYSQL database at %s: %s",
                host.c_str(), mysql_error(my_));
            mysql_close(my_);
            my_ = NULL;
            break;
        }

    } while (false);

    return my_ != NULL;
}

void MysqlConnection::close(void)
{
    if (my_ != NULL)
    {
        mysql_close(my_);
        my_ = NULL;
    }
}

///////////////////////////////////////////////////////////////////////////////

SqlResultSetPtr MysqlConnection::query(const char *sql)
{
    if (!mysql_query(my_, sql))
    {
        MYSQL_RES* res = mysql_store_result(my_);
        if (res != NULL)
        {
            SqlResultSetPtr rs(new MysqlResultSet(res));
            if (rs && rs->nextRow())
                return rs;
        }
    }
    else
        ERROR_LOG("MysqlConnection::Query %s", mysql_error(my_));
    return SqlResultSetPtr();
}

bool MysqlConnection::execute(const char *sql)
{
    if (!mysql_query(my_, sql))
        return true;
    ERROR_LOG("MysqlConnection::Execute %s", mysql_error(my_));
    return false;
}