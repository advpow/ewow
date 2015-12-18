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
#include "database/SqlDatabase.h"
#include "RealmList.h"

#define s_BYTE_SIZE 32

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

extern MysqlDatabase sLoginDB;
extern RealmList sRealmList;

RealmdSocket::RealmdSocket(void)
: bAuthed_(false)
, accountId_(0)
, build_(0)
, accountSecurityLevel_(SEC_PLAYER)
{
    N_.setHexStr("894B645E89E1535BBDAD5B8B290650530801B18EBFBF5E8FAB3C82872A3E9BB7");
    g_.setUInt32(7);
}

RealmdSocket::~RealmdSocket(void)
{

}

bool RealmdSocket::open(ev_uintptr_t fd)
{
    if (Socket::open(fd))
    {
        BASIC_LOG("new connection from %s:%d", 
            getPeerAddress().c_str(), getPeerPort());
        return true;
    }
    return false;
}

///////////////////////////////////////////////////////////////////////////////
void RealmdSocket::onRead(void)
{
    BYTE_t cmd;
    while (true)
    {
        if (!peek(&cmd, 1))
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
            close();
            return;
        }
    }
}

void RealmdSocket::onClose(void)
{
    BASIC_LOG("%s:%d disconnected",
        getPeerAddress().c_str(), getPeerPort());
}

