#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>


int handle_client(int client_fd) {

    // For now we respond to all requests with a simple PONG response
    // We cannot currently handle the full redis-cli

    send(client_fd, "+PONG\r\n", 7, 0); // Send a simple PONG response
    std::cout << "Handled client request, sent PONG response\n";
    return 1; // Successfully handled request
}