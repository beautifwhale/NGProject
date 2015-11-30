#include "../../../libsocket/socket.h"
#include "../includes/definitions.h"
#include "../includes/game.h"
#include <string.h>

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
	iListenSocketFileDescriptor = Connection(NULL, "1071", TYPE_SERVER);

	while(1)
	{
		printf("waiting for message from the client...\n");
		if(recvfrom(iListenSocketFileDescriptor, buffer, MAX_BUF_SIZE, 0, (struct sockaddr*) &sClientAddress.sender, &sClientAddress.sendsize) == 0)
		{
			printf("Client has closed connection\n");
			continue;
		}
		printf("Received: %s\n", (char*)buffer);
		printf("Processing packet...\n");
		char *username = strtok((char*)buffer, "_"); // tokenize the string using space as delimiter. Get username
		char *message = strtok((char*)NULL, "_"); // get the remaining message

		printf("Connected to client %s with message %s\n", username, message);

		PrintActiveGameSessions();

		// Search for game session and create one if none exists
		gameSession = FindGameSession(username);
		if(gameSession == NULL)
		{
			printf("No more game slots available\n");
		}

		// ProcessRequest from client and return to check next message
		if(ProcessRequest(iListenSocketFileDescriptor, sClientAddress, gameSession, message) == -1)
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




