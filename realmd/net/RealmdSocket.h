///////////////////////////////////////////////////////////////////////////////
// File:        RealmdSocket.h
// Author:      671643387@qq.com
// Date:        2015年12月16日 下午2:01:53
// Description:
///////////////////////////////////////////////////////////////////////////////

#ifndef REALMD_NET_REALMDSOCKET_H_
#define REALMD_NET_REALMDSOCKET_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif

#include "crypto/BigNumber.h"
#include "crypto/Sha1.h"
#include "util/ByteBuffer.h"
#include "net/Socket.h"
#include "Common.h"

class RealmdSocket : public Socket
{
public:
    RealmdSocket(void);
    virtual ~RealmdSocket(void);

public:
    bool _handleLogonChallenge(void);
    bool _handleLogonProof(void);
    bool _handleReconnectChallenge(void);
    bool _handleReconnectProof(void);
    bool _handleRealmList(void);
    bool _handleXferAccept(void);
    bool _handleXferResume(void);
    bool _handleXferCancel(void);

protected:
    virtual void onRead(void);
    virtual void onClose(void);

private:
    void _setVSField(const std::string& rI);
    void _sendProof(Sha1Hash &sha);
    void _loadRealmlist(ByteBuffer &pkt);

private:
    BigNumber N_, s_, g_, v_;
    BigNumber b_, B_;
    BigNumber K_;
    BigNumber reconnectProof_;

    bool bAuthed_;

    std::uint32_t accountId_;

    std::string loginName_;
    std::string safeLoginName_;

    std::string localizationName_;
    std::uint16_t build_;
    eAccountTypes accountSecurityLevel_;
};

#endif /* REALMD_NET_REALMDSOCKET_H_ */
