
#include "processor.h"
#include "utils_string.h"

#include <chrono>
#include <iostream>
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
    if (commands.size() < 3) {
        return RespValue::error(std::string{"SET command requires a key and a value"});
    }

    const std::string& key = commands[1].as_string();
    const std::string& value = commands[2].as_string();

    int64_t expiry_time = -1;

    if (commands.size() == 5) {


        std::string expiry_command = commands[3].as_string();
        to_uppercase(expiry_command); // Convert to uppercase for case-insensitive matchingv
        auto now = std::chrono::high_resolution_clock::now();
        int64_t timestamp_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()
        ).count();

        int64_t expiry_value = std::stoi(commands[4].as_string());

        if (expiry_command == "PX") {
            expiry_time = timestamp_ms + expiry_value;
        } else if (expiry_command == "EX") {
            expiry_time = timestamp_ms + expiry_value * 1000; // Store the expiry time
        } else {
            return RespValue::error(std::string{"SET command requires PX or EX for expiry"});
        }
    } else if (commands.size() != 3) {
        return RespValue::error(std::string{"SET command requires either 3 or 5 arguments"});
    }

    d_key_value_store[key] = value;
    if (expiry_time != -1) {
        d_expiry_store[key] = expiry_time; // Store the expiry time
    } else {
        d_expiry_store.erase(key); // Remove from expiry store if no expiry is set
    }

    return RespValue::simple_string("OK");
}

RespValue Processor::handle_get(const std::vector<RespValue>& commands) {
    if (commands.size() != 2) {
        return RespValue::error(std::string{"GET command requires a key"});
    }
    const std::string& key = commands[1].as_string();

    // Check if the key has expired
    auto expiry_it = d_expiry_store.find(key);
    if (expiry_it != d_expiry_store.end()) {
        auto now = std::chrono::high_resolution_clock::now();
        int64_t current_time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()
        ).count();

        if (current_time_ms > expiry_it->second) {
            // Key has expired
            d_key_value_store.erase(key);
            d_expiry_store.erase(key);
        }
    }

    auto it = d_key_value_store.find(key);
    if (it == d_key_value_store.end()) {
        return RespValue::null();
    }

    return RespValue::bulk_string(it->second);

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
        to_uppercase(command); // Convert command to uppercase for case-insensitive matching
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