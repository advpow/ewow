///////////////////////////////////////////////////////////////////////////////
// File:        CircleBuffer.h
// Author:      671643387@qq.com
// Date:        2015年12月16日 下午2:25:52
// Description:
///////////////////////////////////////////////////////////////////////////////

#ifndef SHARED_UTIL_CIRCLEBUFFER_H_
#define SHARED_UTIL_CIRCLEBUFFER_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif

#include <vector>
#include <boost/noncopyable.hpp>
#include "Typedef.h"

class CircleBuffer : public boost::noncopyable
{
public:
    CircleBuffer(std::size_t size);
    ~CircleBuffer(void);

public:
    std::size_t read(BYTE_t *, std::size_t);
    std::size_t write(const BYTE_t *, std::size_t);
    std::size_t peek(BYTE_t *, std::size_t);
    std::size_t skip(std::size_t);
    std::size_t capacity(void) { return capacity_; }
    std::size_t space(void) { return capacity_ - size_; }
    std::size_t size(void) { return size_; }

private:
    std::size_t             capacity_;
    std::size_t             size_;
    std::size_t             rpos_;
    std::size_t             wpos_;
    std::vector<BYTE_t>     buf_;
};

#endif /* SHARED_UTIL_CIRCLEBUFFER_H_ */
