///////////////////////////////////////////////////////////////////////////////
// File:        Common.h
// Author:      671643387@qq.com
// Date:        2015年12月17日 下午2:15:51
// Description:
///////////////////////////////////////////////////////////////////////////////

#ifndef COMMON_COMMON_H_
#define COMMON_COMMON_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif

#include <string>
#include <vector>

enum eAccountTypes
{
    SEC_PLAYER = 0,
    SEC_MODERATOR = 1,
    SEC_GAMEMASTER = 2,
    SEC_ADMINISTRATOR = 3,
    SEC_CONSOLE = 4                                  // must be always last in list, accounts must have less security level always also
};

enum LocaleConstant
{
    LOCALE_enUS = 0,                                        // also enGB
    LOCALE_koKR = 1,
    LOCALE_frFR = 2,
    LOCALE_deDE = 3,
    LOCALE_zhCN = 4,
    LOCALE_zhTW = 5,
    LOCALE_esES = 6,
    LOCALE_esMX = 7
};

#define MAX_LOCALE 8
#define DEFAULT_LOCALE LOCALE_enUS

LocaleConstant GetLocaleByName(const std::string& name);

typedef std::vector<std::string> StringVector;

struct LocaleNameStr
{
    char const* name;
    LocaleConstant locale;
};

#endif /* COMMON_COMMON_H_ */
