#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MAX_BUFFER_SIZE 1024
#define PORT 8080

int main() {
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[MAX_BUFFER_SIZE] = {0};

    // Create socket file descriptor
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    // Get the number from the user
    printf("Enter a number: ");
    int number;
    scanf("%d", &number);

    // Convert the number to a string
    snprintf(buffer, sizeof(buffer), "%d", number);

    // Send the number to the server
    send(sock, buffer, strlen(buffer), 0);

    // Receive the result from the server
    memset(buffer, 0, sizeof(buffer));
    valread = read(sock, buffer, MAX_BUFFER_SIZE);

    printf("Result from the server: %s\n", buffer);

    return 0;
}
