/*
 * Game.h
 *
 *  Created on: 4 Oct 2015
 *      Author: david , Derek O Brien
 */

#ifndef GAME_H_
#define GAME_H_

#include "Definitions.h"

#define MAX_GAME_SESSIONS 20

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

	// Id given to game session.
	int iSessionId;
};


struct GameSession gameSessions[MAX_GAME_SESSIONS];

void InitGameSessions();
void initGameSessions(struct GameSession *gameSessions /* array of game sessions*/);
void PrintActiveGameSessions();
void PrintGameSession(struct GameSession *gameSession);
void EndGameSession(struct GameSession *gameSession);

int ProcessRequest(int out, struct GameSession* gameSession, char* message);
struct GameSession *FindGameSession(int fd);
struct GameSession *BuildGame(char* username, int fd);

#endif /* INCLUDES_GAME_H_ */
