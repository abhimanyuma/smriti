/**
 * This has a lot of standard convertors that we use
 */
#include "convertors.h"

#include <cstddef>
#include <cstdint>
#include <optional>

namespace Smriti {

std::optional<int64_t> parse_int(char*& current, std::size_t length) {
    /**
     * @brief Converts a numeric string to an integer.
     *
     * Parses the input C-string and converts its leading numeric characters
     * into an integer value. Parsing stops at the first non-digit character.
     *
     * @param current Pointer to a null-terminated string containing the number to convert.
     * @param length The length of the string to consider for conversion.
     * @return int The integer value represented by the numeric characters at the start of the string.
     *         Returns 0 if the string does not start with a digit.
     */
    int64_t result = 0;
    int sign = 1;
    bool has_value = false;

    if (*current == '-' || *current == '+') {

        sign = (*current == '-') ? -1 : 1;
        current++;
        length--;
    }
    while ((*current >= '0' && *current <= '9') && length > 0) {
        result = result * 10 + (*current - '0');
        current++;
        length--;
        has_value = true;
    }

    if (!has_value) {
        return std::nullopt; // No valid digits found
    }
    return result * sign;
}

}   // namespace Smriti