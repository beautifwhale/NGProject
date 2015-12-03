// select_client.c
//
// Year 4 Networked Games Assignment 2015
// Team:	David Morton
//			Kevin Byrne
//			Derek O Brien
// 			add names here...
//
// Description: The Select client will take the username and then start a connection with the server
// using the once the connection is made the client uses MultiplexIO() function which implements 
// select to check if the input is from the stdin or the server.
//
// Dependencies: libhangman, libsocket
//
#include "../includes/Definitions.h"
#include "../../../libsocket/socket.h"
#include <string.h> // strlen()

int main(int argc, char * argv[]) {
	int iSocketFileDescriptor;
	char * strServerIPAddress;
	struct Address sAddress;
	char*  username;
	username = argv[2];

	char buffer[MAX_BUF_SIZE];

	strServerIPAddress = argv[1];

	//Set up socket and connection
	iSocketFileDescriptor = Socket(AF_INET, SOCK_STREAM, 0);
	
	Address(AF_INET, (struct Address*) &sAddress, strServerIPAddress, HANGMAN_TCP_PORT);

	Connect(iSocketFileDescriptor, (struct sockaddr*) &sAddress.m_sAddress, sizeof(sAddress.m_sAddress));

	// Get the username from stdin
	// Send username to the server
	sprintf(buffer, "%s ", username);
	Write(iSocketFileDescriptor, &buffer, strlen(buffer) +1);
	printf("Username %s sent to the server\n", buffer);

	MultiplexIO(stdin, iSocketFileDescriptor);

	printf("Game over");
	return 0;
}
