#ifndef INCLUDED_SMRITI_SESSION_H
#define INCLUDED_SMRITI_SESSION_H

#include <boost/asio.hpp>

using boost::asio::ip::tcp;

namespace Smriti {

class Session : public std::enable_shared_from_this<Session> {
  public:
    explicit Session(tcp::socket socket);
    void start();

  private:
    void read();
    void write();

    tcp::socket d_socket;
    enum { max_length = 1024 };
    char d_data[max_length];
};

} // namespace Smriti

#endif // INCLUDED_SMRITI_SESSION_H