///////////////////////////////////////////////////////////////////////////////
bool RealmdSocket::_handleLogonChallenge(void)
{
    // 处理登录请求
    DEBUG_LOG("Entering _handleLogonChallenge");
    if (size() < sizeof(sAuthLogonChallenge_C))
        return false;

    // 接收数据包
    std::vector<BYTE_t> buf;
    buf.resize(4);
    recv(&buf[0], 4);

    std::uint16_t remaining = ((sAuthLogonChallenge_C*)&buf[0])->size;
    DEBUG_LOG("[AuthChallenge] got header, body is %#04x bytes", remaining);
    if ((remaining < sizeof(sAuthLogonChallenge_C)-buf.size()) || (size() < remaining))
        return false;

    buf.resize(remaining + buf.size() + 1);
    buf[buf.size() - 1] = 0;
    recv(&buf[4], remaining);

    sAuthLogonChallenge_C *ch = (sAuthLogonChallenge_C*)&buf[0];
    DEBUG_LOG("[AuthChallenge] got full packet, %#04x bytes", ch->size);
    DEBUG_LOG("[AuthChallenge] name(%d): '%s'", ch->I_len, ch->I);

    ByteBuffer pkt;

    loginName_ = (const char*)ch->I;
    build_ = ch->build;
    safeLoginName_ = loginName_;
    sLoginDB.escapeString(safeLoginName_);

    pkt << (BYTE_t)CMD_AUTH_LOGON_CHALLENGE;
    pkt << (BYTE_t)0;

    // 查询IP地址是否被封
    std::string address = getPeerAddress();
    sLoginDB.escapeString(address);
    SqlResultSetPtr result = sLoginDB.pquery("SELECT unbandate FROM ip_banned WHERE "
        //    permanent                    still banned
        "(unbandate = bandate OR unbandate > UNIX_TIMESTAMP()) AND ip = '%s'", address.c_str());
    if (result)
    {
        // IP地址被封
        pkt << (BYTE_t)WOW_FAIL_BANNED;
        BASIC_LOG("[AuthChallenge] Banned ip %s tries to login!", address.c_str());
    }
    else
    {
        // 查询帐号数据
        result = sLoginDB.pquery("SELECT sha_pass_hash,id,locked,last_ip,gmlevel,v,s FROM account WHERE username = '%s'", safeLoginName_.c_str());
        if (result)
        {
            bool locked = false;
            if ((*result)[2]->getUInt8() == 1)
            {
                // 帐号锁定IP登录
                DEBUG_LOG("[AuthChallenge] Account '%s' is locked to IP - '%s'", loginName_.c_str(), (*result)[3]->getCString());
                DEBUG_LOG("[AuthChallenge] Player address is '%s'", address.c_str());
                if (strcmp((*result)[3]->getCString(), getPeerAddress().c_str()))
                {
                    // 当前登录IP和帐号锁定IP不一致
                    DEBUG_LOG("[AuthChallenge] Account IP differs");
                    pkt << (BYTE_t)WOW_FAIL_SUSPENDED;
                    locked = true;
                }
            }

            if (!locked)
            {
                // 查询帐号封锁数据
                SqlResultSetPtr banresult = sLoginDB.pquery("SELECT bandate,unbandate FROM account_banned WHERE "
                    "id = %u AND active = 1 AND (unbandate > UNIX_TIMESTAMP() OR unbandate = bandate)", (*result)[1]->getUInt32());
                if (banresult)
                {
                    if ((*banresult)[0]->getUInt64() == (*banresult)[1]->getUInt64())
                    {
                        pkt << (BYTE_t)WOW_FAIL_BANNED;
                        BASIC_LOG("[AuthChallenge] Banned account %s tries to login!", loginName_.c_str());
                    }
                    else
                    {
                        pkt << (BYTE_t)WOW_FAIL_SUSPENDED;
                        BASIC_LOG("[AuthChallenge] Temporarily banned account %s tries to login!", loginName_.c_str());
                    }
                }
                else
                {
                    // 获取帐号密码hash值
                    std::string rI = (*result)[0]->getCppString();

                    // 获取保存的v,s字段
                    std::string dbV = (*result)[5]->getCppString();
                    std::string dbS = (*result)[6]->getCppString();

                    DEBUG_LOG("database authentication values: v='%s' s='%s'", dbV.c_str(), dbS.c_str());

                    if (dbV.size() != s_BYTE_SIZE * 2 || dbS.size() != s_BYTE_SIZE * 2)
                        _setVSField(rI);
                    else
                    {
                        s_.setHexStr(dbS.c_str());
                        v_.setHexStr(dbV.c_str());
                    }

                    // SRP计算
                    b_.setRand(19 * 8);
                    BigNumber gmod = g_.modExp(b_, N_);
                    B_ = ((v_ * 3) + gmod) % N_;

                    assert(gmod.getNumBytes() <= 32);

                    BigNumber unk3;
                    unk3.setRand(16 * 8);

                    // 填充响应包
                    pkt << (BYTE_t)WOW_SUCCESS;

                    pkt.append(B_.asByteArray(32), 32);

                    pkt << (BYTE_t)1;
                    pkt.append(g_.asByteArray(), 1);

                    pkt << (BYTE_t)32;
                    pkt.append(N_.asByteArray(32), 32);
                    pkt.append(s_.asByteArray(), s_.getNumBytes());
                    pkt.append(unk3.asByteArray(16), 16);
                    
                    BYTE_t securityFlags = 0;
                    pkt << securityFlags;
                    if (securityFlags & 0x01)
                    {
                        pkt << std::uint32_t(0);
                        pkt << std::uint64_t(0) << std::uint64_t(0);
                    }
                    if (securityFlags & 0x02)
                    {
                        pkt << BYTE_t(0);
                        pkt << BYTE_t(0);
                        pkt << BYTE_t(0);
                        pkt << BYTE_t(0);
                        pkt << std::uint64_t(0);
                    }
                    if (securityFlags & 0x04)
                    {
                        pkt << BYTE_t(1);
                    }

                    // 解析帐号安全等级和登录地区
                    BYTE_t secLevel = (*result)[4]->getUInt8();
                    accountSecurityLevel_ = secLevel <= SEC_ADMINISTRATOR ? eAccountTypes(secLevel) : SEC_ADMINISTRATOR;

                    localizationName_.resize(4);
                    for (int x = 0; x < 4; ++x)
                        localizationName_[x] = ch->country[4 - x - 1];

                    accountId_ = (*result)[1]->getUInt32();

                    BASIC_LOG("[AuthChallenge] account %s is using '%c%c%c%c' locale (%u)", 
                        loginName_.c_str(), ch->country[3], ch->country[2], ch->country[1], ch->country[0], 
                        GetLocaleByName(localizationName_));
                }
            }
        }
        else
        {
            // 没有这个帐号
            pkt << (BYTE_t)WOW_FAIL_UNKNOWN_ACCOUNT;
        }
    }

    send(pkt.contents(), pkt.size());
    return true;
}

