// udp_client.c
//
// Year 4 Networked Games Assignment 2015
// Team:	David Morton
//			Kevin Byrne
// 			add names here...
//
// Description: The UDP client will take the username, secret password, hostname, and
// service port number from the command line. It will then create a connection with the server
// by sending the clients username and password for the game session. If the server replies with a
// success message the client will take user input and send it to the server until the player has won or
// lost the game. Games can be resumed if closed, server holds state on game session.
//
// Dependencies: libhangman
//
//#include "hangman.h" // Will also work because cmake includes API header files
#include "../external/libhangman/include/hangman.h"
#include <string.h>

int main(int argc, char * argv[])
{
	int iSocketFileDescriptor;

	char* strServerIPAddress;
	char* strSerivceName;
	char* strUsername;
	char* strSecret;

	int sequenceNumber = 0;

	char buffer[MAX_BUF_SIZE];
	char userInput[MAX_BUF_SIZE];

	// Address structure to hold information about
	// addressing type and storage. See libsocket
	struct Address sServerAddress;
	sServerAddress.sendsize = sizeof(sServerAddress.sender);
	bzero(&sServerAddress.sender, sizeof(sServerAddress.sender));

 	if (argc != 5)
  	{
		printf("usage: clientUDP <IP address> <service name/port number> <userName> <secret>\n");
		exit(1);
   	}

 	// Get server hostname, service name, username, and password from the user
	strServerIPAddress = argv[1];
	strSerivceName = argv[2];
	strUsername = argv[3];
	strSecret = argv[4];

	// Connection to server is active initially.
	int iConnectionSuccess = 1;

	// Create a connection to the server using the servers IPv4 or IPv6 address, and port number.
	// The connection type is TYPE_CLIENT to specify client application
	iSocketFileDescriptor = InitConnection(strServerIPAddress, strSerivceName, TYPE_CLIENT, SOCK_DGRAM);

	// Send Username and secret to server and add '_' delimiters
	sprintf(buffer, "%s_%s_ ", strUsername, strSecret);

	SendMessage(iSocketFileDescriptor, buffer, strlen(buffer) + 1, 0);
	printf("Username %s and Secret <%s> sent to the server\n", strUsername, strSecret);

	// Receive message from server, this may signify a failure or
	// a successful connection.
	printf("Waiting for confirmation message from the server...\n");

	ReceiveMessage(iSocketFileDescriptor, buffer, MAX_BUF_SIZE, 0, (struct sockaddr*) &sServerAddress.sender, &sServerAddress.sendsize);
	printf("%s\n", buffer);

	// If the server is full the connection is refused.
	// The client will not enter the read write loop if the connection fails.
	if(strstr(buffer, "failed") != NULL)
	{
		iConnectionSuccess = 0; // Connection refused
	}
	else
	{
		// Receive game session status if the connection was accepted
		iConnectionSuccess = 1; // Connection accepted

		printf("Waiting for game session status...\n");

		ReceiveMessage(iSocketFileDescriptor, buffer, MAX_BUF_SIZE, 0, (struct sockaddr*) &sServerAddress.sender, &sServerAddress.sendsize);
		printf("%s\n", buffer);
	}

	// Enter read/write loop only if the connection was accepted.
	while(iConnectionSuccess)
	{
		// Get the users guess of the word and send it
		// to the server. Input is taken from stdin.
		printf("Send guess to the server:\n");
		fgets(userInput, sizeof(userInput), stdin);

		sprintf(buffer, "%s_%s_%c", strUsername, strSecret, userInput[0]);
		printf("Sending: %s\n", buffer);
		SendMessage(iSocketFileDescriptor, buffer, strlen(buffer) + 1, 0);

		// Receive reply from server
		printf("Waiting for reply from the server...\n");

		ReceiveMessage(iSocketFileDescriptor, buffer, MAX_BUF_SIZE, 0, (struct sockaddr*) &sServerAddress.sender, &sServerAddress.sendsize);
		printf("%s\n", buffer);

		// If the servers reply contains 'won' or 'lost' break and close the socket.
		if(strstr(buffer, "lost") != NULL || strstr(buffer, "won") != NULL)
		{
			// Game over
			printf("Game over\n");
			break;
		}
	}

	close(iSocketFileDescriptor);
	return 0;
}
