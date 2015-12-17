///////////////////////////////////////////////////////////////////////////////
// File:        Realmd.h
// Author:      671643387@qq.com
// Date:        2015年12月17日 下午1:51:11
// Description:
///////////////////////////////////////////////////////////////////////////////

#ifndef REALMD_REALMD_H_
#define REALMD_REALMD_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif

enum eAuthCmd
{
    CMD_AUTH_LOGON_CHALLENGE = 0x00,
    CMD_AUTH_LOGON_PROOF = 0x01,
    CMD_AUTH_RECONNECT_CHALLENGE = 0x02,
    CMD_AUTH_RECONNECT_PROOF = 0x03,
    CMD_REALM_LIST = 0x10,
    CMD_XFER_INITIATE = 0x30,
    CMD_XFER_DATA = 0x31,

    CMD_XFER_ACCEPT = 0x32,
    CMD_XFER_RESUME = 0x33,
    CMD_XFER_CANCEL = 0x34
};

enum eAuthResult
{
    WOW_SUCCESS = 0x00,
    WOW_FAIL_UNKNOWN0 = 0x01,
    WOW_FAIL_UNKNOWN1 = 0x02,
    WOW_FAIL_BANNED = 0x03,
    WOW_FAIL_UNKNOWN_ACCOUNT = 0x04,
    WOW_FAIL_INCORRECT_PASSWORD = 0x05,
    WOW_FAIL_ALREADY_ONLINE = 0x06,
    WOW_FAIL_NO_TIME = 0x07,
    WOW_FAIL_DB_BUSY = 0x08,
    WOW_FAIL_VERSION_INVALID = 0x09,
    WOW_FAIL_VERSION_UPDATE = 0x0A,
    WOW_FAIL_INVALID_SERVER = 0x0B,
    WOW_FAIL_SUSPENDED = 0x0C,
    WOW_FAIL_FAIL_NOACCESS = 0x0D,
    WOW_SUCCESS_SURVEY = 0x0E,
    WOW_FAIL_PARENTCONTROL = 0x0F,
    WOW_FAIL_LOCKED_ENFORCED = 0x10,
    WOW_FAIL_TRIAL_ENDED = 0x11,
    WOW_FAIL_USE_BATTLENET = 0x12
};

#endif /* REALMD_REALMD_H_ */