bool RealmdSocket::_handleLogonProof(void)
{
    // 登录验证
    DEBUG_LOG("Entering _handleLogonProof");
    ///- Read the packet
    sAuthLogonProof_C lp;
    if (size() < sizeof(sAuthLogonProof_C))
        return false;
    recv((BYTE_t*)&lp, sizeof(sAuthLogonProof_C));

    // 检查版本
    switch (build_)
    {
    case 5875:                                          // 1.12.1
    case 6005:                                          // 1.12.2
    case 6141:                                          // 1.12.3
        break;
    default:
        close();
        return false;
    }

    // SRP6计算
    BigNumber A;
    A.setBinary(lp.A, 32);

    if (A.isZero())
        return false;

    Sha1Hash sha;
    sha.updateBigNumbers(&A, &B_, NULL);
    sha.finalize();

    BigNumber u;
    u.setBinary(sha.getDigest(), sha.getLength());
    BigNumber S = (A * (v_.modExp(u, N_))).modExp(b_, N_);

    BYTE_t t[32];
    BYTE_t t1[16];
    BYTE_t vK[40];
    memcpy(t, S.asByteArray(32), 32);
    for (int x = 0; x < 16; ++x)
        t1[x] = t[x * 2];

    sha.initialize();
    sha.updateData(t1, 16);
    sha.finalize();
    for (int x = 0; x < 20; ++x)
        vK[x * 2] = sha.getDigest()[x];
    for (int x = 0; x < 16; ++x)
        t1[x] = t[x * 2 + 1];
    sha.initialize();
    sha.updateData(t1, 16);
    sha.finalize();
    for (int x = 0; x < 20; ++x)
        vK[x * 2 + 1] = sha.getDigest()[x];
    K_.setBinary(vK, 40);

    BYTE_t hash[20];

    sha.initialize();
    sha.updateBigNumbers(&N_, NULL);
    sha.finalize();
    memcpy(hash, sha.getDigest(), sha.getLength());

    sha.initialize();
    sha.updateBigNumbers(&g_, NULL);
    sha.finalize();
    for (int x = 0; x < 20; ++x)
        hash[x] ^= sha.getDigest()[x];

    BigNumber t3;
    t3.setBinary(hash, 20);

    sha.initialize();
    sha.updateData(loginName_);
    sha.finalize();
    BYTE_t t4[SHA_DIGEST_LENGTH];
    memcpy(t4, sha.getDigest(), SHA_DIGEST_LENGTH);

    sha.initialize();
    sha.updateBigNumbers(&t3, NULL);
    sha.updateData(t4, SHA_DIGEST_LENGTH);
    sha.updateBigNumbers(&s_, &A, &B_, &K_, NULL);
    sha.finalize();

    BigNumber M;
    M.setBinary(sha.getDigest(), sha.getLength());

    if (!memcmp(M.asByteArray(), lp.M1, 20))
    {
        // 登录校验成功
        BASIC_LOG("User '%s' successfully authenticated", loginName_.c_str());

        // 保存SessionKey(K)，更新帐号数据
        const char *K_hex = K_.asHexStr();
        sLoginDB.pexecute("UPDATE account SET sessionkey = '%s', last_ip = '%s', last_login = NOW(), "
            "locale = '%u', failed_logins = 0 WHERE username = '%s'", 
            K_hex, getPeerAddress().c_str(), GetLocaleByName(localizationName_), safeLoginName_.c_str());
        BigNumber::free((void*)K_hex);

        // 发送登录验证数据
        sha.initialize();
        sha.updateBigNumbers(&A, &M, &K_, NULL);
        sha.finalize();
        _sendProof(sha);

        // 登录成功
        bAuthed_ = true;
    }
    else
    {
        if (build_ > 6005)                                  // > 1.12.2
        {
            BYTE_t data[4] = { CMD_AUTH_LOGON_PROOF, WOW_FAIL_UNKNOWN_ACCOUNT, 3, 0 };
            send(data, sizeof(data));
        }
        else
        {
            // 1.x not react incorrectly at 4-byte message use 3 as real error
            BYTE_t data[2] = { CMD_AUTH_LOGON_PROOF, WOW_FAIL_UNKNOWN_ACCOUNT };
            send(data, sizeof(data));
        }

        BASIC_LOG("[AuthChallenge] account %s tried to login with wrong password!", loginName_.c_str());

        // TODO: 统计帐号登录失败次数封锁帐号
    }

    return true;
}

