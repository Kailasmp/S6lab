#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <arpa/inet.h>

#define MAX_BUFFER_SIZE 1024
#define PORT 8080

bool is_prime(int number) {
    if (number <= 1)
        return false;

    for (int i = 2; i * i <= number; i++) {
        if (number % i == 0)
            return false;
    }

    return true;
}

int main() {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[MAX_BUFFER_SIZE] = {0};

    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Attach socket to the port
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket to the specified address and port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Start listening for connections
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    // Accept incoming connections
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                             (socklen_t *)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    // Receive number from client
    valread = read(new_socket, buffer, MAX_BUFFER_SIZE);
    int number = atoi(buffer);

    // Check if the number is prime
    bool isPrime = is_prime(number);

    // Prepare the result message
    memset(buffer, 0, sizeof(buffer));
    if (isPrime)
        strcpy(buffer, "Prime");
    else
        strcpy(buffer, "Not prime");

    // Send the result to the client
    send(new_socket, buffer, strlen(buffer), 0);
    printf("Result sent to the client: %s\n", buffer);

    return 0;
}
