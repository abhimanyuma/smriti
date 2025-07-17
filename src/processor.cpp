
#include "processor.h"

#include <string>
#include <vector>
#include <ctype.h>
#include <algorithm>

namespace Smriti {

RespValue Processor::handle_ping(const std::vector<RespValue>& commands) {
    // PING command should return a simple string "PONG"
    return RespValue::simple_string(std::string{"PONG"});
}

RespValue Processor::handle_echo(const std::vector<RespValue>& commands) {
    if (commands.size() < 2) {
        return RespValue::error(std::string{"ECHO command requires an argument"});
    }
    return RespValue::bulk_string(commands[1].as_string());
}

RespValue Processor::handle_set(const std::vector<RespValue>& commands) {
    if (commands.size() != 3) {
        return RespValue::error(std::string{"SET command requires a key and a value"});
    }
    const std::string& key = commands[1].as_string();
    const std::string& value = commands[2].as_string();
    d_key_value_store[key] = value;
    return RespValue::simple_string("OK");
}

RespValue Processor::handle_get(const std::vector<RespValue>& commands) {
    if (commands.size() != 2) {
        return RespValue::error(std::string{"GET command requires a key"});
    }
    const std::string& key = commands[1].as_string();
    auto it = d_key_value_store.find(key);
    if (it != d_key_value_store.end()) {
        return RespValue::bulk_string(it->second);
    }
    return RespValue::null();
}

RespValue Processor::process(const RespValue& input_value) {
    if (input_value.is_null()) {
        return RespValue::null();
    }

    if (input_value.type() == RespTypeEnum::Array) {
        std::vector<RespValue> commands = input_value.as_array();
        if (commands.empty()) {
            return RespValue::error("Empty array");
        }
        auto command = commands[0].as_string();
        std::transform(command.begin(), command.end(), command.begin(), ::toupper);
        if (command == "PING") {
            return handle_ping(commands);
        } else if (command == "ECHO") {
            return handle_echo(commands);
        } else if (command == "SET") {
            return handle_set(commands);
        } else if (command == "GET") {
            return handle_get(commands);
        }
    }
    // We have not yet implemented other commands
    // For now, we return an error for unimplemented commands
    return RespValue::error("Processing not implemented yet");
}


} // namespace Smriti