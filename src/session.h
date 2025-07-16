#ifndef INCLUDED_SMRITI_SESSION_H
#define INCLUDED_SMRITI_SESSION_H

#include <boost/asio.hpp>
#include "processor.h"

using boost::asio::ip::tcp;

namespace Smriti {

class Session : public std::enable_shared_from_this<Session> {
  public:
    explicit Session(tcp::socket socket, Processor& processor);
    void start();

  private:
    void read();
    void write(std::string response);

    tcp::socket d_socket;
    enum { max_length = 1024 };
    char d_data[max_length];
    Processor& d_processor;
};

} // namespace Smriti

#endif // INCLUDED_SMRITI_SESSION_H