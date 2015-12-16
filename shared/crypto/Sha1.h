///////////////////////////////////////////////////////////////////////////////
// File:        Sha1.h
// Author:      671643387@qq.com
// Date:        2015年12月16日 下午4:06:59
// Description:
///////////////////////////////////////////////////////////////////////////////

#ifndef SHARED_CRYPTO_SHA1_H_
#define SHARED_CRYPTO_SHA1_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif

#include <cstddef>
#include <cstdint>
#include <string>
#include <openssl/sha.h>
#include <openssl/crypto.h>
#include "Typedef.h"

class BigNumber;

class Sha1Hash
{
public:
    Sha1Hash(void);
    ~Sha1Hash(void);

public:
    void initialize(void);
    void finalize(void);

    void updateBigNumbers(BigNumber*, ...);
    void updateData(const BYTE_t*, std::size_t);
    void updateData(const std::string&);

    const BYTE_t* getDigest(void) const { return &mDigest[0]; }
    int getLength(void) const { return SHA_DIGEST_LENGTH; }

private:
    SHA_CTX     mC_;
    BYTE_t      mDigest[SHA_DIGEST_LENGTH];
};

#endif /* SHARED_CRYPTO_SHA1_H_ */
