/* Network server for hangman game */
/* File: hangserver.c */

#include "../includes/Definitions.h"
#include "../includes/Sockets.h"
#include "../includes/Game.h"


int main(int argc, char* argv[]) {
	int iSocketFileDescriptor;
	char * strServerIPAddress;
	struct Address sAddress;

	int fd;
	int client_len;

	struct sockaddr_in client;

	strServerIPAddress = "0.0.0.0";

	iSocketFileDescriptor = Socket(AF_INET, SOCK_STREAM, 0);

	Address(AF_INET, (struct Address*) &sAddress, strServerIPAddress, HANGMAN_TCP_PORT);


	//Bind();
	//Listen();
	//Accept();

	if (bind(iSocketFileDescriptor, (struct sockaddr *) &sAddress.m_sAddress, sizeof(sAddress.m_sAddress)) < 0) {
		perror("binding socket");
		exit(2);
	}

	listen(iSocketFileDescriptor, 5);

	//Signal();

	while (1) {
		client_len = sizeof(client);
		if ((fd = accept(iSocketFileDescriptor, (struct sockaddr *) &client, &client_len)) < 0) {
			perror("accepting connection");
			exit(3);
		}

		/* ---------------- Play_hangman () ---------------------*/
		play_hangman(fd, fd);

		close(fd);
	}
	return 0;
}



