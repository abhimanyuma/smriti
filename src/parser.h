#ifndef INCLUDED_SMRITI_PARSER_H
#define INCLUDED_SMRITI_PARSER_H

#include "resp_value.h"

#include <optional>

namespace Smriti {

std::optional<RespValue>  parse(char*& data, size_t length);

} // namespace Smriti

#endif  // INCLUDED_SMRITI_PARSER_H