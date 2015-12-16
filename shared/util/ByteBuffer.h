///////////////////////////////////////////////////////////////////////////////
// File:        ByteBuffer.h
// Author:      671643387@qq.com
// Date:        2015年12月16日 下午3:55:49
// Description:
///////////////////////////////////////////////////////////////////////////////

#ifndef SHARED_UTIL_BYTEBUFFER_H_
#define SHARED_UTIL_BYTEBUFFER_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <vector>
#include <memory.h>
#include "Typedef.h"

class ByteBufferException
{
public:
    ByteBufferException(bool add, std::size_t pos, 
        std::size_t esize, std::size_t size)
        : add_(add), pos_(pos), esize_(esize), size_(size)
    {
        PrintPosError();
    }

    void PrintPosError() const;
private:
    bool        add_;
    std::size_t pos_;
    std::size_t esize_;
    std::size_t size_;
};

class ByteBuffer
{
public:
    const static std::size_t DEFAULT_SIZE = 0x1000;
    const static std::size_t MAX_SIZE = 10000000;

    typedef ByteBuffer  Self;

public:
    ByteBuffer(void)
        : rpos_(0)
        , wpos_(0)
        , storage_()
    {
        storage_.reserve(DEFAULT_SIZE);
    }

    ByteBuffer(std::size_t size)
        : rpos_(0)
        , wpos_(0)
        , storage_(size)
    {

    }

    ~ByteBuffer(void)
    {

    }

public:
    std::vector<std::uint8_t>::size_type size(void) const
    {
        return storage_.size();
    }

    const std::uint8_t* contents(void) const
    {
        return &storage_[0];
    }

    Self& operator<<(std::uint8_t x)
    {
        append<std::uint8_t>(x);
        return *this;
    }

    Self& operator<<(std::uint16_t x)
    {
        append<std::uint16_t>(x);
        return *this;
    }

    Self& operator<<(std::uint32_t x)
    {
        append<std::uint32_t>(x);
        return *this;
    }

    Self& operator<<(std::uint64_t x)
    {
        append<std::uint64_t>(x);
        return *this;
    }

    Self& operator<<(float x)
    {
        append(x);
        return *this;
    }

    Self& operator<<(const char* x)
    {
        append(x, strlen(x));
        *this << (std::uint8_t)0;
        return *this;
    }

    Self& operator<<(const std::string& x)
    {
        append(x.c_str(), x.length());
        *this << (std::uint8_t)0;
        return *this;
    }

    void append(const char* src, std::size_t len)
    {
        if (len == 0)
            return;

        assert((len + wpos_) < MAX_SIZE);
        if (storage_.size() < (len + wpos_))
            storage_.resize(len + wpos_);
        memcpy(&storage_[wpos_], src, len);
        wpos_ += len;
    }

    void append(const std::uint8_t* src, std::size_t len)
    {
        append((const char*)src, len);
    }

    void append(const ByteBuffer& bbuf)
    {
        append(bbuf.contents(), bbuf.size());
    }

private:
    template<typename T>
    void append(T x)
    {
        append((const char*)&x, sizeof(T));
    }

private:
    std::size_t                 rpos_;
    std::size_t                 wpos_;
    std::vector<std::uint8_t>   storage_;
};

#endif /* SHARED_UTIL_BYTEBUFFER_H_ */
