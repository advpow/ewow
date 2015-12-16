///////////////////////////////////////////////////////////////////////////////
// File:        BigNumber.h
// Author:      671643387@qq.com
// Date:        2015年12月16日 下午4:06:37
// Description:
///////////////////////////////////////////////////////////////////////////////

#ifndef SHARED_CRYPTO_BIGNUMBER_H_
#define SHARED_CRYPTO_BIGNUMBER_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif

#include <cstdint>
#include "Typedef.h"

struct bignum_st;

class BigNumber
{
public:
    BigNumber(void);
    BigNumber(const BigNumber& bn);
    BigNumber(uint32_t);
    ~BigNumber(void);

public:
    void setUInt32(uint32_t);
    void setUInt64(uint64_t);
    void setBinary(const uint8_t*, int);
    void setHexStr(const char*);
    void setRand(int);

    static void free(void*);

    BigNumber operator=(const BigNumber&);

    BigNumber operator+=(const BigNumber&);
    BigNumber operator+(const BigNumber& x)
    {
        BigNumber t(*this);
        return t += x;
    }

    BigNumber operator-=(const BigNumber&);
    BigNumber operator-(const BigNumber& x)
    {
        BigNumber t(*this);
        return t -= x;
    }

    BigNumber operator*=(const BigNumber&);
    BigNumber operator*(const BigNumber& x)
    {
        BigNumber t(*this);
        return t *= x;
    }

    BigNumber operator/=(const BigNumber&);
    BigNumber operator/(const BigNumber& x)
    {
        BigNumber t(*this);
        return t /= x;
    }

    BigNumber operator%=(const BigNumber&);
    BigNumber operator%(const BigNumber& x)
    {
        BigNumber t(*this);
        return t %= x;
    }

    bool isZero(void) const;
    BigNumber modExp(const BigNumber&, const BigNumber&);
    BigNumber exp(const BigNumber&);
    int getNumBytes(void);
    struct bignum_st* BN(void) { return bn_; }
    uint32_t asUInt32(void);
    BYTE_t* asByteArray(int minSize = 0);
    const char* asHexStr(void);
    const char* asDecStr(void);

private:
    struct bignum_st *bn_;
    BYTE_t           *array_;
};

#endif /* SHARED_CRYPTO_BIGNUMBER_H_ */
