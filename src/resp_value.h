#ifndef INCLUDED_SMRITI_RESP_VALUE_H
#define INCLUDED_SMRITI_RESP_VALUE_H

#include <variant>
#include <vector>
#include <cstdint>
#include <string>

namespace Smriti {

class RespValue; // Forward declaration

enum class RespTypeEnum : uint8_t {
    SimpleString,
    Error,
    Integer,
    BulkString,
    Array,
    Null
};

using RespTypes = std::variant< std::string, int64_t, std::vector< RespValue > >;

class RespValue {
public:
    RespTypeEnum type() const;
    RespTypes data() const;

    // Constructors for each type
    static RespValue simple_string(std::string s);
    static RespValue error(std::string s);
    static RespValue integer(int64_t i);
    static RespValue bulk_string(std::string s);
    static RespValue array(std::vector< RespValue > arr);
    static RespValue null();

    // Accessors for the data
    const std::string& as_string() const;
    int64_t as_integer() const;
    const std::vector< RespValue >& as_array() const;
    inline bool is_null() const {
        return d_type == RespTypeEnum::Null;
    }

    std::string to_resp_string() const;

private:
    // We use a private constructor to enforce the use of static methods
    explicit RespValue(RespTypeEnum type, RespTypes data);

    RespTypeEnum d_type;
    RespTypes d_data;
};



}

#endif // INCLUDED_SMRITI_RESP_VALUE_H