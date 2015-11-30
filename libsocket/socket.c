#include "socket.h"
#include <string.h> // memcpy()
#include <signal.h> // SIGCHLD
#include <sys/wait.h>

int Socket(int family, int type, int protocol)
{
	int sock = socket(family, type, protocol);
	if (sock < 0)
	{
		perror("Error in Socket()");
		exit(1); // Exit failure
	}
	return sock;
}

void Address(int family, struct Address* address, char* ipAddress, int portNumber)
{
	//printf("Address being created\n");
	// create the server address
	address->m_sHost_info = gethostbyname(ipAddress);
	if (address->m_sHost_info == NULL)
	{
		fprintf(stderr, "unknown host:%s \n", ipAddress);
		exit(1); // Exit failure
	}

	address->m_sAddress.sin_family = address->m_sHost_info->h_addrtype; // set protocol family

	// address struct, network address from host_info, size of host_info
	memcpy((char *) &address->m_sAddress.sin_addr, address->m_sHost_info->h_addr, address->m_sHost_info->h_length);

	address->m_sAddress.sin_port = htons(portNumber); // set server port number
}

int Connection(char *address, char *service, int type /* Client or Server */, struct addrinfo *connectionInfo)
{
	int sockfd;
	struct addrinfo hints, *result, *p;
	int rv;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; // use AF_INET6 to force IPv6
	hints.ai_socktype = SOCK_DGRAM;

	if(type == TYPE_CLIENT)
	{
		if ((rv = getaddrinfo(address, service, &hints, &result)) != 0) {
			fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
			exit(1);
		}
	}else if(type == TYPE_SERVER)
	{
		hints.ai_flags = AI_PASSIVE; // use my IP address

		if ((rv = getaddrinfo(NULL, service, &hints, &result)) != 0) {
		    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		    exit(1);
		}
	}else
	{
		return -1;
	}

	// loop through all the results and connect to the first we can
	for(p = result; p != NULL; p = p->ai_next) {
	    if ((sockfd = socket(p->ai_family, p->ai_socktype,
	            p->ai_protocol)) == -1) {
	        perror("socket");
	        continue;
	    }

	    if(type == TYPE_CLIENT)
	    {
			if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
				close(sockfd);
				perror("connect");
				continue;
			}
	    }
	    else if(type == TYPE_SERVER)
	    {
	    	if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
				close(sockfd);
				perror("bind");
				continue;
			}
	    }
	    else
	    {
	    	// type unspecified
	    	return -1;
	    }

	    break; // if we get here, we must have connected successfully
	}

	if (p == NULL) {
	    // looped off the end of the list with no connection
	    fprintf(stderr, "failed to connect\n");
	    exit(2);
	}

	// Connection successful
	return sockfd;
}

void Connect(int socketFileDescriptor, const struct sockaddr* socketAddress, socklen_t socketSize)
{
	if (connect(socketFileDescriptor, socketAddress, socketSize) < 0)
	{
		perror("Error in Connect()");
		exit(1); // Exit failure
	}
}

int Select(int maxFileDescriptorsPlus1, fd_set *readFileDescriptorSet, fd_set *writeFileDescriptorSet, fd_set *exceptFileDescriptorSet, struct timeval *timeout)
{
	int n;
	if ( (n = select(maxFileDescriptorsPlus1, readFileDescriptorSet, writeFileDescriptorSet, exceptFileDescriptorSet, timeout)) < 0)
	{
		perror("Error in Select()");
		exit(1); // Exit failure
	}
	return(n);		/* can return 0 on timeout */
}

ssize_t Read(int fileDescriptor, void *buffer, size_t numberOfBytes)
{
	ssize_t n;
	if ( (n = read(fileDescriptor, buffer, numberOfBytes)) == -1)
	{
		perror("Error in Read()");
		exit(1); // Exit failure
	}
	return(n);
}

void Write(int fileDescriptor, void *buffer, size_t numberOfBytes)
{
	if (write(fileDescriptor, buffer, numberOfBytes) != numberOfBytes)
	{
		perror("Error in Write()");
		exit(1); // Exit failure
	}
}

void Shutdown(int fileDescriptor, int shutdownOption)
{
	if (shutdown(fileDescriptor, shutdownOption) < 0)
	{
		perror("Error in Shutdown()");
		exit(1); // Exit failure
	}
}

int Max(int x, int y)
{
	return ( x < y ) ? y : x;
}

void Signal(int signalNumber, void* SignalHandler)
{
	if(signal(SIGCHLD, SignalHandler) == SIG_ERR)
	{
		perror("Error in Signal()");
		exit(1); // Exit failure
	}
}

void SignalHandler(int signalNumber)
{
	pid_t processID;
	int stat;

	while( (processID = waitpid(WAIT_ANY, &stat, WNOHANG)) > 0)
	{
		printf("child terminated\n");
	}
	return;
}

void Bind(int socketFileDescriptor, const struct sockaddr* socketAddress, socklen_t socketSize)
{
	if (bind(socketFileDescriptor, socketAddress, socketSize) < 0) {
		perror("Error in Bind()");
		exit(1); // Exit failure
	}
}

void Listen(int socketFileDescriptor, int maxListenQSize)
{
	if(listen(socketFileDescriptor, maxListenQSize) < 0)
	{
		perror("Error in Listen()");
		exit(1); // Exit failure
	}
}

void MultiplexStdinFileDescriptor(FILE* fp, int socketFileDescriptor)
{
	int maxFileDescriptorsPlus1;
	int stdinEOF = 0;
	fd_set readFileDescriptorSet;
	char buffer[MAX_BUF_SIZE];
	int numberOfBytesReceived;

	// clear the read set bits
	FD_ZERO(&readFileDescriptorSet);

	for( ; ; )
	{
		if( stdinEOF == 0)
		{
			// get the integer value for the stdin file descriptor and set this is the read set
			FD_SET(fileno(fp), &readFileDescriptorSet);
		}

		// set the socket file descriptor in the read set
		FD_SET(socketFileDescriptor, &readFileDescriptorSet);

		// find the highest index for the readset
		maxFileDescriptorsPlus1 = Max(fileno(fp), socketFileDescriptor) + 1;

		// call the select function to check each file descriptor for activity
		Select(maxFileDescriptorsPlus1, &readFileDescriptorSet, NULL, NULL, NULL);

		// socket file descriptor is active
		if( FD_ISSET(socketFileDescriptor, &readFileDescriptorSet) )
		{
			numberOfBytesReceived = Read(socketFileDescriptor, buffer, MAX_BUF_SIZE);
			if( numberOfBytesReceived == 0 )
			{
				if( stdinEOF == 1 )
				{
					// Client has shutdown the connection
					return;
				}
				else
				{
					// server has terminated the connection
					perror("Server terminated");
					exit(0);
				}
			}

			// write the results of reading the socket
			Write(fileno(stdout), buffer, numberOfBytesReceived);
		}

		// input file descriptor is active
		if( FD_ISSET(fileno(fp), &readFileDescriptorSet) )
		{
			numberOfBytesReceived = Read(fileno(fp), buffer, MAX_BUF_SIZE);

			// if the client is terminated the socket is shutdown
			if( numberOfBytesReceived == 0 )
			{
				//printf("Client has terminated the connection");
				stdinEOF = 1;
				Shutdown(socketFileDescriptor, SHUT_WR);
				FD_CLR(fileno(fp), &readFileDescriptorSet);
				continue;
			}

			Write(socketFileDescriptor, buffer, numberOfBytesReceived);
		}
	}
}
