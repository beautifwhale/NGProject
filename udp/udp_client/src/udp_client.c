// udp_client.c
//
// Year 4 Networked Games Assignment 2015
// Team:	David Morton
//			Kevin Byrne
// 			add names here...
//
//
//
// Description: The UDP client
//
#include "../../../libsocket/socket.h"
#include "../../../libhangman/hangman.h"
#include <string.h>

int main(int argc, char * argv[])
{
	int iSocketFileDescriptor;

	char* strServerIPAddress;

	struct Address sServerAddress;
	sServerAddress.sendsize = sizeof(sServerAddress.sender);
	bzero(&sServerAddress.sender, sizeof(sServerAddress.sender));

 	if (argc != 3)
  	{
		printf("usage:  udpclient <IP address> <userName>\n");
		exit(1);
   	}

	strServerIPAddress = argv[1];

	char* strUsername;
	strUsername = argv[2];

	char buffer[MAX_BUF_SIZE];
	char userInput[MAX_BUF_SIZE];

	// Connection to server is active initially.
	int iConnectionSuccess = 1;

	// Create a connection to the server using the servers IPv4 or IPv6 address, and port number.
	// The connection type is TYPE_CLIENT to specify client application
	iSocketFileDescriptor = Connection(strServerIPAddress, "1071", TYPE_CLIENT, SOCK_DGRAM);

	// Send Username to server and add '_' delimiter
	sprintf(buffer, "%s_ ", strUsername);
	send(iSocketFileDescriptor, buffer, strlen(buffer) + 1, 0);
	printf("Username %s sent to the server\n", strUsername);

	// Receive message from server, this may signify a failure or
	// a successful connection.
	printf("Waiting for confirmation message from the server...\n");
	//recvfrom(iSocketFileDescriptor, buffer, MAX_BUF_SIZE, 0, (struct sockaddr*) &sServerAddress.sender, &sServerAddress.sendsize);
	ReceiveFromServer(iSocketFileDescriptor, buffer, MAX_BUF_SIZE, 0, (struct sockaddr*) &sServerAddress.sender, &sServerAddress.sendsize);
	printf("%s\n", buffer);

	// If the server is full the connection is refused.
	// The client will not enter the read write loop if the connection fails.
	if(strstr(buffer, "failed") != NULL)
	{
		printf("Connection to server failed. Server may be full.\n");
		iConnectionSuccess = 0; // Connection refused
	}
	else
	{
		// Receive game session status if the connection was accepted
		iConnectionSuccess = 1; // Connection accepted

		printf("Waiting for game session status...\n");
		//recvfrom(iSocketFileDescriptor, buffer, MAX_BUF_SIZE, 0, (struct sockaddr*) &sServerAddress.sender, &sServerAddress.sendsize);
		ReceiveFromServer(iSocketFileDescriptor, buffer, MAX_BUF_SIZE, 0, (struct sockaddr*) &sServerAddress.sender, &sServerAddress.sendsize);
		printf("%s\n", buffer);
	}

	// Enter read/write loop only if the connection was accepted.
	while(iConnectionSuccess)
	{
		// Get the users guess of the word and send it
		// to the server. Input is taken from stdin.
		printf("Send guess to the server:\n");
		fgets(userInput, sizeof(userInput), stdin);
		sprintf(buffer, "%s_%s", strUsername, userInput);
		printf("Sending: %s", buffer);
		send(iSocketFileDescriptor, buffer, strlen(buffer) + 1, 0);

		// Receive reply from server
		printf("Waiting for reply from the server...\n");
		//recvfrom(iSocketFileDescriptor, buffer, MAX_BUF_SIZE, 0, (struct sockaddr*) &sServerAddress.sender, &sServerAddress.sendsize);
		ReceiveFromServer(iSocketFileDescriptor, buffer, MAX_BUF_SIZE, 0, (struct sockaddr*) &sServerAddress.sender, &sServerAddress.sendsize);
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
