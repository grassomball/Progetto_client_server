/*
 ============================================================================
 Name        : server.c
 Author      : Gabriele Grasso
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
#if defined WIN32
#include<winsock2.h>  // Include Winsock library for Windows
#else
#define closesocket close  // Define `closesocket` as `close` for non-Windows platforms
#include<sys/socket.h>    // Provides functions for socket communication
#include<arpa/inet.h>     // Functions for IP address manipulation
#include<unistd.h>        // Provides functions for interacting with the OS and processes
#endif
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<time.h>
#include "protocol.h"

#define PROTOPORT 61000 // Default port number for the server
#define QLEN 5          // Maximum size of the request queue for the server

// Error handler function to display error messages
void errorhandler(char *errormessage) {
    printf("%s", errormessage);  // Print the error message to standard output
}

// Function to clean up network resources (specifically for Windows)
void clearsock() {
    #if defined WIN32
    WSACleanup();  // Clean up the Winsock library (Windows-specific)
    #endif
}

// Function declarations for password generation (defined later)
void generate_secure(char *password, int length);
void generate_alpha(char* password, int length);
void generate_mixed(char* password, int length);
void generate_secure(char* password, int length);  // Duplicate declaration (remove one)

// Main server program
int main() {
    srand(time(NULL));  // Seed the random number generator with the current time

    #if defined WIN32
    // Initialize Winsock (Windows-specific)
    WSADATA wsa_data;
    int result = WSAStartup(MAKEWORD(2, 2), &wsa_data);  // Initialize Winsock version 2.2
    if (result != NO_ERROR) {
        printf("Error at WSAStartup()\n");
        return 0;  // Exit if initialization fails
    }
    #endif

    // Create the server socket
    int server_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);  // TCP socket
    if (server_socket < 0) {
        errorhandler("Socket creation failed.\n");  // Handle error if socket creation fails
        clearsock();  // Cleanup resources
        return -1;  // Exit the program
    }

    // Set up the server address structure (IP and port)
    struct sockaddr_in sad;
    memset(&sad, 0, sizeof(sad));  // Initialize the sockaddr_in structure to 0
    sad.sin_family = AF_INET;  // Use IPv4 addressing
    sad.sin_addr.s_addr = inet_addr(SERVER_IP);  // Set server IP address (to be defined elsewhere)
    sad.sin_port = htons(PROTOPORT);  // Set the server port (converted to network byte order)

    // Bind the socket to the specified address and port
    if (bind(server_socket, (struct sockaddr*)&sad, sizeof(sad)) < 0) {
        errorhandler("Bind failed.\n");  // Handle error if binding fails
        closesocket(server_socket);  // Close the socket
        clearsock();  // Cleanup resources
        return -1;  // Exit the program
    }

    // Set the socket to listen for incoming connections with a maximum queue size of QLEN
    if (listen(server_socket, QLEN) < 0) {
        errorhandler("Listen failed.\n");  // Handle error if listen fails
        closesocket(server_socket);  // Close the socket
        clearsock();  // Cleanup resources
        return -1;  // Exit the program
    }

    // Server enters an infinite loop, waiting for incoming client connections
    struct sockaddr_in cad;  // Client address structure
    int client_socket;       // Client socket descriptor
    int client_len;          // Size of the client address
    printf("Waiting for a client to connect...\n\n");

    while (1) {
        client_len = sizeof(cad);  // Set the size of the client address structure
        // Accept an incoming client connection
        if ((client_socket = accept(server_socket, (struct sockaddr*)&cad, &client_len)) < 0) {
            errorhandler("Accept failed.\n");  // Handle error if accept fails
            closesocket(client_socket);  // Close the client socket
            clearsock();  // Cleanup resources
            return 0;  // Exit the program
        }

        // Print the client's IP address and port number
        printf("New connection from %s:%d\n", inet_ntoa(cad.sin_addr), ntohs(cad.sin_port));

        // Define a buffer to receive data from the client
        char buffer[BUFFER_SIZE];
        while (recv(client_socket, buffer, BUFFER_SIZE, 0) > 0) {  // Receive data from the client
            char type = buffer[0];  // The first character indicates the password type
            int length = atoi(buffer + 2);  // The length of the password (starting from index 2)

            // Check if the length is valid (between 6 and 32 characters)
            if (length < 6 || length > 32) {
                snprintf(buffer, BUFFER_SIZE, "Error: Length must be between 6 and 32.\n");  // Error message for invalid length
            } else {
                // Generate the password based on the specified type
                switch (type) {
                    case 'n': generate_numeric(buffer, length); break;  // Numeric password
                    case 'a': generate_alpha(buffer, length); break;    // Alphabetical password
                    case 'm': generate_mixed(buffer, length); break;    // Mixed password (letters and numbers)
                    case 's': generate_secure(buffer, length); break;   // Secure password (letters, numbers, and symbols)
                    default: snprintf(buffer, BUFFER_SIZE, "Error: Invalid type.\n"); break;  // Invalid password type
                }
            }

            // Send the generated password (or error message) back to the client
            send(client_socket, buffer, strlen(buffer), 0);
        }

        printf("Client disconnected.\n");  // Print when the client disconnects
        closesocket(client_socket);  // Close the client socket
    }

    closesocket(server_socket);  // Close the server socket when done
    return 0;  // Exit the program
}

