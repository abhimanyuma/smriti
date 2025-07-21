#ifndef INCLUDED_SMRITI_PROCESSOR_H
#define INCLUDED_SMRITI_PROCESSOR_H

#include "resp_value.h"
#include <optional>
#include <map>

namespace Smriti {

class Processor {

private:
    RespValue handle_ping(const std::vector<RespValue>& commands);
    RespValue handle_echo(const std::vector<RespValue>& commands);
    RespValue handle_set(const std::vector<RespValue>& commands);
    RespValue handle_get(const std::vector<RespValue>& commands);

    std::map<std::string, std::string> d_key_value_store;
    std::map<std::string, int64_t> d_expiry_store; // Expiry store

public:
    RespValue process(const RespValue& input_value);

};

}   // namespace Smriti


#endif  // INCLUDED_SMRITI_PROCESSOR_H