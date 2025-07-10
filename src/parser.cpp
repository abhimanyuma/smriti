#include "parser.h"

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

// std::optional<RespValue> parse_integer(const char* data, size_t length) {
//     auto line = read_line(data, length);
//     if (!line) return std::nullopt;

//     int64_t value;
//     auto result = std::from_chars(line->data(), line->data() + line->size(), value);
//     if (result.ec != std::errc{}) {
//         return std::nullopt;
//     }

//     return RespValue::integer(value);
// }

std::optional<RespValue> parse(const char* data, size_t length) {
    if (length == 0) {
        return std::nullopt;
    }

    char type = *data++;
    --length;

    switch (type) {
        case '+': return parse_simple_string(data, length);
        case '-': return parse_error(data, length);
        // Unimplemented RESP types
        // case ':': return parse_integer(data, length);
        // case '$': return parse_bulk_string(data, length);
        // case '*': return parse_array(data, length);
        default:
            return std::nullopt;
    }

}

} // namespace Smriti