#ifndef INCLUDED_SMRITI_PARSER_H
#define INCLUDED_SMRITI_PARSER_H

#include "resp_value.h"

namespace Smriti {

RespValue parse(const char* data, size_t length);

} // namespace Smriti

#endif  // INCLUDED_SMRITI_PARSER_H