///////////////////////////////////////////////////////////////////////////////
// File:        Main.cc
// Author:      671643387@qq.com
// Date:        2015年12月15日 下午3:26:50
// Description:
///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <signal.h>
#if defined(__WINDOWS__)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include "Log.h"
#include "util/IniFile.h"
#include "net/SocketManager.h"
#include "net/RealmdSocketFactory.h"
#include "database/SqlDatabase.h"


// 配置文件
IniFile sConfig;
// 登录数据库
MysqlDatabase sLoginDB;
// 套接字管理器
SocketManager *gsockmgr = NULL;


/* 信号处理程序 */
void signal_handler(int signal)
{
    // 收到SIGINT信号，关闭服务器
    if (signal == SIGINT)
        gsockmgr->close();
}


int main(int argc, char *argv[])
{
#if defined(__WINDOWS__)
    //_CrtSetBreakAlloc(163);
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

    signal(SIGINT, signal_handler);
    {
        try
        {
            do 
            {
                // 读取配置
                sConfig.open("realmd.ini");

                // 打开数据库连接
                if (!sLoginDB.initialize(sConfig.get<std::string>("RealmdConf.LoginDBInfo")))
                {
                    ERROR_LOG("Initialize login database failed");
                    break;
                }

                // 启动网络服务
                SocketManager sockmgr(SocketFactoryPtr(new RealmdSocketFactory()), true);
                if (sockmgr.open(sConfig.get<std::string>("RealmdConf.BindIP").c_str(), 
                    sConfig.get<int>("RealmdConf.BindPort"), sConfig.get<int>("RealmdConf.BindBacklog")))
                {
                    gsockmgr = &sockmgr;
                    sockmgr.join();

                    // 关闭数据库
                    sLoginDB.close();
                }
            } while (false);
        }
        catch (...)
        {
            ERROR_LOG("Startup realmd server failed");
        }
    }

#if defined(__WINDOWS__)
    WSACleanup();
    _CrtDumpMemoryLeaks();
#endif

    return 0;
}