bool RealmdSocket::_handleReconnectChallenge(void)
{
    // 重登录请求
    DEBUG_LOG("Entering _handleReconnectChallenge");
    if (size() < sizeof(sAuthLogonChallenge_C))
        return false;

    // 接收数据包
    std::vector<BYTE_t> buf;
    buf.resize(4);

    recv((BYTE_t*)&buf[0], 4);

    std::uint16_t remaining = ((sAuthLogonChallenge_C*)&buf[0])->size;
    DEBUG_LOG("[ReconnectChallenge] got header, body is %#04x bytes", remaining);

    if ((remaining < sizeof(sAuthLogonChallenge_C)-buf.size()) || (size() < remaining))
        return false;

    buf.resize(remaining + buf.size() + 1);
    buf[buf.size() - 1] = 0;
    sAuthLogonChallenge_C* ch = (sAuthLogonChallenge_C*)&buf[0];

    recv((BYTE_t*)&buf[4], remaining);
    DEBUG_LOG("[ReconnectChallenge] got full packet, %#04x bytes", ch->size);
    DEBUG_LOG("[ReconnectChallenge] name(%d): '%s'", ch->I_len, ch->I);

    loginName_ = (const char*)ch->I;
    safeLoginName_ = loginName_;
    sLoginDB.escapeString(safeLoginName_);
    build_ = ch->build;

    // 检查版本
    switch (build_)
    {
    case 5875:                                          // 1.12.1
    case 6005:                                          // 1.12.2
    case 6141:                                          // 1.12.3
        break;
    default:
        close();
        return false;
    }

    // 查询sessionKey和帐号id
    SqlResultSetPtr result = sLoginDB.pquery("SELECT sessionkey,id FROM account WHERE username = '%s'", 
        safeLoginName_.c_str());
    if (!result)
    {
        ERROR_LOG("[ERROR] user %s tried to login and we can not find his session key in the database.", 
            loginName_.c_str());
        close();
        return false;
    }

    K_.setHexStr((*result)[0]->getCString());
    accountId_ = (*result)[1]->getUInt32();

    // 发送响应
    ByteBuffer pkt;
    pkt << (BYTE_t)CMD_AUTH_RECONNECT_CHALLENGE;
    pkt << (BYTE_t)0x00;
    reconnectProof_.setRand(16 * 8);
    pkt.append(reconnectProof_.asByteArray(16), 16);        // 16 bytes random
    pkt << (std::uint64_t)0x00 << (std::uint64_t)0x00;                  // 16 bytes zeros
    send((const BYTE_t*)pkt.contents(), pkt.size());
    return true;
}

bool RealmdSocket::_handleReconnectProof(void)
{
    // 重登录验证
    DEBUG_LOG("Entering _handleReconnectProof");
    
    // 接收数据包
    sAuthReconnectProof_C lp;
    if (!recv((BYTE_t*)&lp, sizeof(sAuthReconnectProof_C)))
        return false;

    if (loginName_.empty() || !reconnectProof_.getNumBytes() || !K_.getNumBytes())
        return false;

    BigNumber t1;
    t1.setBinary(lp.R1, 16);

    Sha1Hash sha;
    sha.initialize();
    sha.updateData(loginName_);
    sha.updateBigNumbers(&t1, &reconnectProof_, &K_, NULL);
    sha.finalize();

    if (!memcmp(sha.getDigest(), lp.R2, SHA_DIGEST_LENGTH))
    {
        ///- Sending response
        ByteBuffer pkt;
        pkt << (BYTE_t)CMD_AUTH_RECONNECT_PROOF;
        pkt << (BYTE_t)0x00;
        //If we keep from sending this we don't receive Session Expired on the client when
        //changing realms after being logged on to the world
        if (build_ > 6141) // Last vanilla, 1.12.3
            pkt << (std::uint16_t)0x00;                               // 2 bytes zeros
        send((BYTE_t const*)pkt.contents(), pkt.size());

        // 验证成功
        bAuthed_ = true;

        return true;
    }
    else
    {
        ERROR_LOG("[ERROR] user %s tried to login, but session invalid.", loginName_.c_str());
        close();
        return false;
    }
}

