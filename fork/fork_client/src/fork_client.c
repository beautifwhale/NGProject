// fork_client.c
// 
// Year 4 Networked Games Assignment 2015
//
// Team:	David Morton
//			Kevin Byrne
//			Derek O Brien
// 			add names here...
//
//
// Description: The fork client uses the libsocket library to communicate with the 
// server running the Hangman game. The client will initiate a connection with the server using
// the wrapper functions provided by the libsocket library. Once a connection is set up inputs 
// from the user and the network socket are multiplexed using the select() function inside the 
// call to PlayHangmanClientTCP(). Implementations for each function call in the client
// can be found in the libsocket socket.c file.
//
#include "../../../libhangman/hangman.h"

int main(int argc, char * argv[]) {
	int iSocketFileDescriptor;
	char *strServerIPAddress;
	char *strServiceName;

	// Check for command line arguments
	if (argc != 3)
   	{
		printf("usage: clientForkTCP <hostname> <service name/port Number>\n");
		exit(1);
   	}

	// Get hostname and service name of game on the server from stdin
	strServerIPAddress = argv[1];
	strServiceName = argv[2];

	// Old version using gethostbyname()
	//iSocketFileDescriptor = Socket(AF_INET, SOCK_STREAM, 0);
	//Address(AF_INET, (struct Address*) &sAddress, strServerIPAddress, HANGMAN_TCP_PORT);
	//Connect(iSocketFileDescriptor, (struct sockaddr*) &sAddress.m_sAddress, sizeof(sAddress.m_sAddress));


	// Create connection to the server using getaddrinfo()
	// Pass in the hostname, service port number, application type, and protocol
	iSocketFileDescriptor = InitConnection(strServerIPAddress, strServiceName, TYPE_CLIENT, SOCK_STREAM);


	// Wrapper function to multiplex user input and network input on the 
	// socket file descriptor. MultiplexIO() implementation
	// can be found in the libsocket socket.c file.
	PlayHangmanClientTCP(stdin, iSocketFileDescriptor);


	// On return exit application
	//shutdown(iSocketFileDescriptor, SHUT_RDWR);
	//close(iSocketFileDescriptor);
	printf("Game over\n");
	return 0;
}
