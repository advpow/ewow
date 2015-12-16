///////////////////////////////////////////////////////////////////////////////
// File:        CircleBuffer.cc
// Author:      671643387@qq.com
// Date:        2015年12月16日 下午2:25:56
// Description:
///////////////////////////////////////////////////////////////////////////////

#include <cassert>
#include "util/CircleBuffer.h"

CircleBuffer::CircleBuffer(std::size_t size)
: capacity_(size)
, size_(0)
, rpos_(0)
, wpos_(0)
{
    assert(size > 0);
    buf_.resize(capacity_);
}

CircleBuffer::~CircleBuffer(void)
{

}

std::size_t CircleBuffer::read(BYTE_t *buf, std::size_t len)
{
    if (size_ == 0)
        return 0;

    std::size_t bytsCpy = __MIN_(size_, len);

    if (bytsCpy <= capacity_ - rpos_)
    {
        memcpy(buf, &buf_[rpos_], bytsCpy);
        rpos_ += bytsCpy;
        if (rpos_ == capacity_)
            rpos_ = 0;
    }
    else
    {
        std::size_t size1 = capacity_ - rpos_;
        memcpy(buf, &buf_[rpos_], size1);
        std::size_t size2 = bytsCpy - size1;
        memcpy(buf + size1, &buf_[0], size2);
        rpos_ = size2;
    }
    size_ -= bytsCpy;

    return bytsCpy;
}

std::size_t CircleBuffer::peek(BYTE_t *buf, std::size_t len)
{
    if (size_ == 0)
        return 0;

    std::size_t bytsCpy = __MIN_(size_, len);

    if (bytsCpy <= capacity_ - rpos_)
    {
        memcpy(buf, &buf_[rpos_], bytsCpy);
    }
    else
    {
        std::size_t size1 = capacity_ - rpos_;
        memcpy(buf, &buf_[rpos_], size1);
        std::size_t size2 = bytsCpy - size1;
        memcpy(buf + size1, &buf_[0], size2);
    }

    return bytsCpy;
}

std::size_t CircleBuffer::skip(std::size_t len)
{
    if (size_ == 0)
        return 0;

    std::size_t bytsSkp = __MIN_(size_, len);

    if (bytsSkp <= capacity_ - rpos_)
    {
        rpos_ += bytsSkp;
        if (rpos_ == capacity_)
            rpos_ = 0;
    }
    else
        rpos_ = bytsSkp - (capacity_ - rpos_);

    size_ -= bytsSkp;
    return bytsSkp;
}

std::size_t CircleBuffer::write(const BYTE_t *buf, std::size_t len)
{
    if (space() == 0)
        return 0;

    std::size_t bytsWrit = __MIN_(space(), len);

    if (bytsWrit <= capacity_ - wpos_)
    {
        memcpy(&buf_[wpos_], buf, bytsWrit);
        wpos_ += bytsWrit;
        if (wpos_ == capacity_)
            wpos_ = 0;
    }
    else
    {
        std::size_t size1 = capacity_ - wpos_;
        memcpy(&buf_[wpos_], buf, size1);
        std::size_t size2 = bytsWrit - size1;
        memcpy(&buf_[0], buf + size1, size2);
        wpos_ = size2;
    }
    size_ += bytsWrit;

    return bytsWrit;
}