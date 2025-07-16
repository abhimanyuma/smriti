#include "session.h"

#include <boost/asio.hpp>
#include <iostream>

#include "parser.h"

namespace Smriti {

Session::Session(tcp::socket socket, Processor& processor)
    : d_socket(std::move(socket)), d_processor(processor) {}

void Session::start() {
    // Start the read operation
    read();
}

void Session::read() {
    auto self(shared_from_this());
    d_socket.async_read_some(
        boost::asio::buffer(d_data, max_length),
        [this, self](boost::system::error_code ec, std::size_t length) {
            if (!ec) {
                const std::string data{d_data, length};
                std::cout << "Received data: " << data << "\n";
                auto parser = Parser{data};
                auto input_value = parser.parse();
                auto output_value = d_processor.process(input_value.value_or(RespValue::null()));
                write(std::move(output_value.to_resp_string()));
            } else {
                std::cerr << "Read error: " << ec.message() << "\n";
            }
        }
    );
}

void Session::write(std::string response) {
    auto self(shared_from_this());
    d_socket.async_write_some(
        boost::asio::buffer(response),
        [this, self](boost::system::error_code ec, std::size_t /*length*/) {
            if (!ec) {
                // Continue reading for more data
                read();
            } else {
                std::cerr << "Write error: " << ec.message() << "\n";
            }
        }
    );
}

} // namespace Smriti