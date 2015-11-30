// fork_server.c
// 
// Year 4 Networked Games Assignment 2015
// Team:	David Morton
//			Kevin Byrne
// 			add names here...
//
// 
//
// Description: The server in the Hangman game will create a socket, bind to a port, and will then 
// listen for incoming connections from clients. A signal handler is created to process signals from 
// child processes of the server in order to kill and release resources used by them on the system. 
// The SignalHandler() function implementation can be found in the socket.c file in the libsocket library.
// For each new TCP connection from a client the server will create a new process using the fork() function. 
// The listening socket inside the new process is then closed and control is passed to the play_hangman() function
// described in the game.c file. Time permitting the game.c implementations will be archived to create
// a library for the hangman game. Once the game has ended the play_hangman() function returns and the process
// will exit with code 0 meaning 'Success'
//
#include "../../../libsocket/socket.h"
#include "../includes/definitions.h"
#include "../includes/game.h"

int main(int argc, char* argv[]) {
	int iListenSocketFileDescriptor;
	char *strServerIPAddress;
	struct Address sAddress;
	pid_t childProcessID;
	int connfd;
	socklen_t client_len;

	strServerIPAddress = "0.0.0.0";

	printf("Server: initialising\n");

	// Old version using gethostbyname()
	//iListenSocketFileDescriptor = Socket(AF_INET, SOCK_STREAM, 0);
	// TODO Create wrapper for Address() that will allow IPv4 and IPv6 connections.
	// Use command line input to pass in the hostname and service port number.
	// AddressIPX("www.google.com", "1071", struct addrinfo *hints, struct addrinfo** result);
	//Address(AF_INET, (struct Address*) &sAddress, strServerIPAddress, HANGMAN_TCP_PORT);
	//Bind(iListenSocketFileDescriptor, (struct sockaddr *) &sAddress.m_sAddress, sizeof(sAddress.m_sAddress));

	// Create a connection for the server
	iListenSocketFileDescriptor = Connection(NULL, "1071", TYPE_SERVER, SOCK_STREAM);

	// Listen for incoming TCP connections
	Listen(iListenSocketFileDescriptor, MAX_LISTEN_QUEUE_SIZE);

	// Signal handler for terminated processes
	Signal(SIGCHLD, SignalHandler);

	printf("listening for connections\n");
	for( ; ; ) {
		client_len = sizeof(sAddress.m_sAddress);
		// Accept connections from clients
		if ((connfd = accept(iListenSocketFileDescriptor, (struct sockaddr *) &sAddress.m_sAddress, &client_len)) < 0)
		{
			// There was an error (interrupt)
			if( errno == EINTR )
			{
				// Try another Accept() in the event of a system interrupt
				continue;
			}
			else
			{
				// There was an error other than an interrupt so close the Parent process
				perror("Accept error");
				exit(3);
			}
		}

		// There was no error in Accept()! Woo! Create a child process
		if( (childProcessID = fork()) == 0)
		{
			// CHILD
			//printf("child %d created\n", childProcessID);

			// close the parents listen file descriptor in the child
			close(iListenSocketFileDescriptor);

			/* ---------------- Play_hangman () ---------------------*/
			play_hangman(connfd, connfd);

			/*
			 *  On return exit to kill the process. The kernel will then
			 *  send a signal to the parent which is caught by the parents
			 *  SignalHandler() set in Signal()
			 */
			exit(0);
		}

		close(connfd);
	}
	return 0;
}



