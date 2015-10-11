/*
 * Sockets.h
 *
 *  Created on: 6 Oct 2015
 *      Author: derekobrien
 */

#ifndef SOCKETS_H_
#define SOCKETS_H_

#include "Definitions.h"

struct Address{
	struct sockaddr_in m_sAddress; /* Server's address assembled here */
	struct hostent * m_sHost_info;
};

int Socket(int family, int type, int protocol);
void Address(int family, struct Address* address, char* ipAddress, int portNumber);
int Connect(int socketFileDescriptor, struct Address* address, socklen_t socketSize);
void Bind(int filedescriptor, struct Address* address, socklen_t socketSize);
int Accept(int filedescriptor, struct Address* address, socklen_t socketSize);
void Listen(int filedescriptor, int n);

int Select(int maxFileDescriptorsPlus1, fd_set *readFileDescriptorSet, fd_set *writeFileDescriptorSet, fd_set *exceptFileDescriptorSet, struct timeval *timeout);
ssize_t Read(int fileDescriptor, void *buffer, size_t numberOfBytes);
void Write( int fileDescriptor, void *buffer, size_t numberOfBytes);

#endif /* SOCKETS_H_ */
