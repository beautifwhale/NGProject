/*
 * Hangclient.c - Client for hangman server.
 *
 * @author	David Morton, add names here...
 * @date	4.10.2015
*/
#include "../includes/Definitions.h"
//#include "../includes/Sockets.h"
#include "../../../libsocket/Sockets.h"

int main(int argc, char * argv[]) {
	int iSocketFileDescriptor;
	char * strServerIPAddress;
	struct Address sAddress;

	//strServerIPAddress = (argc == 1) ? argv[1] : "localhost";
	strServerIPAddress = argv[1];

	iSocketFileDescriptor = Socket(AF_INET, SOCK_STREAM, 0);

	Address(AF_INET, (struct Address*) &sAddress, strServerIPAddress, HANGMAN_TCP_PORT);

	Connect(iSocketFileDescriptor, (struct sockaddr*) &sAddress.m_sAddress, sizeof(sAddress.m_sAddress));

	/*OK connected to the server.
	 Take a line from the server and show it, take a line and send the user input to the server.
	 Repeat until the server terminates the connection. */

	// Select() start game
	multiplexStdinFileDescriptor(stdin, iSocketFileDescriptor);

	printf("Game over");
	return 0;
}
