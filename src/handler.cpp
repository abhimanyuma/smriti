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

    while (true) {
        char input_buffer[1024];
        ssize_t bytes_received = recv(client_fd, input_buffer, sizeof(input_buffer) - 1, 0);
        if (bytes_received < 0) {
            std::cerr << "Failed to receive data from client\n";
            return -1; // Indicate failure
        }
        if (bytes_received == 0) {
            std::cout << "Client " << client_fd << " disconnected\n";
            return 0; // Client disconnected
        }

        // Here you can process the request and send a response back to the client
        const char *response = "+PONG\r\n"; // Example response for a PING command
        send(client_fd, response, strlen(response), 0);
    }
}