#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <limits.h>

#define SIDE 4
#define MAX_DEPTH 3

extern int score;
extern int topScore;
extern int userScore;
extern int aiScore;
extern int userTopScore;
extern int aiTopScore;
extern int currentPlayer;
extern int gameField[SIDE][SIDE];
extern int moves[]; // Array of possible moves

bool isBoardFull();
void createGame();
void drawScene();
void createNewNumber();
bool canUserMove();
void gameOver();
void applyMove(char move);
void moveLeft();
void moveRight();
void moveUp();
void moveDown();
int getMaxTileValue();
void win();
void saveTopScore();
void loadTopScore();
void play();

#endif
