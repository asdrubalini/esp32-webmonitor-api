#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

#include "request.h"

#define LISTEN_PORT 8080
#define BUFFER_SIZE 128 // bytes


int main() {
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    // Check for errors
    if (sock_fd < 0) {
        std::cerr << "Error while creating socket" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Listen on all interfaces
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(LISTEN_PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sock_fd, (struct sockaddr *) &server_addr, sizeof (server_addr)) < 0) {
        std::cerr << "bind() error" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (listen(sock_fd, 100) < 0) {
        std::cerr << "listen() error" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Allocate buffer memory
    char *buffer = (char *) calloc(BUFFER_SIZE, sizeof (char));

    // Client address
    struct sockaddr_in client_addr;
    auto client_addr_length = sizeof (client_addr);

    // Client socket fd
    int client_sock;

    // Server loop
    while (true) {
        client_sock = accept(sock_fd, (struct sockaddr *) &client_addr, (socklen_t*) &client_addr_length);

        if (client_sock < 0) {
            std::cout << "accept() error" << std::endl;

            close(client_sock);

            continue;
        }

        int read_length = read(client_sock, buffer, BUFFER_SIZE);

        if (read_length < 0) {
            std::cout << "Error reading from socket" << std::endl;

            memset(buffer, 0, BUFFER_SIZE);
            close(client_sock);

            continue;
        }

        // Remove new-lines and stuff
        buffer[strcspn(buffer, "\r\n")] = 0;

        std::cout << "Requesting: " << buffer << std::endl;
        
        auto status = request_get_status(buffer);
        
        // Response format
        const char *res_format = "%d,%d,%d";

        // Obtain expected response length for memory allocation
        int res_length = snprintf(
            NULL, 0, res_format,
            status.error, status.status_code, status.duration_ms
        ) + 1;

        // Allocate response memory
        char *res_data = (char *) calloc(res_length, sizeof (char));

        if (!res_data) {
            std::cout << "Error while allocating output data" << std::endl;

            memset(buffer, 0, BUFFER_SIZE);
            close(client_sock);
            free(res_data);

            continue;
        }

        // Build response
        snprintf(
            res_data, res_length, res_format,
            status.error, status.status_code, status.duration_ms
        );

        // Write back to client
        write(client_sock, res_data, res_length);

        // Cleanup
        memset(buffer, 0, BUFFER_SIZE);
        close(client_sock);
        free(res_data);
    }

    // Cleanup
    free(buffer);
    close(sock_fd);

    return 0;
}
