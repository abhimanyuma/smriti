#ifndef INCLUDED_SMRITI_PROCESSOR_H
#define INCLUDED_SMRITI_PROCESSOR_H

#include "resp_value.h"
#include <optional>

namespace Smriti {

class Processor {
    public:
    std::optional<RespValue> process(RespValue input_value);

};

}   // namespace Smriti


#endif  // INCLUDED_SMRITI_PROCESSOR_H