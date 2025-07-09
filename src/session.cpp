#include "session.h"

#include <boost/asio.hpp>
#include <iostream>

namespace Smriti {

Session::Session(tcp::socket socket) : d_socket(std::move(socket)) {}

void Session::start() {
    // Start the read operation
    read();
}

void Session::read() {
    auto self(shared_from_this());
    d_socket.async_read_some(boost::asio::buffer(d_data, max_length),
                             [this, self](boost::system::error_code ec, std::size_t length) {
                                 if (!ec) {
                                     // Process the data received
                                     std::cout << "Received data: " << std::string(d_data, length) << "\n";
                                     // Continue reading
                                     write();
                                 } else {
                                     std::cerr << "Read error: " << ec.message() << "\n";
                                 }
                             });
}

void Session::write() {
    auto self(shared_from_this());
    d_socket.async_write_some(boost::asio::buffer("+PONG\r\n", 7),
                              [this, self](boost::system::error_code ec, std::size_t /*length*/) {
                                  if (!ec) {
                                      // Continue reading for more data
                                      read();
                                  } else {
                                      std::cerr << "Write error: " << ec.message() << "\n";
                                  }
                              });
}

} // namespace Smriti