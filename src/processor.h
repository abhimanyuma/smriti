#ifndef INCLUDED_SMRITI_PROCESSOR_H
#define INCLUDED_SMRITI_PROCESSOR_H

#include "resp_value.h"
#include <optional>

namespace Smriti {

class Processor {
    private:
    RespValue handle_ping(const std::vector<RespValue>& commands);
    RespValue handle_echo(const std::vector<RespValue>& commands);
    public:
    RespValue process(const RespValue& input_value);

};

}   // namespace Smriti


#endif  // INCLUDED_SMRITI_PROCESSOR_H