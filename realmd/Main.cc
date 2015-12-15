///////////////////////////////////////////////////////////////////////////////
// File:        Main.cc
// Author:      671643387@qq.com
// Date:        2015年12月15日 下午3:26:50
// Description:
///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <signal.h>
#if defined(__WINDOWS__)
#include <crtdbg.h>
#endif

#include "net/SocketManager.h"

SocketManager *gsockmgr = NULL;

void signal_handler(int signal)
{
    if (signal == SIGINT)
        gsockmgr->close();
}

int main(int argc, char *argv[])
{
#if defined(__WINDOWS__)
    _CrtSetBreakAlloc(163);
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

    signal(SIGINT, signal_handler);
    {
        SocketManager sockmgr;
        if (sockmgr.open("0.0.0.0", 8888))
        {
            gsockmgr = &sockmgr;
            sockmgr.join();
        }
    }

#if defined(__WINDOWS__)
    WSACleanup();
    _CrtDumpMemoryLeaks();
#endif

    return 0;
}