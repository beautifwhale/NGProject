/* Network server for hangman game */
/* File: hangserver.c */

#include "../includes/Definitions.h"
#include "../../../libsocket/Sockets.h"
#include "../includes/Game.h"
#include <string.h>

void initGameSessions(GameSession* gameSessions /* array of game sessions*/)
{
	int i;
	for(i = 0; i < MAX_GAME_SESSIONS; i++)
	{
		gameSessions[i].strUsername = "null";
		gameSessions[i].index = i;
		gameSessions[i].cGameState = 'U'; // unknown

	}
}

void printActiveGameSessions(GameSession* gameSessions)
{
	int i;
	for(i = 0; i < MAX_GAME_SESSIONS; i++)
	{
		printf("%d index %d %s\n", i, gameSessions[i].index, gameSessions[i].strUsername);
	}
}

int main(int argc, char* argv[]) {
	int iListenSocketFileDescriptor;
	char * strServerIPAddress;
	struct Address sAddress;
	struct Address sClientAddress;

	struct GameSession gameSessions[MAX_GAME_SESSIONS];
	int iNumberOfGames = 0;
	int gameSessionId = 0;

	// initialize game sessions
	initGameSessions(gameSessions);
	printActiveGameSessions(gameSessions);

	strServerIPAddress = "0.0.0.0";

	socklen_t iClientAddrLen;
	//int iStatus;
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
		/*
		iClientAddrLen = sizeof(sClientAddress);
		iStatus = recvfrom(iListenSocketFileDescriptor, buffer, MAX_BUF_SIZE, 0, (struct sockaddr*) &sClientAddress, &iClientAddrLen);

		printf("%s", buffer);

		if(strcmp(buffer, "exit\n") == 0)
		{
			exitFlag = 1;
			strcat(buffer, "true\n");
			printf("server shutting down because exit signal recieved from client");
		}

		iStatus = sendto(iListenSocketFileDescriptor, buffer, strlen(buffer) + 1, 0, (struct sockaddr*) &sClientAddress, sizeof(sClientAddress));
		*/
		recvfrom(iListenSocketFileDescriptor, buffer, MAX_BUF_SIZE, 0, (struct sockaddr*) &sClientAddress, &iClientAddrLen);
		printf("received username from client %s\n", buffer);

		// search for game session and create one if none exists
		gameSessionId = findGameSession(gameSessions, MAX_GAME_SESSIONS, buffer);
		if(gameSessionId < 0)
		{
			printf("No more game slots available");
			// send message to client
		}

		printActiveGameSessions(gameSessions);

		play_hangman(iListenSocketFileDescriptor, iListenSocketFileDescriptor, sClientAddress, &gameSessions[gameSessionId]);
		break;
	}

	/*
	printf("listening for connections\n");
	for( ; ; ) {
		client_len = sizeof(sAddress.m_sAddress);
		// Accept connections from clients
		if ((connfd = accept(iListenSocketFileDescriptor, (struct sockaddr *) &sAddress.m_sAddress, &client_len)) < 0)
		{
			// There was an error (interrupt)
			if( errno == EINTR )
			{
				// Try another Accept() in the event of a system call interrupt
				continue;
			}
			else
			{
				// There was an error other than an interrupt so close the Parent process
				perror("Accept error");
				exit(3);
			}
		}

		// There was no error in Accept()! Woo! Create a child process
		if( (childProcessID = fork()) == 0)
		{
			// CHILD
			//printf("child %d created\n", childProcessID);

			// close the parents listen file descriptor in the child
			close(iListenSocketFileDescriptor);

			// ---------------- Play_hangman () ---------------------
			play_hangman(connfd, connfd);


			 // On return exit to kill the process. The kernel will then
			 // send a signal to the parent which is caught by the parents
			 // signalHandler() set in Signal()

			exit(0);
		}

		close(connfd);
	}
	*/
	close(iListenSocketFileDescriptor);
	return 0;
}

int findGameSession(GameSession* gameSessions, int len, char* username)
{
	printf("Searching for game session...\n");
	int gameSessionId = 0;
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



