#include "parser.h"

#include <string>
#include <vector>
#include <string_view>
#include <optional>
#include <charconv>

#include <iostream>

namespace Smriti {

std::optional<std::string_view> Parser::read_line() {
    size_t start = d_current_index;
    size_t end = d_data.length();

    // Look for \r\n sequence
    while (d_current_index < end - 1) {
        if (d_data.at(d_current_index) == '\r' && d_data.at(d_current_index + 1) == '\n') {
            std::string_view result(d_data.data() + start, d_current_index - start);
            d_current_index += 2; // Skip \r\n
            return result;
        }
        d_current_index++;
    }

    return std::nullopt;
}

std::optional<RespValue> Parser::parse_simple_string() {
    auto line = read_line();
    if (!line) return std::nullopt;
    return RespValue::simple_string(std::string{*line});
}

std::optional<RespValue> Parser::parse_error() {
    auto line = read_line();
    if (!line) return std::nullopt;
    return RespValue::error(std::string{*line});
}

std::optional<int64_t> Parser::parse_bare_int() {

    int64_t result = 0;
    int sign = 1;
    bool has_value = false;

    if (d_data.at(d_current_index) == '-' || d_data.at(d_current_index) == '+') {

        sign = (d_data.at(d_current_index) == '-') ? -1 : 1;
        d_current_index++;
    }
    while (d_current_index < d_data.length() &&
           d_data.at(d_current_index) >= '0' &&
           d_data.at(d_current_index) <= '9') {
        result = result * 10 + (d_data.at(d_current_index) - '0');
        d_current_index++;
        has_value = true;
    }

    if (!has_value) {
        return std::nullopt; // No valid digits found
    }
    return result * sign;
}

std::optional<RespValue> Parser::parse_integer() {

    auto value_opt = parse_bare_int();
    if (!value_opt) return std::nullopt;

    int64_t value = *value_opt;

    d_current_index += 2;  // Account for \r\n

    return RespValue::integer(value);
}

std::optional<RespValue> Parser::parse_bulk_string() {

    size_t start = d_current_index;

    auto optional_line_length = parse_bare_int();

    if (!optional_line_length) return std::nullopt;
    int64_t line_length = *optional_line_length;

    d_current_index += 2; // Skip the \r\n after the length

    // RESP2 uses -1 to indicate a null bulk string
    if (line_length == -1) {
        return RespValue::null();
    }

    if (d_current_index + line_length > d_data.length()) {
        std::cerr << "Not enough data for bulk string parsing.\n";
        return std::nullopt; // Not enough data left
    }

    std::string_view str(d_data.data() + d_current_index, line_length);

    d_current_index += line_length + 2;

    return RespValue::bulk_string(std::string{str});
}

std::optional<RespValue> Parser::parse_array() {

    size_t start = d_current_index;

    auto optional_array_length = parse_bare_int();
    if (!optional_array_length) return std::nullopt;

    int64_t array_length = *optional_array_length;

    d_current_index += 2; // Skip the \r\n after the length

    if (array_length == -1) {
        return RespValue::null();
    }
    if (array_length < 0) return std::nullopt;

    std::vector<RespValue> temp_array;
    temp_array.reserve(array_length);

    for (int64_t i = 0; i < array_length; ++i) {

        if (d_current_index >= d_data.length()) {
            std::cerr << "Reached end of data while parsing array element.\n";
            return std::nullopt; // Prevent out-of-bounds access
        }

        size_t start = d_current_index;
        auto element = parse();
        if (!element) {
            std::cerr << "Failed to parse array element at index " << d_current_index << "\n";
            return std::nullopt; // Failed to parse an element
        }

        if (!element) return std::nullopt;
        temp_array.push_back(std::move(*element));
    }

    return RespValue::array(std::move(temp_array));
}

std::optional<RespValue> Parser::parse() {
    if (d_data.length() == 0) {
        return std::nullopt;
    }

    char type = d_data.at(d_current_index);
    d_current_index++;

    if (d_current_index >= d_data.length()) {
        return std::nullopt;
    }

    switch (type) {
        case '+': return parse_simple_string();
        case '-': return parse_error();
        case ':': return parse_integer();
        case '$': return parse_bulk_string();
        case '*': return parse_array();
        default:
            return std::nullopt;
    }

}

} // namespace Smriti