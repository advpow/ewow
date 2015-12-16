///////////////////////////////////////////////////////////////////////////////
// File:        BigNumber.cc
// Author:      671643387@qq.com
// Date:        2015年12月16日 下午4:06:43
// Description:
///////////////////////////////////////////////////////////////////////////////

#include <openssl/bn.h>
#include <algorithm>
#include "crypto/BigNumber.h"

BigNumber::BigNumber(void)
: bn_(BN_new())
, array_(NULL)
{

}

BigNumber::BigNumber(const BigNumber& x)
: bn_(BN_dup(x.bn_))
, array_(NULL)
{

}

BigNumber::BigNumber(uint32_t x)
: bn_(BN_new())
, array_(NULL)
{
    BN_set_word(bn_, x);
}

BigNumber::~BigNumber(void)
{
    BN_free(bn_);
    if (array_)
        delete[] array_;
}

void BigNumber::setUInt32(uint32_t x)
{
    BN_set_word(bn_, x);
}

void BigNumber::setUInt64(uint64_t x)
{
    BN_add_word(bn_, (uint32_t)(x >> 32));
    BN_lshift(bn_, bn_, 32);
    BN_add_word(bn_, (uint32_t)(x & 0xFFFFFFFF));
}

void BigNumber::setBinary(const uint8_t* bytes, int len)
{
    uint8_t t[1000];
    for (int i = 0; i < len; ++i)
    {
        t[i] = bytes[len - 1 - i];
    }
    BN_bin2bn(t, len, bn_);
}

void BigNumber::setHexStr(const char* str)
{
    BN_hex2bn(&bn_, str);
}

void BigNumber::setRand(int numbits)
{
    BN_rand(bn_, numbits, 0, 1);
}

void BigNumber::free(void* x)
{
    OPENSSL_free(x);
}

BigNumber BigNumber::operator=(const BigNumber& x)
{
    BN_copy(bn_, x.bn_);
    return *this;
}

BigNumber BigNumber::operator+=(const BigNumber& x)
{
    BN_add(bn_, bn_, x.bn_);
    return *this;
}

BigNumber BigNumber::operator-=(const BigNumber& x)
{
    BN_sub(bn_, bn_, x.bn_);
    return *this;
}

BigNumber BigNumber::operator*=(const BigNumber& x)
{
    BN_CTX* bnctx;

    bnctx = BN_CTX_new();
    BN_mul(bn_, bn_, x.bn_, bnctx);
    BN_CTX_free(bnctx);

    return *this;
}

BigNumber BigNumber::operator/=(const BigNumber& x)
{
    BN_CTX* bnctx;

    bnctx = BN_CTX_new();
    BN_div(bn_, NULL, bn_, x.bn_, bnctx);
    BN_CTX_free(bnctx);

    return *this;
}

BigNumber BigNumber::operator%=(const BigNumber& x)
{
    BN_CTX* bnctx;

    bnctx = BN_CTX_new();
    BN_mod(bn_, bn_, x.bn_, bnctx);
    BN_CTX_free(bnctx);

    return *this;
}

BigNumber BigNumber::exp(const BigNumber& bn)
{
    BigNumber ret;
    BN_CTX* bnctx;

    bnctx = BN_CTX_new();
    BN_exp(ret.bn_, bn_, bn.bn_, bnctx);
    BN_CTX_free(bnctx);

    return ret;
}

BigNumber BigNumber::modExp(const BigNumber& bn1, const BigNumber& bn2)
{
    BigNumber ret;
    BN_CTX* bnctx;

    bnctx = BN_CTX_new();
    BN_mod_exp(ret.bn_, bn_, bn1.bn_, bn2.bn_, bnctx);
    BN_CTX_free(bnctx);

    return ret;
}

int BigNumber::getNumBytes(void)
{
    return BN_num_bytes(bn_);
}

uint32_t BigNumber::asUInt32()
{
    return (uint32_t)BN_get_word(bn_);
}

bool BigNumber::isZero() const
{
    return BN_is_zero(bn_) != 0;
}

uint8_t* BigNumber::asByteArray(int minSize)
{
    int length = (minSize >= getNumBytes()) ? minSize : getNumBytes();

    delete[] array_;
    array_ = new uint8_t[length];

    // If we need more bytes than length of BigNumber set the rest to 0
    if (length > getNumBytes())
    {
        memset((void*)array_, 0, length);
    }

    BN_bn2bin(bn_, (unsigned char*)array_);

    std::reverse(array_, array_ + length);

    return array_;
}

const char* BigNumber::asHexStr()
{
    return BN_bn2hex(bn_);
}

const char* BigNumber::asDecStr()
{
    return BN_bn2dec(bn_);
}