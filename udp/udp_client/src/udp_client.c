#include "../../../libsocket/socket.h"
#include "../includes/definitions.h"
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
	char userInput[MAX_BUF_SIZE];

	socklen_t iServerAddrSize;

	// Connection to server is active initially.
	int iConnectionSuccess = 1;


	iSocketFileDescriptor = Socket(AF_INET, SOCK_DGRAM, 0);

	Address(AF_INET, (struct Address*) &sClientAddress, strClientIPAddress, 0);
	Address(AF_INET, (struct Address*) &sServerAddress, strServerIPAddress, HANGMAN_TCP_PORT);

	Bind(iSocketFileDescriptor, (struct sockaddr *) &sClientAddress.m_sAddress, sizeof(sClientAddress.m_sAddress));

	iServerAddrSize = sizeof(sServerAddress.m_sAddress);

	// Send Username to server
	sprintf(buffer, "%s_ ", strUsername);
	sendto(iSocketFileDescriptor, buffer, strlen(buffer) + 1, 0, (struct sockaddr*) &sServerAddress, sizeof(sServerAddress.m_sAddress));
	printf("Username %s sent to the server\n", strUsername);

	// Receive confirmation message from server
	printf("Waiting for confirmation message from the server...\n");
	recvfrom(iSocketFileDescriptor, buffer, MAX_BUF_SIZE, 0, (struct sockaddr*) &sServerAddress.m_sAddress, &iServerAddrSize);
	printf("%s\n", buffer);

	// If the server is full the connection is refused.
	// The client will not enter the read write loop if the conneciton fails.
	if(strstr(buffer, "failed") != NULL)
	{
		iConnectionSuccess = 0; // Connection refused
	}
	else
	{
		// Receive game session status if the connection was accepted
		iConnectionSuccess = 1; // Connection accepted

		printf("Waiting for game session status...\n");
		recvfrom(iSocketFileDescriptor, buffer, MAX_BUF_SIZE, 0, (struct sockaddr*) &sServerAddress.m_sAddress, &iServerAddrSize);
		printf("%s\n", buffer);
	}

	// Enter read/write loop only if the connection was accepted.
	while(iConnectionSuccess)
	{
		// Send guess to the server
		printf("send guess to the server:\n");
		fgets(userInput, sizeof(userInput), stdin);
		sprintf(buffer, "%s_%s", strUsername, userInput);
		printf("Sending: %s", buffer);
		sendto(iSocketFileDescriptor, buffer, strlen(buffer) + 1, 0, (struct sockaddr*) &sServerAddress, sizeof(sServerAddress.m_sAddress));

		// Receive reply from server
		printf("waiting for reply from the server...\n");
		recvfrom(iSocketFileDescriptor, buffer, MAX_BUF_SIZE, 0, (struct sockaddr*) &sServerAddress.m_sAddress, &iServerAddrSize);
		printf("%s\n", buffer);

		// If the servers reply contains 'won' or 'lost' break and close the socket.
		if(strstr(buffer, "lost") != NULL || strstr(buffer, "won") != NULL)
		{
			// Game over
			printf("Game over\n");
			break;
		}
	}

	close(iSocketFileDescriptor);
	return 0;
}
