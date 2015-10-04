/*
 * Game.c
 *
 *  Created on: 4 Oct 2015
 *      Author: david
 */

#include <stdio.h>
#include "../includes/Sockets.h"
#include "../includes/Definitions.h"
#include "../includes/Game.h"

void StartGame(FILE* fp, int socketFileDescriptor)
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

		// call the select function to check each file descriptor
		Select(maxFileDescriptorsPlus1, &readFileDescriptorSet, NULL, NULL, NULL);

		// socket file descriptor is active
		if( FD_ISSET(socketFileDescriptor, &readFileDescriptorSet) )
		{
			numberOfBytesReceived = Read(socketFileDescriptor, buffer, MAX_BUF_SIZE);
			if( numberOfBytesReceived == 0 )
			{
				// connection to the server was terminated normally
				if( stdinEOF == 1)
				{
					return;
				}
				else
				{
					perror("Server termination:");
					exit(1);
				}
			}

			// write the results of reading the socket
			Write(fileno(stdout), buffer, numberOfBytesReceived);
		}

		// input file descriptor is active
		if( FD_ISSET(fileno(fp), &readFileDescriptorSet) )
		{
			numberOfBytesReceived = Read(fileno(fp), buffer, MAX_BUF_SIZE);
			if( numberOfBytesReceived == 0 )
			{
				stdinEOF = -1;
				Shutdown(socketFileDescriptor, SHUT_WR);
				FD_CLR(fileno(fp), &readFileDescriptorSet);
				continue;
			}

			Write(socketFileDescriptor, buffer, numberOfBytesReceived);
		}
	}
}
