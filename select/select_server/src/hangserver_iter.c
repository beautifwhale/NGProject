/* Network server_addr for hangman game */
/* File: hangserver_addr.c */

/*
 * 	Select working 
 *	
 *
 */

#include "../includes/Game.h"
#include "../includes/Definitions.h"
#include "../../../libsocket/socket.h"
#include <string.h>  strlen()

extern time_t time();

int main(int argc, char* argv[]) {
	int listeningFileDescriptor, addrlen, new_socket, i, read_socket, nbytes,
			max_sd;
	int client_socket[MAX_GAME_SESSIONS];
	struct Address sClientAddress;

	char buffer[MAXLEN];
	char userBuffer[MAXLEN];
	char *username;

	//set of socket descriptors
	fd_set readfds;

	char * strServerIPAddress;
	strServerIPAddress = "0.0.0.0";

	struct GameSession *gameSession;

	// Initialize all game sessions
	InitGameSessions();

	//welcome message
	char *message = "Welcome to hangman server please enter your quess\n\0";

	//initialise all client_socket[] to 0 so not checked
	for (i = 0; i < MAX_GAME_SESSIONS; i++) {
		client_socket[i] = 0;
	}

	printf("\n--------Setting up Connection-----------\n");
	/* create the listener socket*/
	listeningFileDescriptor = Socket(AF_INET, SOCK_STREAM, 0);
	printf("Socket() is OK...\n");

	/* Address */
	Address(AF_INET, (struct Address*) &sClientAddress, strServerIPAddress,
			HANGMAN_TCP_PORT);
	printf("Address() is OK...\n");

	/* bind */
	Bind(listeningFileDescriptor,
			(struct sockaddr *) &sClientAddress.m_sAddress,
			sizeof(sClientAddress.m_sAddress));
	printf("Bind() is OK...\n");

	/* listen */
	Listen(listeningFileDescriptor, MAX_GAME_SESSIONS);
	printf("Listen() is OK...\n");

	/*Set address size */
	addrlen = sizeof(sClientAddress.m_sAddress);

	printf("--------End Setting up Connection -------\n");

	printf("--------Wating for connections-----------\n");
	while (1) {
		//clear the socket set
		FD_ZERO(&readfds);

		//add master socket to filedesciptorset
		FD_SET(listeningFileDescriptor, &readfds);
		max_sd = listeningFileDescriptor;

		//add read_socket sockets to set
		for (i = 0; i < MAX_GAME_SESSIONS; i++) {
			//socket descriptor
			read_socket = client_socket[i];

			//if valid socket descriptor then add to read list
			if (read_socket > 0)
				FD_SET(read_socket, &readfds);

			//highest file descriptor number, need it for the select function
			if (read_socket > max_sd)
				max_sd = read_socket;
		}

		//wait for an activity on one of the sockets , timeout is NULL , so wait indefinitely
		Select(max_sd + 1, &readfds, NULL, NULL, NULL);

		//If something happened on the master socket , then its an incoming connection
		if (FD_ISSET(listeningFileDescriptor, &readfds)) {
			new_socket = accept(listeningFileDescriptor,
					(struct sockaddr *) &sClientAddress.m_sAddress,
					(socklen_t*) &addrlen);

			/*Get Username and print*/
			Read(new_socket, userBuffer, sizeof(userBuffer));
			username = (char*) userBuffer;
			printf("Received username: %s\n", (char *) username);

			//inform user of socket number - used in send and receive commands
			printf("New connection on socket: %d \n", new_socket);

			/*Write Welcome Message*/
			write(new_socket, message, strlen(message));

			//add new socket to array of sockets
			for (i = 0; i < MAX_GAME_SESSIONS; i++) {
				//if slot is empty
				if (client_socket[i] == 0) {
					client_socket[i] = new_socket;
					printf("Adding socket list of sockets as %d\n", i);

					break;
				}
			}
			//Build game session for client
			gameSession = BuildGame((char*) username, new_socket);

			//Print out active sessions
			PrintActiveGameSessions();
		}

		//else its some IO operation on some other socket
		for (i = 0; i < MAX_GAME_SESSIONS; i++) {
			read_socket = client_socket[i];

			if (FD_ISSET(read_socket, &readfds)) {
				//Check if it was for closing , and also read the incoming message

				if ((nbytes = read(read_socket, buffer, sizeof(buffer))) == 0) {

					printf("Socket %d Closed \n", read_socket);
					//Close the socket and mark as 0 in list for reuse
					close(read_socket);
					client_socket[i] = 0;
				}

				//Play game with guess
				else {

					//Find which client guessed
					gameSession = FindGameSession(read_socket);

					// ProcessRequest for client with is game session and message
					if (ProcessRequest(gameSession->iSessionId, gameSession,
							buffer) == -1) {
						printf("End game session and remove from memory\n");
						if (gameSession) {
							EndGameSession(gameSession);
						}
					}
				}
			}
		}
	}

	return 0;
}

