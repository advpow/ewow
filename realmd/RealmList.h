///////////////////////////////////////////////////////////////////////////////
// File:        RealmdList.h
// Author:      671643387@qq.com
// Date:        2015年12月17日 下午1:51:17
// Description:
///////////////////////////////////////////////////////////////////////////////

#ifndef REALMD_REALMDLIST_H_
#define REALMD_REALMDLIST_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif

#include <cstdint>
#include <string>
#include <vector>
#include <boost/thread/mutex.hpp>
#include "Common.h"
#include "util/Time.h"

struct Realm
{
    uint32_t        id;
    std::string     name;
    std::string     address;
    uint8_t         icon;
    eRealmFlags     realmflags;
    uint8_t         timezone;
    float           populationLevel;
};

class RealmList
{
public:
    typedef std::vector<Realm>      RealmArray;
    typedef RealmArray::iterator    iterator;
    typedef RealmArray::size_type   size_type;

public:
    RealmList(void);
    ~RealmList(void);

    void update(void);
    RealmArray getRealms(void) const;

    void clear(void) { realms_.clear(); }

private:
    TIME_t               updateTime_;
    RealmArray           realms_;
    mutable boost::mutex realmsLOCK_;
};

#endif /* REALMD_REALMDLIST_H_ */
