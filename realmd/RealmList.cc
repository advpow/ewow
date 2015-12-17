///////////////////////////////////////////////////////////////////////////////
// File:        RealmdList.cc
// Author:      671643387@qq.com
// Date:        2015年12月17日 下午1:51:22
// Description:
///////////////////////////////////////////////////////////////////////////////

#include "Log.h"
#include "Realmd.h"
#include "RealmList.h"
#include "database/SqlDatabase.h"

#define REALM_UPDATE_INTERVAL   60000

extern MysqlDatabase sLoginDB;

RealmList::RealmList(void)
: updateTime_(0)
{

}

RealmList::~RealmList(void)
{

}

void RealmList::update(void)
{
    TIME_t now = fromStartup();
    if ((now - updateTime_) < REALM_UPDATE_INTERVAL)
        return;

    updateTime_ = now;
    realms_.clear();

    DEBUG_LOG("load realms from database");

    SqlResultSetPtr result = sLoginDB.pquery("SELECT * FROM realmlist;");
    if (result)
    {
        do
        {
            struct Realm realm = { 0 };
            realm.id = (*result)[0]->getUInt32();
            realm.name = (*result)[1]->getCppString();
            realm.address.append((*result)[2]->getCString())
                .append(":").append((*result)[3]->getCString());
            realm.icon = (*result)[4]->getUInt8();
            realm.realmflags = (eRealmFlags)(*result)[5]->getUInt8();
            realm.timezone = (*result)[6]->getUInt8();
            realm.populationLevel = (*result)[8]->getFloat();
            realms_.push_back(realm);
        } while (result->nextRow());
    }
}