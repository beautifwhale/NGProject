#include <string.h>
#include "../../../libsocket/socket.h"
#include "../includes/definitions.h"
#include "../includes/game.h"

int main(int argc, char* argv[]) {
	int iListenSocketFileDescriptor;
	char * strServerIPAddress;
	struct Address sAddress;
	struct Address sClientAddress;

	struct GameSession gameSessions[MAX_GAME_SESSIONS];
	int gameSessionId = 0;

	// initialize game sessions
	initGameSessions(gameSessions);

	strServerIPAddress = "0.0.0.0";

	socklen_t iClientAddrLen;
	char* buffer[MAX_BUF_SIZE];

	printf("Server: initialising\n");

	iListenSocketFileDescriptor = Socket(AF_INET, SOCK_DGRAM, 0);

	// Use command line input to pass in the hostname and service port number.
	// AddressIPX("www.google.com", "1071", struct addrinfo *hints, struct addrinfo** result);
	Address(AF_INET, (struct Address*) &sAddress, strServerIPAddress, HANGMAN_TCP_PORT);

	Bind(iListenSocketFileDescriptor, (struct sockaddr *) &sAddress.m_sAddress, sizeof(sAddress.m_sAddress));

	iClientAddrLen = sizeof(sClientAddress.m_sAddress);

	while(1)
	{
		printf("waiting for message from the client...\n");
		if(recvfrom(iListenSocketFileDescriptor, buffer, MAX_BUF_SIZE, 0, (struct sockaddr*) &sClientAddress, &iClientAddrLen) == 0)
		{
			printf("Client has closed connection\n");
			continue;
		}
		printf("Received: %s\n", (char*)buffer);
		printf("Processing packet...\n");
		char *username = strtok((char*)buffer, "_"); // tokenize the string using space as delimiter. Get username
		char *message = strtok((char*)NULL, "_"); // get the remaining message

		printf("Connected to client %s with message %s\n", username, message);

		PrintActiveGameSessions(gameSessions);

		// search for game session and create one if none exists
		gameSessionId = FindGameSession(gameSessions, MAX_GAME_SESSIONS, username);
		if(gameSessionId < 0)
		{
			printf("No more game slots available");
			// send message to client
		}

		// ProcessRequest from client and return to check next message
		if(ProcessRequest(iListenSocketFileDescriptor, iListenSocketFileDescriptor, sClientAddress, &gameSessions[gameSessionId], message) == -1)
		{
			printf("End game session and remove from memory\n");
			EndGameSession(&gameSessions[gameSessionId]);
		}

		/*
		printf("calling play_hangman()\n");
		if(play_hangman(iListenSocketFileDescriptor, iListenSocketFileDescriptor, sClientAddress, &gameSessions[gameSessionId]) == -1)
		{
			printf("closing socket file descriptor\n");
			close(iListenSocketFileDescriptor);
		}
		*/
	}
	return 0;
}