bool RealmdSocket::_handleRealmList(void)
{
    // 获取服务器列表
    DEBUG_LOG("Entering _handleRealmList");
    if (size() < 5)
    {
        return false;
    }
    skip(5);

    ByteBuffer pkt;
    _loadRealmlist(pkt);

    ByteBuffer hdr;
    hdr << (BYTE_t)CMD_REALM_LIST;
    hdr << (std::uint16_t)pkt.size();
    hdr.append(pkt);

    send((BYTE_t*)hdr.contents(), hdr.size());
    return true;
}

bool RealmdSocket::_handleXferAccept(void)
{
    // 不支持
    close();
    return false;
}

bool RealmdSocket::_handleXferResume(void)
{
    // 不支持
    close();
    return false;
}

bool RealmdSocket::_handleXferCancel(void)
{
    // 不支持
    close();
    return false;
}

///////////////////////////////////////////////////////////////////////////////
void RealmdSocket::_setVSField(const std::string& rI)
{
    // 计算v,s

    s_.setRand(s_BYTE_SIZE * 8);

    BigNumber I;
    I.setHexStr(rI.c_str());

    std::uint8_t digest[SHA_DIGEST_LENGTH];
    memset(digest, 0, SHA_DIGEST_LENGTH);
    if (I.getNumBytes() <= SHA_DIGEST_LENGTH)
        memcpy(digest, I.asByteArray(), I.getNumBytes());
    std::reverse(digest, digest + SHA_DIGEST_LENGTH);

    Sha1Hash sha;
    sha.updateData(s_.asByteArray(), s_.getNumBytes());
    sha.updateData(digest, SHA_DIGEST_LENGTH);
    sha.finalize();

    BigNumber x;
    x.setBinary(sha.getDigest(), sha.getLength());

    v_ = g_.modExp(x, N_);

    // 保存v,s到数据库
    const char *v_hex, *s_hex;
    v_hex = v_.asHexStr();
    s_hex = s_.asHexStr();
    sLoginDB.pexecute("UPDATE account SET v = '%s', s = '%s' WHERE username = '%s'",
        v_hex, s_hex, safeLoginName_.c_str());
    BigNumber::free((void*)v_hex);
    BigNumber::free((void*)s_hex);
}

void RealmdSocket::_sendProof(Sha1Hash &sha)
{
    // 发送登录验证响应数据

    sAuthLogonProof_S_BUILD_6005 proof;
    memcpy(proof.M2, sha.getDigest(), 20);
    proof.cmd = CMD_AUTH_LOGON_PROOF;
    proof.error = 0;
    proof.unk2 = 0x00;

    send((BYTE_t*)&proof, sizeof(proof));
}

void RealmdSocket::_loadRealmlist(ByteBuffer &pkt)
{
    // 更新服务器列表
    sRealmList.update();

    // 获取服务器列表的一个拷贝
    RealmList::RealmArray realms = sRealmList.getRealms();

    pkt << std::uint32_t(0);
    pkt << BYTE_t(realms.size());

    // 遍历所有服务器
    RealmList::iterator iter = realms.begin();
    for (; iter != realms.end(); ++iter)
    {
        // 查询帐号在该服务器角色数量
        SqlResultSetPtr result = sLoginDB.pquery(
            "SELECT numchars FROM realmcharacters WHERE realmid = %d AND acctid = %d;",
            iter->id, accountId_);

        BYTE_t chars = 0;
        if (result)
            chars = (*result)[0]->getUInt8();

        pkt << std::uint32_t(iter->icon);       // icon
        pkt << BYTE_t(iter->realmflags);        // 服务器标志
        pkt << iter->name;                      // 服务器名
        pkt << iter->address;                   // 服务器地址
        pkt << iter->populationLevel;           // population
        pkt << chars;                           // 帐号在该服务器角色数量
        pkt << iter->timezone;                  // 服务器时区
        pkt << BYTE_t(0x00);                    // unk
    }

    pkt << uint16_t(0x0002);
}