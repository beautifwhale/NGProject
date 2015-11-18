// fork_client.c
// 
// Year 4 Networked Games Assignment 2015
// Team:	David Morton
//		Kevin Byrne
// 		add names here...
//
// 
//
// Description: The fork client uses the libsocket library to communicate with the 
// server running the Hangman game. The client will initiate a connection with the server using
// the wrapper functions provided by the libsocket library. Once a connection is set up inputs 
// from the user and the network socket are multiplexed using the select() function inside the 
// call to MultiplexStdinFileDescriptor(). Implementations for each function call in the client 
// can be found in the libsocket socket.c file.
#include "../../../libsocket/socket.h"
#include "../includes/definitions.h"

int main(int argc, char * argv[]) {
	int iSocketFileDescriptor;
	char * strServerIPAddress;
	struct Address sAddress;

	if (argc != 2)
   	{
      		printf("usage:  udpclient <IP address>\n");
     		exit(1);
   	}
	strServerIPAddress = argv[1];

	iSocketFileDescriptor = Socket(AF_INET, SOCK_STREAM, 0);

	Address(AF_INET, (struct Address*) &sAddress, strServerIPAddress, HANGMAN_TCP_PORT);

	Connect(iSocketFileDescriptor, (struct sockaddr*) &sAddress.m_sAddress, sizeof(sAddress.m_sAddress));

	// Wrapper function to multiplex user input and network input on the 
	// socket file descriptor. MultiplexStdinFileDescriptor() implementation
	// can be found in the libsocket socket.c file.
	MultiplexStdinFileDescriptor(stdin, iSocketFileDescriptor);

	printf("Game over");
	return 0;
}
