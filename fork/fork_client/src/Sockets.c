/*
 * Socket.c
 *
 *  Created on: 4 Oct 2015
 *      Author: david
 */
#include "../includes/Sockets.h"

int Socket(int family, int type, int protocol)
{
	int sock = socket(family, type, protocol);
	if (sock < 0)
	{
		perror("Creating stream socket");
		exit(1);
	}
	return sock;
}

void Address(int family, struct Address* address, char* ipAddress, int portNumber)
{
	// create the server address
	address->host_info = gethostbyname(ipAddress);
	if (address->host_info == NULL)
	{
		fprintf(stderr, "unknown host:%s \n", ipAddress);
		exit(2);
	}

	//
	address->server.sin_family = address->host_info->h_addrtype; // set protocol family

	// address struct, network address from host_info, size of host_info
	memcpy((char *) &address->server.sin_addr, address->host_info->h_addr, address->host_info->h_length);

	address->server.sin_port = htons(portNumber); // set server port number
}

void Connect(int socketFileDescriptor, const struct sockaddr* socketAddress, socklen_t socketSize)
{
	if (connect(socketFileDescriptor, socketAddress, socketSize) < 0)
	{
		perror("connecting to server");
		exit(3);
	}
}

int Select(int maxFileDescriptorsPlus1, fd_set *readFileDescriptorSet, fd_set *writeFileDescriptorSet, fd_set *exceptFileDescriptorSet, struct timeval *timeout)
{
	int n;
	if ( (n = select(maxFileDescriptorsPlus1, readFileDescriptorSet, writeFileDescriptorSet, exceptFileDescriptorSet, timeout)) < 0)
	{
		perror("Error in Select()");
		exit(0);
	}
	return(n);		/* can return 0 on timeout */
}

ssize_t Read(int fileDescriptor, void *buffer, size_t numberOfBytes)
{
	ssize_t n;
	if ( (n = read(fileDescriptor, buffer, numberOfBytes)) == -1)
	{
		perror("Error in Read()");
		exit(0);
	}
	return(n);
}

void Write(int fileDescriptor, void *buffer, size_t numberOfBytes)
{
	if (write(fileDescriptor, buffer, numberOfBytes) != numberOfBytes)
	{
		perror("Error in Write()");
		exit(0);
	}
}

void Shutdown(int fileDescriptor, int shutdownOption)
{
	if (shutdown(fileDescriptor, shutdownOption) < 0)
	{
		perror("Error in Shutdown()");
		exit(0);
	}
}

int Max(int x, int y)
{
	return ( x < y ) ? y : x;
}

