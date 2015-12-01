/*
 * game.cpp
 *
 *  Created on: 4 Oct 2015
 *      Author: david, derek
 */

#include "../includes/Definitions.h"
#include "../includes/Game.h"
#include <syslog.h>
#include <unistd.h> // gethostname(), write()
#include <stdlib.h>
#include <string.h> // strlen()

char *word[] = {
# include "../words"
		};

extern time_t time();

void InitGameSessions() 
{
	int i;
	for (i = 0; i < MAX_GAME_SESSIONS; i++) 
	{
		strcpy(gameSessions[i].strUsername, "null");
		gameSessions[i].cGameState = 'U'; // unknown
		gameSessions[i].iLives = 0;
		gameSessions[i].iRandomWordLength = 0;
		gameSessions[i].iSequenceNumber = 0;
		gameSessions[i].strRandomWord = "null";
		gameSessions[i].iSessionId = 0;
	}
}

void initGameSessions(
		struct GameSession* gameSessions /* array of game sessions*/) 
{
	int i;
	for (i = 0; i < MAX_GAME_SESSIONS; i++) {
		strcpy(gameSessions[i].strUsername, "null");
		gameSessions[i].cGameState = 'U'; // unknown
		gameSessions[i].iLives = 0;
		gameSessions[i].iRandomWordLength = 0;
		gameSessions[i].iSequenceNumber = 0;
		gameSessions[i].strRandomWord = "null";
		gameSessions[i].iSessionId = 0;
	}
}

void PrintActiveGameSessions() {
	printf("active game sessions:\n");
	int i;
	for (i = 0; i < MAX_GAME_SESSIONS; i++) {
		if (strcmp(gameSessions[i].strUsername, "null") != 0) {
			printf("Index %d Username: %s\n", i, gameSessions[i].strUsername);
		}
	}
}

void PrintGameSession(struct GameSession *gameSession) {
	printf("SessionID: %d\n", gameSession->iSessionId);
	printf("Username: %s\n", gameSession->strUsername);
	printf("Lives: %d\n", gameSession->iLives);
	printf("Secret word: %s\n", gameSession->strRandomWord);
	printf("Progress: %s\n", gameSession->strPartWord);
}

void EndGameSession(struct GameSession *gameSession) {
	strcpy(gameSession->strUsername, "null");
	gameSession->cGameState = 'U';
	gameSession->iSequenceNumber = 0;
	gameSession->iSessionId = 0;
}

struct GameSession *FindGameSession(int fd) {
	printf("Searching for game session with %d...\n", fd);
	int i;
	for (i = 0; i < MAX_GAME_SESSIONS; i++) {
		if (gameSessions[i].iSessionId == fd) {
			printf("Game session found!\n");
			return &gameSessions[i];
		}
	}

	// there are no game session slots left
	return NULL;
}

int ProcessRequest(int out, struct GameSession* gameSession, char* message) {

	char outbuf[MAXLEN];

	int i;
	int good_guess;
	char hostname[MAXLEN];

	printf("Checking game session status...\n");
	// No more game slots available on the server
	if (gameSession == NULL) {
		sprintf(outbuf, "%s",
				"Connection failed no empty game slots on the server.");
		printf("Connection refused\n");
		write(out, outbuf, strlen(outbuf));

		return -1;
	}

	// If the game has started and is Incomplete
	if (gameSession->cGameState == 'I')
	/* Get a letter from player guess */
	{
		printf("Processing guess for client. Printing game session...\n");
		PrintGameSession(gameSession);

		// If the client guesses right update the part word in the
		// game session. If the client is resuming their game skip the guess
		if (message[0] != ' ') {
			good_guess = 0;
			for (i = 0; i < gameSession->iRandomWordLength; i++) {
				if (message[0] == gameSession->strRandomWord[i]) {
					good_guess = 1;
					gameSession->strPartWord[i] = gameSession->strRandomWord[i];
				}
			}

			// If the client guesses wrong decrement their life
			if (!good_guess)
				gameSession->iLives--;
		} else {
			gethostname(hostname, MAXLEN);
			printf("sending confirmation message to the client..\n");
			sprintf(outbuf, "Playing hangman on host %s with %s: \n", hostname,
					gameSession->strUsername);
			write(out, outbuf, strlen(outbuf));
		}

		// Check if the word has been guessed correctly
		if (strcmp(gameSession->strRandomWord, gameSession->strPartWord) == 0) {
			gameSession->cGameState = 'W';
			sprintf(outbuf,
					"Congratulations you won!\nSecret word: %s\nLives left: %d",
					gameSession->strPartWord, gameSession->iLives);
			printf("Client %s Won!\n", gameSession->strUsername);
			write(out, outbuf, strlen(outbuf));

			return -1;
		} else if (gameSession->iLives == 0) {
			gameSession->cGameState = 'L';
			sprintf(outbuf, "Sorry you lost\nSecret word: %s",
					gameSession->strRandomWord);
			printf("Client %s Lost!\nSending final message: %s\n",
					gameSession->strUsername, outbuf);
			write(out, outbuf, strlen(outbuf));

			return -1;
		}

		printf("sending game state to the client...\n");
		sprintf(outbuf, "%s %d \n", gameSession->strPartWord,
				gameSession->iLives);
		printf("%s\n", outbuf);
		write(out, outbuf, strlen(outbuf));

		return 0;
	}

	return -1;
}

struct GameSession *BuildGame(char* username, int fd) {

	int i, word_length, j;

	for (i = 0; i < MAX_GAME_SESSIONS; i++) {
		if (strcmp(gameSessions[i].strUsername, "null") == 0) {

			/* Pick a word at random from the list */
			printf("picking a random word...\n");
			srand((int) time((long *) 0)); /* randomize the seed */

			gameSessions[i].strRandomWord = word[rand() % NUM_OF_WORDS];
			word_length = strlen(gameSessions[i].strRandomWord);
			syslog(LOG_USER | LOG_INFO, "server chose hangman word %s",
					gameSessions[i].strRandomWord);

			/* No letters are guessed Initially */
			for (j = 0; j < word_length; j++)
				gameSessions[i].strPartWord[j] = '-';

			gameSessions[i].strPartWord[j] = '\0';


			// Store information in game session
			gameSessions[i].cGameState = 'I';
			gameSessions[i].iRandomWordLength = word_length;
			strcpy(gameSessions[i].strUsername, username);
			gameSessions[i].iLives = MAX_LIVES;
			gameSessions[i].iSessionId = fd;

			printf("------------NEW GAME---------\n");
			PrintGameSession(&gameSessions[i]);
			printf("-----------------------------\n");

			return &gameSessions[i];
		}

	}
	return NULL;
}

