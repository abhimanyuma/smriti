#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

int get_input(char* input_buffer, int client_fd) {
    ssize_t bytes_received = recv(client_fd, input_buffer, sizeof(input_buffer) - 1, 0);
    
    if (bytes_received < 0) {
        std::cerr << "Failed to receive data from client\n";
        return -1;
    } else if (bytes_received == 0) {
        std::cout << "Client disconnected\n";
        return 0; // Client disconnected
    }

    input_buffer[bytes_received] = '\0';
}

int handle_output(char* output_buffer, const char* input_buffer, int client_fd) {
    // Process the input and prepare the output
    if (std::strcmp(input_buffer, "PING") == 0) {
        std::strcpy(output_buffer, "+PONG\r\n");
    } else {
        std::strcpy(output_buffer, "-ERR unknown command\r\n");
    }
    
    return 1; // Successfully sent response
}

int handle_client(int client_fd) {
    char input_buffer[1024];
    
    get_input(input_buffer, client_fd);
    
    char output_buffer[1024];
    handle_output(output_buffer, input_buffer, client_fd);
    
    ssize_t bytes_sent = send(client_fd, output_buffer, std::strlen(output_buffer), 0);
    if (bytes_sent < 0) {
        std::cerr << "Failed to send data to client\n";
        return -1; // Error in sending response
    } else if (bytes_sent == 0) {
        std::cout << "Client disconnected while sending response\n";
        return 0; // Client disconnected
    }
    std::cout << "Response sent to client: " << output_buffer;
    return 1; // Successfully handled request
}