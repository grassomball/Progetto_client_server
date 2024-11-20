/*
 ============================================================================
 Name        : client.c
 Author      : Gabriele Grasso
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */



// server.c

#if defined WIN32
#include <winsock2.h>  // Include Winsock for Windows platform
#else
#define closesocket close  // Redefine closesocket to close for non-Windows platforms
#include <sys/socket.h>    // Socket communication functions in C
#include <arpa/inet.h>     // Network functions and IP address manipulation
#include <unistd.h>        // OS and process interaction functions
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "protocol.h"

#define SERVER_IP "127.0.0.1"   // Server's IP address
#define PROTOPORT 61000         // Server's port number
#define BUFFER_SIZE 128         // Buffer size for sending and receiving data

// Error handler function to print error messages
void errorhandler(char *errormessage) {
    printf("%s", errormessage);
}

// Cleanup function for Winsock (Windows-specific)
void clearsock() {
#if defined WIN32
    WSACleanup();  // Cleanup Winsock resources on Windows
#endif
}

int main() {
    srand(time(NULL));  // Seed the random number generator for password generation

#if defined WIN32
    WSADATA wsa_data;
    int result = WSAStartup(MAKEWORD(2, 2), &wsa_data);  // Initialize Winsock 2.2
    if (result != NO_ERROR) {
        printf("Error at WSAStartup()\n");
        return 0;
    }
#endif

    // Create the client socket
    int client_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (client_socket < 0) {
        errorhandler("Socket creation failed.\n");
        clearsock();  // Cleanup
        return -1;
    }

    // Set up server address and port
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));  // Initialize the server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);  // Convert string IP to binary format
    server_addr.sin_port = htons(PROTOPORT);  // Convert port number to network byte order

    // Connect to the server
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        errorhandler("Connection failed.\n");
        closesocket(client_socket);
        clearsock();  // Cleanup
        return -1;
    }

    printf("Connected to server %s:%d\n", SERVER_IP, PROTOPORT);

    char buffer[BUFFER_SIZE];
    /*char TYPE;
    int length;*/

    while (1) {
        // Request user input for password type and length
        printf("Enter password type and length (e.g., 'n 8' for numeric password of 8 chars) or 'q' to quit:\n");
        fgets(buffer, sizeof(buffer), stdin);

        // Check if user wants to quit
        if (buffer[0] == 'q') {
            printf("Closing connection.\n");
            break;
        }

        // Send the request to the server
        send(client_socket, buffer, strlen(buffer), 0);

        // Receive the generated password from the server
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';  // Null-terminate the received string
            printf("Generated password: %s\n", buffer);
        } else {
            printf("Error receiving data.\n");
        }
    }

    // Close the client socket and cleanup
    closesocket(client_socket);
    clearsock();
    return 0;
}
