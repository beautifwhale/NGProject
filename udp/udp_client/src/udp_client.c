#include "../../../libsocket/socket.h"
#include <string.h>
#include "../includes/definitions.h"

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

	printf("Username: %s\n", strUsername);

	char buffer[MAX_BUF_SIZE];
	char userInput[MAX_BUF_SIZE];

	int iBytesRecieved = 0;
	socklen_t iServerAddrSize;

	iSocketFileDescriptor = Socket(AF_INET, SOCK_DGRAM, 0);

	Address(AF_INET, (struct Address*) &sClientAddress, strClientIPAddress, 0);
	Address(AF_INET, (struct Address*) &sServerAddress, strServerIPAddress, HANGMAN_TCP_PORT);

	Bind(iSocketFileDescriptor, (struct sockaddr *) &sClientAddress.m_sAddress, sizeof(sClientAddress.m_sAddress));

	iServerAddrSize = sizeof(sServerAddress.m_sAddress);

	// Send username to server
	sprintf(buffer, "%s_ ", strUsername);
	sendto(iSocketFileDescriptor, buffer, strlen(buffer) + 1, 0, (struct sockaddr*) &sServerAddress, sizeof(sServerAddress.m_sAddress));
	printf("Username %s sent to the server\n", strUsername);

	// receive confirmation message from server
	printf("waiting for confirmation message from the server...\n");
	iBytesRecieved = recvfrom(iSocketFileDescriptor, buffer, MAX_BUF_SIZE, 0, (struct sockaddr*) &sServerAddress.m_sAddress, &iServerAddrSize);
	printf("%s\n", buffer);

	// receive game session status
	printf("waiting for game session status...\n");
	recvfrom(iSocketFileDescriptor, buffer, MAX_BUF_SIZE, 0, (struct sockaddr*) &sServerAddress.m_sAddress, &iServerAddrSize);
	printf("%s\n", buffer);

	while(1)
	{
		// send guess to the server
		printf("send guess to the server:\n");
		fgets(userInput, sizeof(userInput), stdin);
		sprintf(buffer, "%s_%s", strUsername, userInput);
		printf("Sending: %s", buffer);
		sendto(iSocketFileDescriptor, buffer, strlen(buffer) + 1, 0, (struct sockaddr*) &sServerAddress, sizeof(sServerAddress.m_sAddress));

		// receive reply from server
		printf("waiting for reply from the server...\n");
		recvfrom(iSocketFileDescriptor, buffer, MAX_BUF_SIZE, 0, (struct sockaddr*) &sServerAddress.m_sAddress, &iServerAddrSize);
		printf("%s\n", buffer);
		if(strstr(buffer, "lost") != NULL || strstr(buffer, "won") != NULL)
		{
			// Game over
			break;
		}
	}

	//multiplexStdinFileDescriptor(stdin, iSocketFileDescriptor);

	close(iSocketFileDescriptor);
	printf("Game over\n");
	return 0;
}
