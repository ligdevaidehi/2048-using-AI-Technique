#ifndef MINIMAX_H
#define MINIMAX_H

#include <stdbool.h>
extern int moves[]; // Array of possible moves
int minimax(int depth, bool isMaximizingPlayer, int alpha, int beta);
char chooseBestMove();
int evaluateGameState();

#endif
