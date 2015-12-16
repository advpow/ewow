///////////////////////////////////////////////////////////////////////////////
// File:        Sha1.cc
// Author:      671643387@qq.com
// Date:        2015年12月16日 下午4:07:05
// Description:
///////////////////////////////////////////////////////////////////////////////

#include <cstdarg>
#include "crypto/BigNumber.h"
#include "crypto/Sha1.h"

Sha1Hash::Sha1Hash(void)
{
    SHA1_Init(&mC_);
}

Sha1Hash::~Sha1Hash(void)
{
    SHA1_Init(&mC_);
}

///////////////////////////////////////////////////////////////////////////////

void Sha1Hash::initialize(void)
{
    SHA1_Init(&mC_);
}

void Sha1Hash::finalize(void)
{
    SHA1_Final(mDigest, &mC_);
}

///////////////////////////////////////////////////////////////////////////////

void Sha1Hash::updateData(const BYTE_t* dat, std::size_t len)
{
    SHA1_Update(&mC_, (const void*)dat, len);
}

void Sha1Hash::updateData(const std::string& str)
{
    updateData((const BYTE_t*)str.c_str(), str.length());
}

void Sha1Hash::updateBigNumbers(BigNumber* x, ...)
{
    std::va_list v;
    BigNumber* bn;

    va_start(v, x);
    bn = x;
    while (bn)
    {
        updateData(bn->asByteArray(), bn->getNumBytes());
        bn = va_arg(v, BigNumber*);
    }
    va_end(v);
}