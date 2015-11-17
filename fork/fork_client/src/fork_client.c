#include "../../../libsocket/socket.h"
#include "../includes/definitions.h"

int main(int argc, char * argv[]) {
	int iSocketFileDescriptor;
	char * strServerIPAddress;
	struct Address sAddress;

	if (argc != 2)
   	{
      		printf("usage:  udpclient <IP address>\n");
     		exit(1);
   	}
	strServerIPAddress = argv[1];

	iSocketFileDescriptor = Socket(AF_INET, SOCK_STREAM, 0);

	Address(AF_INET, (struct Address*) &sAddress, strServerIPAddress, HANGMAN_TCP_PORT);

	Connect(iSocketFileDescriptor, (struct sockaddr*) &sAddress.m_sAddress, sizeof(sAddress.m_sAddress));

	MultiplexStdinFileDescriptor(stdin, iSocketFileDescriptor);

	printf("Game over");
	return 0;
}
