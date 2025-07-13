#include "parser.h"
#include "convertors.h"

#include <string_view>
#include <optional>
#include <charconv>

#include <iostream>

namespace Smriti {

std::optional<std::string_view> read_line(char*& data, size_t length) {
    const char* start = data;
    const char* end = data + length;

    // Look for \r\n sequence
    while (data < end - 1) {
        if (*data == '\r' && *(data + 1) == '\n') {
            std::string_view result(start, data - start);
            data += 2; // Skip \r\n
            return result;
        }
        ++data;
    }

    return std::nullopt;
}

std::optional<RespValue> parse_simple_string(char*& data, size_t length) {
    auto line = read_line(data, length);
    if (!line) return std::nullopt;
    return RespValue::simple_string(*line);
}

std::optional<RespValue> parse_error(char*& data, size_t length) {
    auto line = read_line(data, length);
    if (!line) return std::nullopt;
    return RespValue::error(*line);
}

std::optional<RespValue> parse_integer(char*& data, size_t length) {

    auto value_opt = Smriti::parse_int(data, length);
    if (!value_opt) return std::nullopt;

    int64_t value = *value_opt;

    return RespValue::integer(value);
}

std::optional<RespValue> parse_bulk_string(char*& data, size_t length) {

    char* start = data;

    auto optional_line_length = parse_int(data, length);
    if (!optional_line_length) return std::nullopt;

    int64_t line_length = *optional_line_length;

    data += 2; // Skip the \r\n after the length

    // RESP2 uses -1 to indicate a null bulk string
    if (line_length == -1) {
        return RespValue::null();
    }

    int number_length = data - start;
    if (number_length + line_length + 2 > length) {
        return std::nullopt; // Not enough data for the bulk string
    }

    std::string_view str(data, data + line_length);

    data = data + line_length + 2;

    return RespValue::bulk_string(str);
}

std::optional<RespValue> parse_array(char*& data, size_t length) {

    char* start = data;

    auto optional_array_length = parse_int(data, length);
    if (!optional_array_length) return std::nullopt;

    int64_t array_length = *optional_array_length;
    int number_length = data - start;

    data += 2; // Skip the \r\n after the length
    if (array_length == -1) {
        return RespValue::null();
    }
    if (length < 0) return std::nullopt;

    if (number_length + 2 > length) {
        return std::nullopt; // Not enough data for the bulk string
    }

    std::vector<RespValue> temp_array;
    temp_array.reserve(array_length);

    for (int64_t i = 0; i < array_length; ++i) {
        char* start = data;
        auto element = parse(data, length);
        int parsed_length = data - start;
        length -= parsed_length;
        if (length < 0) return std::nullopt; // Not enough data left

        if (!element) return std::nullopt;
        temp_array.push_back(std::move(*element));
    }

    return RespValue::array(std::move(temp_array));
}

std::optional<RespValue> parse(char*& data, size_t length) {
    if (length == 0) {
        return std::nullopt;
    }

    char type = *data++;
    --length;

    if (length == 0) {
        return std::nullopt;
    }

    switch (type) {
        case '+': return parse_simple_string(data, length);
        case '-': return parse_error(data, length);
        case ':': return parse_integer(data, length);
        case '$': return parse_bulk_string(data, length);
        case '*': return parse_array(data, length);
        default:
            return std::nullopt;
    }

}

} // namespace Smriti