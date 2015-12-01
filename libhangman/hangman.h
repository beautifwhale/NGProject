
#ifndef INCLUDES_GAME_H_
#define INCLUDES_GAME_H_

#define MAX_LIVES 12
#define NUM_OF_WORDS (sizeof (word) / sizeof (word [0]))
#define MAXLEN 80 /* Maximum size in the world of Any string */
#define MAX_GAME_SESSIONS 10 // Number of game slots available on the server

#include "../libsocket/socket.h"

struct GameSession{

	// Status of the game session. If this game session has ended
	// it is removed from server memory
	char cGameState;

	// Unique identifier for game session associated with a
	// client
	char strUsername[255];

	// Sequence number used to keep track of progress
	// of game session with client
	int iSequenceNumber;

	// Random word chosen by the server for this game session
	char* strRandomWord;

	// Length of the random word used to check guess with each char
	// in the random word char[]
	int iRandomWordLength;

	// The clients word status so far. As the client guesses correctly the
	// part word is filled updated.
	char strPartWord[MAXLEN];

	// Number of lives left
	int iLives;

	// Id given to game session. This is also the index in the game sessions array.
	int iSessionId;
};

struct GameSession gameSessions[MAX_GAME_SESSIONS];

void InitGameSessions();
void PrintActiveGameSessions();
void PrintGameSession(struct GameSession *gameSession);
struct GameSession *FindGameSession(char* username);
int ProcessRequest(int clientFileDescriptor, struct Address client, struct GameSession* gameSession, char* message);
void EndGameSession(struct GameSession *gameSession);

// Libsocket wrapper functions to abstract the library and reduce coupling
// Wrapper for libsocket Connection() that creates a peer connection based on the
int InitConnection(char *hostname, char *service, int type /* Client or Server */, int protocol /* UDP or TCP */);

// Receive messages from the server using libsocket TODO: create wrapper in libsocket for revfrom()
int SendMessage(int socketFileDescriptor, char* buffer, size_t size, int flags);

// Receive data from the connected server using recvfrom()
int ReceiveMessage(int iListenSocketFileDescriptor, char* buffer, int bufferSize, int flags, struct sockaddr *sender, socklen_t *sendsize);

// PlayHangmanClientTCP is a wrapper for the libsocket function MultiplexIO()
// Parameters are stdin and the network socket file descriptor
void PlayHangmanClientTCP(FILE *filePointer, int socketFileDescriptor);

// PlayHangmanServerTCP is used on the hangman server to
// process TCP connections with clients
void PlayHangmanServerTCP(int in, int out);

#endif /* INCLUDES_GAME_H_ */
