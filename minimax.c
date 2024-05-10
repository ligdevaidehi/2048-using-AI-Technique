#include "minimax.h"
#include "game.h"
#include <stdio.h>
#include <limits.h>
#include <stdbool.h>

int moves[] = {'l', 'r', 'u', 'd'};

// Minimax implementation
int minimax(int depth, bool isMaximizingPlayer, int alpha, int beta)
{
    if (depth == 0 || isBoardFull())
    {
        return evaluateGameState();
    }

    if (isMaximizingPlayer)
    {
        int maxScore = INT_MIN;
        size_t num_moves = sizeof(moves) / sizeof(moves[0]);
        for (size_t i = 0; i < num_moves; i++)
        {
            char move = moves[i];
            // Simulate the move
            applyMove(move);

            int score = minimax(depth - 1, false, alpha, beta);
            maxScore = (score > maxScore) ? score : maxScore;

            // Undo the move
            applyMove(move); // Undo the move back to original state

            alpha = (alpha > maxScore) ? alpha : maxScore;
            if (beta <= alpha)
            {
                break; // Beta cut-off
            }
        }
        return maxScore;
    }
    else
    {
        int minScore = INT_MAX;
        size_t num_moves = sizeof(moves) / sizeof(moves[0]);
        for (size_t i = 0; i < num_moves; i++)
        {
            char move = moves[i];
            // Simulate the move
            applyMove(move);

            int score = minimax(depth - 1, true, alpha, beta);
            minScore = (score < minScore) ? score : minScore;

            // Undo the move
            applyMove(move); // Undo the move back to original state

            beta = (beta < minScore) ? beta : minScore;
            if (beta <= alpha)
            {
                break; // Alpha cut-off
            }
        }
        return minScore;
    }
}

char chooseBestMove()
{
    size_t num_moves = sizeof(moves) / sizeof(moves[0]);
    int bestScore = INT_MIN;
    char bestMove = 'l'; // Default move (left)
    int alpha = INT_MIN;
    int beta = INT_MAX;

    for (size_t i = 0; i < num_moves; i++)
    {
        char move = moves[i];

        // Simulate the move
        applyMove(move);

        int score = minimax(MAX_DEPTH, false, alpha, beta);
        if (score > bestScore)
        {
            bestScore = score;
            bestMove = move;
        }

        // Undo the move
        applyMove(move); // Undo the move back to original state
    }

    return bestMove;
}

int evaluateGameState()
{
    int totalScore = 0;
    for (int i = 0; i < SIDE; i++)
    {
        for (int j = 0; j < SIDE; j++)
        {
            totalScore += gameField[i][j];
        }
    }
    return totalScore;
}
