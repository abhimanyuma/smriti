#include <boost/asio.hpp>
#include <iostream>

#include "server.h"

int main() {
    try {
        boost::asio::io_context io_context;
        unsigned short redis_port = 6379;

        std::cout << "Creating new object...\n" << std::flush;
        Smriti::Server server{io_context, redis_port};

        std::cout << "Calling accept()...\n" << std::flush;
        server.accept();

        std::cout << "Async server listening on port " << redis_port << "...\n";
        io_context.run();
    } catch (std::exception &e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}