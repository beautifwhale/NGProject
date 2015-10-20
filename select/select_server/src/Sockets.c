/*
 * Sockets.c
 *
 *  Created on: 6 Oct 2015
 *      Author: derekobrien
 */

#include "../includes/Sockets.h"

int Socket(int family, int type, int protocol){
	int sock = socket(family, type, protocol);
	if (sock < 0){
		perror("Creating stream socket");
		exit(1);
	}
	return sock;
}

void Address(int family, struct Address* address, char* ipAddress, int portNumber){
	// create the server address
	address->m_sHost_info = gethostbyname(ipAddress);
	if (address->m_sHost_info == NULL)
	{
		fprintf(stderr, "unknown host:%s \n", ipAddress);
		exit(2);
	}

	//
	address->m_sAddress.sin_family = address->m_sHost_info->h_addrtype; // set protocol family

	// address struct, network address from host_info, size of host_info
	memcpy((char *) &address->m_sAddress.sin_addr, address->m_sHost_info->h_addr, address->m_sHost_info->h_length);

	address->m_sAddress.sin_port = htons(portNumber); // set server port number
}

void Bind(int filedescriptor, struct Address* address, socklen_t socketSize){


	if (bind(filedescriptor, (struct Address *)&address->m_sAddress, socketSize) < 0) {
			perror("binding socket");
			exit(2);
		}

}

void Listen(int filedescriptor, int n){
	listen(filedescriptor, n);
}

int Accept(int filedescriptor, struct Address* address, socklen_t socketSize){
	int n;
	if ((n = accept(filedescriptor, (struct Address *) &address->m_sAddress, socketSize)) < 0) {
				perror("accepting connection");
				exit(3);
			}
	return n;
}


int Select(int maxFileDescriptorsPlus1, fd_set *readFileDescriptorSet, fd_set *writeFileDescriptorSet, fd_set *exceptFileDescriptorSet, struct timeval *timeout){
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

void Writen( int fileDescriptor, void *buffer, size_t numberOfBytes){
	if (write(fileDescriptor, buffer, numberOfBytes) != numberOfBytes)
		{
			perror("Error in Write()");
			exit(0);
		}
}

