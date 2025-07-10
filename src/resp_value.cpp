#include "resp_value.h"

#include <variant>
#include <vector>
#include <cstdint>
#include <string_view>

namespace Smriti {

RespValue::RespValue(RespTypeEnum type, RespTypes data)
    : d_type(type), d_data(std::move(data)) {}

RespValue RespValue::simple_string(std::string_view s) {
    return RespValue{RespTypeEnum::SimpleString, s};
}

RespValue RespValue::error(std::string_view s) {
    return RespValue{RespTypeEnum::Error, s};
}

RespValue RespValue::integer(int64_t i) {
    return RespValue{RespTypeEnum::Integer, i};
}

RespValue RespValue::bulk_string(std::string_view s) {
    return RespValue{RespTypeEnum::BulkString, s};
}

RespValue RespValue::array(std::vector< RespValue > arr) {
    return RespValue{RespTypeEnum::Array, std::move(arr)};
}

RespValue RespValue::null() {
    return RespValue{RespTypeEnum::Null, std::string_view{}};
}


// Accessors for the data
const std::string_view& RespValue::as_string() const{
    return std::get<std::string_view>(d_data);
}

int64_t RespValue::as_integer() const {
    return std::get<int64_t>(d_data);
}

const std::vector< RespValue >& RespValue::as_array() const {
    return std::get<std::vector< RespValue >>(d_data);
}

RespTypeEnum RespValue::type() {
    return d_type;
}
RespTypes RespValue::data() const {
    return d_data;
}

} // namespace Smriti
