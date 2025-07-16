#include "resp_value.h"

#include <variant>
#include <vector>
#include <cstdint>
#include <string>

namespace Smriti {

RespValue::RespValue(RespTypeEnum type, RespTypes data)
    : d_type(type), d_data(std::move(data)) {}

RespValue RespValue::simple_string(std::string s) {
    return RespValue{RespTypeEnum::SimpleString, std::move(s)};
}

RespValue RespValue::error(std::string s) {
    return RespValue{RespTypeEnum::Error, std::move(s)};
}

RespValue RespValue::integer(int64_t i) {
    return RespValue{RespTypeEnum::Integer, i};
}

RespValue RespValue::bulk_string(std::string s) {
    return RespValue{RespTypeEnum::BulkString, std::move(s)};
}

RespValue RespValue::array(std::vector< RespValue > arr) {
    return RespValue{RespTypeEnum::Array, std::move(arr)};
}

RespValue RespValue::null() {
    return RespValue{RespTypeEnum::Null, std::string{}};
}


// Accessors for the data
const std::string& RespValue::as_string() const{
    return std::get<std::string>(d_data);
}

int64_t RespValue::as_integer() const {
    return std::get<int64_t>(d_data);
}

const std::vector< RespValue >& RespValue::as_array() const {
    return std::get<std::vector< RespValue >>(d_data);
}

RespTypeEnum RespValue::type() const {
    return d_type;
}
RespTypes RespValue::data() const {
    return d_data;
}

std::string RespValue::to_resp_string() const {
    switch (d_type) {
        case RespTypeEnum::SimpleString:
            return "+" + std::get<std::string>(d_data) + "\r\n";
        case RespTypeEnum::Error:
            return "-" + std::get<std::string>(d_data) + "\r\n";
        case RespTypeEnum::Integer:
            return ":" + std::to_string(std::get<int64_t>(d_data)) + "\r\n";
        case RespTypeEnum::BulkString: {
            const auto& str = std::get<std::string>(d_data);
            return "$" + std::to_string(str.size()) + "\r\n" + str + "\r\n";
        }
        case RespTypeEnum::Array: {
            const auto& arr = std::get<std::vector< RespValue >>(d_data);
            std::string result = "*" + std::to_string(arr.size()) + "\r\n";
            for (const auto& item : arr) {
                result += item.to_resp_string();
            }
            return result;
        }
        case RespTypeEnum::Null:
            return "$-1\r\n";
    }
    return ""; // Should never reach here
}

} // namespace Smriti
