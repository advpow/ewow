///////////////////////////////////////////////////////////////////////////////
// File:        Common.cc
// Author:      671643387@qq.com
// Date:        2015年12月17日 下午2:16:26
// Description:
///////////////////////////////////////////////////////////////////////////////

#include "Common.h"

char const* localeNames[MAX_LOCALE] =
{
    "enUS",                                                 // also enGB
    "koKR",
    "frFR",
    "deDE",
    "zhCN",
    "zhTW",
    "esES",
    "esMX",
};

// used for search by name or iterate all names
LocaleNameStr const fullLocaleNameList[] =
{
    { "enUS", LOCALE_enUS },
    { "enGB", LOCALE_enUS },
    { "koKR", LOCALE_koKR },
    { "frFR", LOCALE_frFR },
    { "deDE", LOCALE_deDE },
    { "zhCN", LOCALE_zhCN },
    { "zhTW", LOCALE_zhTW },
    { "esES", LOCALE_esES },
    { "esMX", LOCALE_esMX },
    { NULL, LOCALE_enUS }
};

LocaleConstant GetLocaleByName(const std::string& name)
{
    for (LocaleNameStr const* itr = &fullLocaleNameList[0]; itr->name; ++itr)
    if (name == itr->name)
    {
        return itr->locale;
    }

    return LOCALE_enUS;                                     // including enGB case
}