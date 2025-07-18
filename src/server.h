#ifndef INCLUDED_SMRITI_SERVER_H
#define INCLUDED_SMRITI_SERVER_H

#include <boost/asio.hpp>
#include <iostream>
#include <memory>

#include "processor.h"

using boost::asio::ip::tcp;

namespace Smriti {

class Server {
  public:
    Server(boost::asio::io_context &io_context, unsigned short port);
    void accept();

  private:

    Processor d_processor;
    tcp::acceptor d_acceptor;
    unsigned short d_port;
};
} // namespace Smriti

#endif // INCLUDED_SMRITI_SERVER_H