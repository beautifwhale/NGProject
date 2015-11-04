/*
 * Hangclient.c - Client for hangman server.
 *
 * @author	David Morton, add names here...
 * @date	4.10.2015
*/
#include "../includes/Definitions.h"
#include "../../../libsocket/socket.h"
#include <string.h>

int main(int argc, char * argv[])
{
	int iSocketFileDescriptor;

	char* strServerIPAddress;
	char* strClientIPAddress;

	struct Address sServerAddress;
	struct Address sClientAddress;

	strServerIPAddress = argv[1];
	strClientIPAddress = "0.0.0.0";

	char* strUsername;
	strUsername = argv[2];

	char buffer[MAX_BUF_SIZE];

	int iBytesRecieved = 0;
	socklen_t iServerAddrSize;

	iSocketFileDescriptor = Socket(AF_INET, SOCK_DGRAM, 0);

	Address(AF_INET, (struct Address*) &sClientAddress, strClientIPAddress, 0);
	Address(AF_INET, (struct Address*) &sServerAddress, strServerIPAddress, HANGMAN_TCP_PORT);

	Bind(iSocketFileDescriptor, (struct sockaddr *) &sClientAddress.m_sAddress, sizeof(sClientAddress.m_sAddress));

	Connect(iSocketFileDescriptor, (struct sockaddr*) &sClientAddress.m_sAddress, sizeof(sClientAddress.m_sAddress));

	//multiplexStdinFileDescriptor(stdin, iSocketFileDescriptor);

	iServerAddrSize = sizeof(sServerAddress.m_sAddress);

	// send username to server
	sprintf(buffer, "%s", strUsername);
	sendto(iSocketFileDescriptor, buffer, strlen(buffer) + 1, 0, (struct sockaddr*) &sServerAddress, sizeof(sServerAddress.m_sAddress));
	printf("Username %s sent to the server\n", strUsername);

	// receive confirmation message from server
	iBytesRecieved = recvfrom(iSocketFileDescriptor, buffer, MAX_BUF_SIZE, 0, (struct sockaddr*) &sServerAddress.m_sAddress, &iServerAddrSize);
	printf("%s", buffer);

	// receive game session status
	iBytesRecieved = recvfrom(iSocketFileDescriptor, buffer, MAX_BUF_SIZE, 0, (struct sockaddr*) &sServerAddress.m_sAddress, &iServerAddrSize);
	printf("%s", buffer);

	while(iBytesRecieved > 0)
	{
		// send guess to the server
		fgets(buffer, sizeof(buffer), stdin);
		sendto(iSocketFileDescriptor, buffer, strlen(buffer) + 1, 0, (struct sockaddr*) &sServerAddress, sizeof(sServerAddress.m_sAddress));

		// receive reply from server
		iBytesRecieved = recvfrom(iSocketFileDescriptor, buffer, MAX_BUF_SIZE, 0, (struct sockaddr*) &sServerAddress.m_sAddress, &iServerAddrSize);
		printf("%s Bytes: %d\n", buffer, iBytesRecieved);
		if(iBytesRecieved < 0)
		{
			// server has terminated the connection
			perror("Server terminated");
			exit(0);
		}

	}

	//multiplexStdinFileDescriptor(stdin, iSocketFileDescriptor);

	close(iSocketFileDescriptor);

	printf("Game over\n");
	return 0;
}
