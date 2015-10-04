/*
 * Definitions.h
 *
 *  Created on: 4 Oct 2015
 *      Author: david
 */

#ifndef INCLUDES_SOCKETS_H_
#define INCLUDES_SOCKETS_H_


#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

struct Address{
	struct sockaddr_in server; /* Server's address assembled here */
	struct hostent * host_info;
};

int Socket(int family, int type, int protocol);
void Address(int family, struct Address* address, char* ipAddress, int portNumber);
void Connect(int socketFileDescriptor, const struct sockaddr* socketAddress, socklen_t socketSize);
int Select(int maxFileDescriptorsPlus1, fd_set *readFileDescriptorSet, fd_set *writeFileDescriptorSet, fd_set *exceptFileDescriptorSet, struct timeval *timeout);
ssize_t Read(int fileDescriptor, void *buffer, size_t numberOfBytes);
void Write(int fileDescriptor, void *buffer, size_t numberOfBytes);
void Shutdown(int fileDescriptor, int shutdownOption);
int Max(int x, int y);

#endif /* INCLUDES_SOCKETS_H_ */
