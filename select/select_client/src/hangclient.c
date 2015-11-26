/*
 * Hangclient.c - Client for hangman server.
 *
 * @author	David Morton, Kevin Byrne
 * @date	4.10.2015
*/
#include "../includes/Definitions.h"
#include "../../../libsocket/Sockets.h"

int main(int argc, char * argv[]) {
	int iSocketFileDescriptor;
	char * strServerIPAddress;
	struct Address sAddress;

	strServerIPAddress = argv[1];

	iSocketFileDescriptor = Socket(AF_INET, SOCK_STREAM, 0);

	Address(AF_INET, (struct Address*) &sAddress, strServerIPAddress, HANGMAN_TCP_PORT);

	Connect(iSocketFileDescriptor, (struct sockaddr*) &sAddress.m_sAddress, sizeof(sAddress.m_sAddress));

	multiplexStdinFileDescriptor(stdin, iSocketFileDescriptor);

	printf("Game over");
	return 0;
}
