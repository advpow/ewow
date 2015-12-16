///////////////////////////////////////////////////////////////////////////////
// File:        SqlField.h
// Author:      671643387@qq.com
// Date:        2015年12月16日 下午4:37:34
// Description:
///////////////////////////////////////////////////////////////////////////////

#ifndef SHARED_DATABASE_SQLFIELD_H_
#define SHARED_DATABASE_SQLFIELD_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif

#include <cstdint>
#include <string>

class SqlField
{
public:
    enum DataTypes
    {
        DB_TYPE_UNKNOWN = 0x00,
        DB_TYPE_STRING = 0x01,
        DB_TYPE_INTEGER = 0x02,
        DB_TYPE_FLOAT = 0x03,
        DB_TYPE_BOOL = 0x04
    };
public:
    SqlField(void);
    SqlField(const char* value, enum DataTypes type);
    ~SqlField(void);

public:
    void setType(enum DataTypes type) { type_ = type; }
    enum DataTypes getType(void) const { return type_; }
    void setValue(const char* value) { value_ = value; }

    bool isNull(void) const { return value_ == NULL; }
    const char* getCString(void) const { return value_; }
    std::string getCppString(void) const { return std::string(((value_ != NULL) ? value_ : "")); }
    float getFloat() const { return value_ ? static_cast<float>(atof(value_)) : 0.0f; }
    bool getBool() const { return value_ ? atoi(value_) > 0 : false; }
    double getDouble() const { return value_ ? static_cast<double>(atof(value_)) : 0.0f; }
    int8_t getInt8() const { return value_ ? static_cast<int8_t>(atol(value_)) : int8_t(0); }
    int32_t getInt32() const { return value_ ? static_cast<int32_t>(atol(value_)) : int32_t(0); }
    uint8_t getUInt8() const { return value_ ? static_cast<uint8_t>(atol(value_)) : uint8_t(0); }
    uint16_t getUInt16() const { return value_ ? static_cast<uint16_t>(atol(value_)) : uint16_t(0); }
    int16_t getInt16() const { return value_ ? static_cast<int16_t>(atol(value_)) : int16_t(0); }
    uint32_t getUInt32() const { return value_ ? static_cast<uint32_t>(atol(value_)) : uint32_t(0); }
    uint64_t getUInt64() const;
    int64_t getInt64() const;

private:
    const char*     value_;
    enum DataTypes  type_;
};

#endif /* SHARED_DATABASE_SQLFIELD_H_ */
