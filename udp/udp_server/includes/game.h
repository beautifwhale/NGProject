#ifndef INCLUDES_GAME_H_
#define INCLUDES_GAME_H_

#include "../../../libsocket/socket.h"

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

#define MAX_GAME_SESSIONS 50

void initGameSessions(struct GameSession* gameSessions /* array of game sessions*/);
void PrintActiveGameSessions(struct GameSession* gameSessions);
void PrintGameSession(struct GameSession *gameSession);
int FindGameSession(struct GameSession* gameSessions, int len, char* username);
void play_hangman(int in, int out, struct Address client, struct GameSession* gameSession);
int ProcessRequest(int in, int out, struct Address client, struct GameSession* gameSession, char* message);
int findGameSession(struct GameSession* gameSessions, int len, char* username);
void EndGameSession(struct GameSession *gameSession);


#endif /* INCLUDES_GAME_H_ */
