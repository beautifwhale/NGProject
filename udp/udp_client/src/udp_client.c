/*
 * Hangclient.c - Client for hangman server.
 *
 * @author	David Morton, add names here...
 * @date	4.10.2015
*/
#include "../includes/Definitions.h"
#include "../../../libsocket/Sockets.h"

int main(int argc, char * argv[]) {
	int iSocketFileDescriptor;

	char* strServerIPAddress;
	char* strClientIPAddress;

	struct Address sServerAddress;
	struct Address sClientAddress;

	strServerIPAddress = argv[1];
	strClientIPAddress = "0.0.0.0";

	char buffer[MAX_BUF_SIZE];

	int iServerAddrLen;
	int iBytesRecieved;

	iSocketFileDescriptor = Socket(AF_INET, SOCK_DGRAM, 0);

	Address(AF_INET, (struct Address*) &sClientAddress, strClientIPAddress, 0);
	Address(AF_INET, (struct Address*) &sServerAddress, strServerIPAddress, HANGMAN_TCP_PORT);

	Bind(iSocketFileDescriptor, (struct sockaddr *) &sClientAddress.m_sAddress, sizeof(sClientAddress.m_sAddress));

	//Connect(iSocketFileDescriptor, (struct sockaddr*) &sAddress.m_sAddress, sizeof(sAddress.m_sAddress));

	//multiplexStdinFileDescriptor(stdin, iSocketFileDescriptor);

	strcpy(buffer, "Hello server");
	sendto(iSocketFileDescriptor, buffer, strlen(buffer) + 1, 0, (struct sockaddr*) &sServerAddress, sizeof(sServerAddress));

	iServerAddrLen = sizeof(sServerAddress);
	iBytesRecieved = recvfrom(iSocketFileDescriptor, buffer, MAX_BUF_SIZE, 0, (struct sockaddr*) &sServerAddress, &iServerAddrLen);
	write(1, buffer, iBytesRecieved);

	close(iSocketFileDescriptor);

	printf("Game over");
	return 0;
}
