/* Hangclient.c - Client for hangman server.  */
#include "../includes/Definitions.h"
#include "../includes/Sockets.h"

int main(int argc, char * argv[]) {
	int sock, count;
	char i_line[LINESIZE];
	char o_line[LINESIZE];
	char * server_name;

	server_name = (argc == 1) ? argv[1] : "localhost";

	sock = Socket(AF_INET, SOCK_STREAM, 0);

	struct Address address;
	Address(AF_INET, (struct Address*) &address, server_name, HANGMAN_TCP_PORT);

	Connect(sock, (struct sockaddr *) &address.server, sizeof(address.server));

	/*OK connected to the server.
	 Take a line from the server and show it, take a line and send the user input to the server.
	 Repeat until the server terminates the connection. */

	StartGame(stdin, sock);

	printf("Connected to server %s \n", server_name);
	while ((count = read(sock, i_line, LINESIZE)) > 0) {
		write(1, i_line, count);
		printf("reading from socket...\n");
		count = read(0, o_line, LINESIZE); //0 = STDIN
		write(sock, o_line, count);
	}

	printf("Client: game over");
	return 0;
}
