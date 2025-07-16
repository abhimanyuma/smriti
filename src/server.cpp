#include "server.h"

#include <boost/asio.hpp>
#include <iostream>
#include <memory>

#include "session.h"

namespace Smriti {
using boost::asio::ip::tcp;

// Constructor for the Server class
// Initializes the acceptor to listen on the specified port
Server::Server(boost::asio::io_context &io_context, unsigned short port)
    : d_acceptor{io_context, tcp::endpoint(tcp::v4(), port)}, d_port{port}, d_processor{} {}

void Server::accept() {
    d_acceptor.async_accept([this](boost::system::error_code ec, tcp::socket socket) {
        if (!ec) {
            std::cout << "New connection accepted on port " << d_port << "\n";
            std::make_shared<Session>(std::move(socket), d_processor)->start();
        } else {
            std::cerr << "Accept error: " << ec.message() << "\n";
        }
        // Continue accepting new connections
        accept();
    }

    );
}
} // namespace Smriti