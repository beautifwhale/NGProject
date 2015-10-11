/* Network server_addr for hangman game */
/* File: hangserver_addr.c */

/*
 * 	Select working as multiply clients connecting to server but issue with client closing and releasing slot
 *
 * 	Need to add way of holding state so client is remembered and games can be played at same time on multiply clients
 *
 *
 */

#include "../includes/Sockets.h"
#include "../includes/Game.h"

extern time_t time();

int main(int argc, char* argv[]) {

	fd_set master, read_fds;
	struct Address sClientAddress;
	int fdmax, listener,newfd, nbytes, addrlen, i ,j, sockfd;
	int clients[FD_SETSIZE], maxindex, nready;
	char buf[MAXLEN];
	char * strServerIPAddress;
	strServerIPAddress = "0.0.0.0";

	/* get the listener */

	listener = Socket(AF_INET, SOCK_STREAM, 0);
	printf("Socket() is OK...\n");

	/* Address */
	Address(AF_INET, (struct Address*) &sClientAddress, strServerIPAddress, HANGMAN_TCP_PORT);
	printf("Address() is OK...\n");

	/* bind */
	Bind(listener, (struct sockaddr *) &sClientAddress.m_sAddress, sizeof(sClientAddress.m_sAddress));
	printf("Bind() is OK...\n");

	/* listen */
	Listen(listener, LISTENQ);
	printf("Listen() is OK...\n");

	/*set max*/
	fdmax = listener;
	maxindex = -1;
	for(i = 0; i < FD_SETSIZE; i++){
		clients[i] = -1;
	}


	/* clear the master and temp sets */
	FD_ZERO(&master);
	FD_ZERO(&read_fds);
	FD_SET(listener, &master);

	for (;;){

		read_fds = master;

		newfd = Select(fdmax + 1, &read_fds, NULL, NULL, NULL);
		printf("Select() is OK...\n");


		/*run through the existing connections looking for data to be read*/
		if (FD_ISSET(listener, &read_fds)){ /* we got one... */
			/* handle new connections */
			addrlen = sizeof(sClientAddress.m_sAddress);
			newfd = accept(listener, (struct sockaddr *)&sClientAddress.m_sAddress, &addrlen);
			printf("Accept() is OK...\n");

			for(i = 0; i < FD_SETSIZE; i++){
				if(clients[i] < 0){
					clients[i] = newfd;
					printf("Descriptor Saved :)\n");
					break;
				}
			}
			FD_SET(newfd, &master); 	/* add to master set */
			if (newfd > fdmax){ 		/* keep track of the maximum */
				fdmax = newfd;
				printf("New connection from %s on socket %d\n", inet_ntoa(sClientAddress.m_sAddress.sin_addr), newfd);
			}
		}

		/*Check all Clients for data*/
		for (i = 0; i <= fdmax; i++){

			if((sockfd = clients[i]) < 0)
				continue;

			if(FD_ISSET(sockfd, &read_fds)){
				if ((nbytes = recv(sockfd, buf, sizeof(buf), 0)) == 0){
						perror("recv() error !");
						close(sockfd);
						FD_CLR(sockfd, &master);
						clients[i] = -1;
				}
			}

			/* handle data from a client */
			else{

				play_hangman( newfd, newfd);

			}
		}
	}
	return 0;
}

