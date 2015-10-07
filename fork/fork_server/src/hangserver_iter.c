/* Network server for hangman game */
/* File: hangserver.c */

#include "../includes/Definitions.h"
#include "../includes/Sockets.h"
#include "../includes/Game.h"


int main(int argc, char* argv[]) {
	int iListenSocketFileDescriptor;
	char * strServerIPAddress;
	struct Address sClientAddress;
	pid_t childProcessID;
	int connfd;
	int client_len;

	strServerIPAddress = "0.0.0.0";

	printf("Server: initialising\n");

	iListenSocketFileDescriptor = Socket(AF_INET, SOCK_STREAM, 0);

	Address(AF_INET, (struct Address*) &sClientAddress, strServerIPAddress, HANGMAN_TCP_PORT);


	//Bind();
	//Listen();
	//Accept();

	if (bind(iListenSocketFileDescriptor, (struct sockaddr *) &sClientAddress.m_sAddress, sizeof(sClientAddress.m_sAddress)) < 0) {
		perror("binding socket");
		exit(2);
	}

	listen(iListenSocketFileDescriptor, 5);

	// signal handler for terminated processes
	Signal(SIGCHLD, signalHandler);
	//signal(SIGCHLD, signalHandler);

	printf("listening for connections\n");
	for( ; ; ) {
		client_len = sizeof(sClientAddress.m_sAddress);
		if ((connfd = accept(iListenSocketFileDescriptor, (struct sockaddr *) &sClientAddress.m_sAddress, &client_len)) < 0) {
			if( errno == EINTR )
			{
				// try another accept
				continue;
			}
			else
			{
				perror("accept error");
				exit(3);
			}
		}

		if( (childProcessID = fork()) == 0)
		{
			printf("child created\n");
			close(iListenSocketFileDescriptor);
			/* ---------------- Play_hangman () ---------------------*/
			play_hangman(connfd, connfd);
			exit(0);
		}
		close(connfd);
	}
	return 0;
}



