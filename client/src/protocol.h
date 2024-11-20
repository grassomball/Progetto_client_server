/*
 * protocol.h
 *
 *  Created on: 18 nov 2024
 *      Author: Gabriele Grasso
 */


#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#define MAX_PASSWORD_LENGTH 32     // max password lenght
#define MIN_PASSWORD_LENGTH 6     // min password lenght

#define BUFFER_SIZE 128           // Define buffer size for communication between client and server
#define PROTOPORT 61000          // Define port number that the server will listen on
#define SERVER_IP "127.0.0.1"   // Define the server's IP address (localhost in this case)
#define QLEN 5                 // Define the maximum size of the queue for incoming client connections

//types of commands sent by the client
#define TYPE_ALPHA "a"
#define TYPE_MIXED "m"
#define TYPE_SECURE "s"


//password generation functions
void generate_numeric(char* password, int length){
 for (int i = 0; i<length; i++){
	 password[i] = '0'+ rand() % 10;
 }
 password[length] = '\0';
}
void generate_alpha(char* password, int length){
	for (int i = 0; i<length; i++){
		password[i] = 'a' + rand() % 26;
	}
	password[length] = '\0';
}
void generate_mixed(char* password, int length){
	for (int i = 0; i<length; i++){
		if(rand() % 2 == 0){
			password[i] = 'a' + rand() % 26;
		}
		else{
			password[i] = '0' + rand() % 10;
		}
		password[length] = '\0';
	}
}
void generate_secure(char* password, int length){
	 const char chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*()";
	 for (int i = 0; i<length; i++){
		 password[i] = chars[rand() % (sizeof(chars)-1)];
	 }
	password[length] = '\0';
}

#endif /* PROTOCOL_H_ */


