#include "parser.h"
#include "convertors.h"

#include <string_view>
#include <optional>
#include <charconv>

namespace Smriti {

std::optional<std::string_view> read_line(const char* data, size_t length) {
    const char* start = data;
    const char* end = data + length;
    const char* current = start;

    // Look for \r\n sequence
    while (current < end - 1) {
        if (*current == '\r' && *(current + 1) == '\n') {
            std::string_view result(start, current - start);
            current += 2; // Skip \r\n
            return result;
        }
        ++current;
    }

    return std::nullopt;
}

std::optional<RespValue> parse_simple_string(const char* data, size_t length) {
    auto line = read_line(data, length);
    if (!line) return std::nullopt;
    return RespValue::simple_string(*line);
}

std::optional<RespValue> parse_error(const char* data, size_t length) {
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

std::optional<RespValue> parse(const char* data, size_t length) {
    if (length == 0) {
        return std::nullopt;
    }

    char* current = const_cast<char*>(data);

    char type = *current++;
    --length;

    switch (type) {
        case '+': return parse_simple_string(current, length);
        case '-': return parse_error(current, length);
        case ':': return parse_integer(current, length);
        // case '$': return parse_bulk_string(data, length);
        // case '*': return parse_array(data, length);
        default:
            return std::nullopt;
    }

}

} // namespace Smriti