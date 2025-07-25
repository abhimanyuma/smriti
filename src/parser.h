#ifndef INCLUDED_SMRITI_PARSER_H
#define INCLUDED_SMRITI_PARSER_H

#include "resp_value.h"

#include <optional>
#include <string>

namespace Smriti {



class Parser {
public:
    std::optional<RespValue>  parse();
    // Constructors and Destructors
    explicit Parser(const std::string& data): d_data{data}, d_current_index{0}{};
    Parser(const Parser& other): d_data{other.d_data}, d_current_index{other.d_current_index}{};
    ~Parser(){};
private:
    size_t d_current_index;
    const std::string& d_data;

    std::optional<std::string_view> read_line();
    std::optional<RespValue> parse_simple_string();
    std::optional<RespValue> parse_error();
    std::optional<RespValue> parse_integer();
    std::optional<RespValue> parse_bulk_string();
    std::optional<RespValue> parse_array();

    std::optional<int64_t> parse_bare_int();

};

} // namespace Smriti

#endif  // INCLUDED_SMRITI_PARSER_H