#define _GNU_SOURCE

#include "as.h"

bool verbose = false;

char *ASIP = "193.136.138.142";
char *ASport = "58045";


int main(int argc, char **argv) {
    int opt;
    char buffer[MAX_RESPONSE_SIZE];

    while ((opt = getopt(argc, argv, "v")) != -1) {
        switch (opt) {
            case 'v':
                verbose = true;
                break;
            case '?':
                fprintf(stderr, "Usage: %s [-v]\n", argv[0]);
                exit(EXIT_SUCCESS);
        }
    }

    // Create a UDP socket
    int udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_socket == -1) {
        perror("Error creating UDP socket");
        exit(EXIT_FAILURE);
    }

    // Create a TCP socket
    int tcp_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (tcp_socket == -1) {
        perror("Error creating TCP socket");
        close(udp_socket);
        exit(EXIT_FAILURE);
    }

    // Set up server address structures for both UDP and TCP
    struct sockaddr_in udp_server_address, tcp_server_address;
    memset(&udp_server_address, 0, sizeof(udp_server_address));
    memset(&tcp_server_address, 0, sizeof(tcp_server_address));

    udp_server_address.sin_family = AF_INET;
    udp_server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    udp_server_address.sin_port = htons(12345); // Change the port as needed

    tcp_server_address.sin_family = AF_INET;
    tcp_server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    tcp_server_address.sin_port = htons(12346); // Change the port as needed

    // Bind UDP socket to the specified address and port
    if (bind(udp_socket, (struct sockaddr*)&udp_server_address, sizeof(udp_server_address)) == -1) {
        perror("Error binding UDP socket");
        close(udp_socket);
        close(tcp_socket);
        exit(EXIT_FAILURE);
    }

    // Bind TCP socket to the specified address and port
    if (bind(tcp_socket, (struct sockaddr*)&tcp_server_address, sizeof(tcp_server_address)) == -1) {
        perror("Error binding TCP socket");
        close(udp_socket);
        close(tcp_socket);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming TCP connections
    if (listen(tcp_socket, 5) == -1) {
        perror("Error listening on TCP socket");
        close(udp_socket);
        close(tcp_socket);
        exit(EXIT_FAILURE);
    }

    printf("Server is running...\n");


    while (1) {
        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_SET(udp_socket, &read_fds);
        FD_SET(tcp_socket, &read_fds);

        // Use select to wait for data on either UDP or TCP socket
        if (select(FD_SETSIZE, &read_fds, NULL, NULL, NULL) == -1) {
            perror("Error in select");
            close(udp_socket);
            close(tcp_socket);
            exit(EXIT_FAILURE);
        }

        if (FD_ISSET(udp_socket, &read_fds)) {
            // Receive data from the UDP client
            struct sockaddr_in client_address;
            socklen_t client_address_len = sizeof(client_address);
            ssize_t bytes_received = recvfrom(udp_socket, buffer, sizeof(buffer), 0,
                                             (struct sockaddr*)&client_address, &client_address_len);

            if (bytes_received == -1) {
                perror("Error receiving UDP data");
                close(udp_socket);
                close(tcp_socket);
                exit(EXIT_FAILURE);
            }

            buffer[bytes_received] = '\0';

            if (verbose) {
                printf("Received UDP message from %s:%d\n", inet_ntoa(client_address.sin_addr),
                       ntohs(client_address.sin_port));
            }

            // Process the received UDP request
            execute_request(buffer);
        }

        if (FD_ISSET(tcp_socket, &read_fds)) {
            // Accept incoming TCP connection
            int client_socket = accept(tcp_socket, NULL, NULL);
            if (client_socket == -1) {
                perror("Error accepting TCP connection");
                close(udp_socket);
                close(tcp_socket);
                exit(EXIT_FAILURE);
            }

            if (verbose) {
                printf("Accepted TCP connection\n");
            }

            // Receive data from the TCP client
            ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
            if (bytes_received == -1) {
                perror("Error receiving TCP data");
                close(udp_socket);
                close(tcp_socket);
                close(client_socket);
                exit(EXIT_FAILURE);
            }

            buffer[bytes_received] = '\0';

            if (verbose) {
                printf("Received TCP message: %s\n", buffer);
            }

            // Process the received TCP request
            execute_request(buffer);

            // Close the client socket
            close(client_socket);
        }
    }

    close(udp_socket);
    close(tcp_socket);

    return 0;
}