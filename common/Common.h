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

enum eAccountTypes
{
    SEC_PLAYER = 0,
    SEC_MODERATOR = 1,
    SEC_GAMEMASTER = 2,
    SEC_ADMINISTRATOR = 3,
    SEC_CONSOLE = 4                                  // must be always last in list, accounts must have less security level always also
};

#endif /* COMMON_COMMON_H_ */
