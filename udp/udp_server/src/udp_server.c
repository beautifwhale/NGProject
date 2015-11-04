#include <string.h>
#include "../../../libsocket/socket.h"
#include "../includes/definitions.h"
#include "../includes/game.h"

void initGameSessions(GameSession* gameSessions /* array of game sessions*/)
{
	int i;
	for(i = 0; i < MAX_GAME_SESSIONS; i++)
	{
		gameSessions[i].strUsername = "null";
		gameSessions[i].cGameState = 'U'; // unknown

	}
}

void printActiveGameSessions(GameSession* gameSessions)
{
	printf("active game sessions:\n");
	int i;
	for(i = 0; i < MAX_GAME_SESSIONS; i++)
	{
		if(strcmp(gameSessions[i].strUsername, "null"))
		{
			printf("%d %s\n", i, gameSessions[i].strUsername);
		}
	}
}

int findGameSession(GameSession* gameSessions, int len, char* username)
{
	printf("Searching for game session...\n");
	int i;
	for(i = 0; i < len; i++)
	{
		if(strcmp(gameSessions[i].strUsername, username) == 0)
		{
			printf("Game session found!\n");
			return i; // return game session id
		}
	}

	// no game session found, create a new game session
	for(i = 0; i < len; i++)
	{
		if(strcmp(gameSessions[i].strUsername, "null") == 0)
		{
			printf("Empty slot found!\n");
			gameSessions[i].strUsername = username;
			return i; // return game session id
		}
	}

	// there are no game session slots left
	return -1;
}


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

	//Listen(iListenSocketFileDescriptor, MAX_LISTEN_QUEUE_SIZE);

	// signal handler for terminated processes
	//Signal(SIGCHLD, signalHandler);
	//int exitFlag = 0;

	iClientAddrLen = sizeof(sClientAddress.m_sAddress);

	while(1)
	{

		printf("waiting for username from the client...\n");
		recvfrom(iListenSocketFileDescriptor, buffer, MAX_BUF_SIZE, 0, (struct sockaddr*) &sClientAddress, &iClientAddrLen);
		printf("received username from client %s\n", (char*)buffer);

		// search for game session and create one if none exists
		gameSessionId = findGameSession(gameSessions, MAX_GAME_SESSIONS, (char*)buffer);
		if(gameSessionId < 0)
		{
			printf("No more game slots available");
			// send message to client
		}

		printActiveGameSessions(gameSessions);

		printf("calling play_hangman()\n");

		// ProcessRequest from client and return to check next message
		play_hangman(iListenSocketFileDescriptor, iListenSocketFileDescriptor, sClientAddress, &gameSessions[gameSessionId]);

		printf("closing socket file descriptor\n");
		close(iListenSocketFileDescriptor);
		break;
	}
	return 0;
}




