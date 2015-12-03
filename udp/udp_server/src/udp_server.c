// udp_server.c
//
// Year 4 Networked Games Assignment 2015
// Team:	David Morton
//			Kevin Byrne
//			Derek O Brien
// 			add names here...
//
//
//
// Description: The server in the Hangman game will listen for messages for clients
// on port 1071. The port number is received from user input on the command line when
// running the server. When a message is received the clients username, password, and message are parsed from the
// packet string. The username is used as a search key in the array of game session structs, if one is found
// it is passed to ProcessRequest(), if not, a new game session is created and processed. Once the client message is processed
// control returns to main and the server listens for a new message.
//
// Dependencies: libhangman
//
#include <string.h>
#include "../external/libhangman/include/hangman.h"

int main(int argc, char* argv[]) {

	int iListenSocketFileDescriptor;
	// Create Address struct to store client information
	struct Address sClientAddress;
	sClientAddress.sendsize = sizeof(sClientAddress.sender);
	bzero(&sClientAddress.sender, sizeof(sClientAddress.sender));

	// Create GameSession struct to store clients game session data
	struct GameSession *gameSession;
	char buffer[MAX_BUF_SIZE];

	printf("Server initialising...\n");

	// Initialize all game sessions
	InitGameSessions();

	// Create a connection; Using NULL address to listen for all incoming
	// connections to server. Server port number 1071 and type TYPE_SERVER
	// Server will listen on port 1071 for any incoming connections
	iListenSocketFileDescriptor = InitConnection(NULL, "1071", TYPE_SERVER, SOCK_DGRAM);

	while(1)
	{
		// Print game session information stored in memory
		PrintActiveGameSessions();

		printf("Waiting for message from the client...\n");
		/*
		if(recvfrom(iListenSocketFileDescriptor, buffer, MAX_BUF_SIZE, 0, (struct sockaddr*) &sClientAddress.sender, &sClientAddress.sendsize) == 0)
		{
			printf("Client has closed connection\n");
			continue;
		}
		*/
		// Block and wait for a message from a client
		// Multiple clients may be sending messages, process
		// each one and return here to process another
		if(ReceiveMessage(iListenSocketFileDescriptor, buffer, MAX_BUF_SIZE, 0, (struct sockaddr*) &sClientAddress.sender, &sClientAddress.sendsize) == 0)
		{
			printf("Client has closed connection\n");
			continue;
		}

		// Client has sent a message to the server. Parse the ASCII message,
		// remove the username, and message, using strtok() to split the
		// message string on a '_' delimiter
		printf("Received: %s\n", (char*)buffer);
		printf("Processing packet...\n");
		char *username = strtok((char*)buffer, "_"); // Tokenize the string using '_' as delimiter
		char *secret = strtok((char*)NULL, "_"); // Get the secret
		char *message = strtok((char*)NULL, "_"); // Get the message


		printf("Connected to client %s with message %s and secret %s\n", username, message, secret);

		// Search for game session. Create one if none exists
		gameSession = FindGameSession(username, secret);
		if(gameSession == NULL)
		{
			// Server may be full or secret was incorrect
			printf("Game session retrieval\n");
		}

		// Authenticate client using secret


		// ProcessRequest from client and return to check next message
		if(PlayHangmanServerUDP(iListenSocketFileDescriptor, sClientAddress, gameSession, message) == -1)
		{
			printf("End game session and remove from memory\n");
			if(gameSession)
			{
				EndGameSession(gameSession);
			}
		}
	}
	return 0;
}




