/**
 * This has a lot of standard convertors that we use
 */

int parse_int(const char* input) {
    /**
     * @brief Converts a numeric string to an integer.
     *
     * Parses the input C-string and converts its leading numeric characters
     * into an integer value. Parsing stops at the first non-digit character.
     *
     * @param input Pointer to a null-terminated string containing the number to convert.
     * @return int The integer value represented by the numeric characters at the start of the string.
     *         Returns 0 if the string does not start with a digit.
     */
    int result = 0;
    if (*input == '-' || *input == '+') {
        // Handle sign, but we only return the absolute value
        // for now.
        input++;
    }
    while (*input >= '0' && *input <= '9') {
        result = result * 10 + (*input - '0');
        input++;
    }
    return result;
}
