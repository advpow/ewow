///////////////////////////////////////////////////////////////////////////////
// File:        RealmdSocket.cc
// Author:      671643387@qq.com
// Date:        2015年12月16日 下午2:01:59
// Description:
///////////////////////////////////////////////////////////////////////////////

#include <openssl/md5.h>
#include "Log.h"
#include "Realmd.h"
#include "net/RealmdSocket.h"

enum eStatus
{
    STATUS_CONNECTED = 0,
    STATUS_AUTHED
};

enum eAccountFlags
{
    ACCOUNT_FLAG_GM = 0x00000001,
    ACCOUNT_FLAG_TRIAL = 0x00000008,
    ACCOUNT_FLAG_PROPASS = 0x00800000,
};

// GCC have alternative #pragma pack(N) syntax and old gcc version not support pack(push,N), also any gcc version not support it at some paltform
#if defined( __GNUC__ )
#pragma pack(1)
#else
#pragma pack(push,1)
#endif

typedef struct AUTH_LOGON_CHALLENGE_C
{
    std::uint8_t   cmd;
    std::uint8_t   error;
    std::uint16_t  size;
    std::uint8_t   gamename[4];
    std::uint8_t   version1;
    std::uint8_t   version2;
    std::uint8_t   version3;
    std::uint16_t  build;
    std::uint8_t   platform[4];
    std::uint8_t   os[4];
    std::uint8_t   country[4];
    std::uint32_t  timezone_bias;
    std::uint32_t  ip;
    std::uint8_t   I_len;
    std::uint8_t   I[1];
} sAuthLogonChallenge_C;

typedef struct AUTH_LOGON_PROOF_C
{
    std::uint8_t   cmd;
    std::uint8_t   A[32];
    std::uint8_t   M1[20];
    std::uint8_t   crc_hash[20];
    std::uint8_t   number_of_keys;
    std::uint8_t   securityFlags;                                  // 0x00-0x04
} sAuthLogonProof_C;

typedef struct AUTH_LOGON_PROOF_S
{
    std::uint8_t   cmd;
    std::uint8_t   error;
    std::uint8_t   M2[20];
    std::uint32_t  accountFlags;                                   // see enum AccountFlags
    std::uint32_t  surveyId;                                       // SurveyId
    std::uint16_t  unkFlags;                                       // some flags (AccountMsgAvailable = 0x01)
} sAuthLogonProof_S;

typedef struct AUTH_LOGON_PROOF_S_BUILD_6005
{
    std::uint8_t   cmd;
    std::uint8_t   error;
    std::uint8_t   M2[20];
    // uint32  unk1;
    std::uint32_t  unk2;
    // uint16  unk3;
} sAuthLogonProof_S_BUILD_6005;

typedef struct AUTH_RECONNECT_PROOF_C
{
    std::uint8_t   cmd;
    std::uint8_t   R1[16];
    std::uint8_t   R2[20];
    std::uint8_t   R3[20];
    std::uint8_t   number_of_keys;
} sAuthReconnectProof_C;

typedef struct XFER_INIT
{
    std::uint8_t cmd;                                              // XFER_INITIATE
    std::uint8_t fileNameLen;                                      // strlen(fileName);
    std::uint8_t fileName[5];                                      // fileName[fileNameLen]
    std::uint64_t file_size;                                       // file size (bytes)
    std::uint8_t md5[MD5_DIGEST_LENGTH];                           // MD5
} XFER_INIT;

typedef struct AuthHandler
{
    eAuthCmd cmd;
    std::uint32_t status;
    bool (RealmdSocket::*handler)(void);
} AuthHandler;

// GCC have alternative #pragma pack() syntax and old gcc version not support pack(pop), also any gcc version not support it at some paltform
#if defined( __GNUC__ )
#pragma pack()
#else
#pragma pack(pop)
#endif

const AuthHandler table[] =
{
    { CMD_AUTH_LOGON_CHALLENGE,     STATUS_CONNECTED,   &RealmdSocket::_handleLogonChallenge },
    { CMD_AUTH_LOGON_PROOF,         STATUS_CONNECTED,   &RealmdSocket::_handleLogonProof },
    { CMD_AUTH_RECONNECT_CHALLENGE, STATUS_CONNECTED,   &RealmdSocket::_handleReconnectChallenge },
    { CMD_AUTH_RECONNECT_PROOF,     STATUS_CONNECTED,   &RealmdSocket::_handleReconnectProof },
    { CMD_REALM_LIST,               STATUS_AUTHED,      &RealmdSocket::_handleRealmList },
    { CMD_XFER_ACCEPT,              STATUS_CONNECTED,   &RealmdSocket::_handleXferAccept },
    { CMD_XFER_RESUME,              STATUS_CONNECTED,   &RealmdSocket::_handleXferResume },
    { CMD_XFER_CANCEL,              STATUS_CONNECTED,   &RealmdSocket::_handleXferCancel }
};

#define AUTH_TOTAL_COMMANDS sizeof(table)/sizeof(AuthHandler)

RealmdSocket::RealmdSocket(void)
: bAuthed_(false)
, build_(0)
, accountSecurityLevel_(SEC_PLAYER)
{
    N_.setHexStr("894B645E89E1535BBDAD5B8B290650530801B18EBFBF5E8FAB3C82872A3E9BB7");
    g_.setUInt32(7);
}

RealmdSocket::~RealmdSocket(void)
{

}

///////////////////////////////////////////////////////////////////////////////
void RealmdSocket::onRead(void)
{
    BYTE_t cmd;
    while (true)
    {
        if (!recv(&cmd, 1))
            return;

        std::size_t x;
        for (x = 0; x < AUTH_TOTAL_COMMANDS; ++x)
        {
            if ((BYTE_t)table[x].cmd == cmd &&
                (table[x].status == STATUS_CONNECTED || (
                table[x].status == STATUS_AUTHED && bAuthed_)))
            {
                DEBUG_LOG("[Auth] got data for cmd %u recv length %u",
                    (std::uint32_t)cmd, (std::uint32_t)size());

                if (!(*this.*table[x].handler)())
                {
                    DEBUG_LOG("Command handler failed for cmd %u recv length %u",
                        (std::uint32_t)cmd, (std::uint32_t)size());
                    return;
                }
                break;
            }
        }

        if (x == AUTH_TOTAL_COMMANDS)
        {
            DEBUG_LOG("[Auth] got unknown packet %u", (std::uint32_t)cmd);
            return;
        }
    }
}

void RealmdSocket::onClose(void)
{

}

///////////////////////////////////////////////////////////////////////////////
bool RealmdSocket::_handleLogonChallenge(void)
{
    return false;
}

bool RealmdSocket::_handleLogonProof(void)
{
    return false;
}

bool RealmdSocket::_handleReconnectChallenge(void)
{
    return false;
}

bool RealmdSocket::_handleReconnectProof(void)
{
    return false;
}

bool RealmdSocket::_handleRealmList(void)
{
    return false;
}

bool RealmdSocket::_handleXferAccept(void)
{
    return false;
}

bool RealmdSocket::_handleXferResume(void)
{
    return false;
}

bool RealmdSocket::_handleXferCancel(void)
{
    return false;
}