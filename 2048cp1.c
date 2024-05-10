#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <time.h>
#include <windows.h>

#define SIDE 4
#define MAX_DEPTH 3

int score = 0;
int topScore = 0;
int userScore = 0;
int aiScore = 0;
int userTopScore = 0;
int aiTopScore = 0;
int currentPlayer = 1;
int gameField[SIDE][SIDE];
const char moves[] = {'l', 'r', 'u', 'd'}; // Array of possible moves

// Function prototypes
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

// Helper function to check if the game board is full
bool isBoardFull()
{
    for (int i = 0; i < SIDE; i++)
    {
        for (int j = 0; j < SIDE; j++)
        {
            if (gameField[i][j] == 0)
            {
                return false;
            }
        }
    }
    return true;
}

// Helper function to evaluate the game state
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

// Minimax algorithm with alpha-beta pruning
int minimax(int depth, bool isMaximizingPlayer, int alpha, int beta);

// Function to choose the best move using Minimax with alpha-beta pruning
char chooseBestMove()
{
    int bestScore = INT_MIN;
    char bestMove = 'l'; // Default move (left)
    int alpha = INT_MIN;
    int beta = INT_MAX;

    for (int i = 0; i < sizeof(moves) / sizeof(moves[0]); i++)
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
        for (int i = 0; i < sizeof(moves) / sizeof(moves[0]); i++)
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
        for (int i = 0; i < sizeof(moves) / sizeof(moves[0]); i++)
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

// Implementations game-related functions
void createGame()
{
    userScore = 0;
    aiScore = 0;
    for (int i = 0; i < SIDE; i++)
    {
        for (int j = 0; j < SIDE; j++)
        {
            gameField[i][j] = 0;
        }
    }
    createNewNumber();
    createNewNumber();
}

void createNewNumber()
{
    if (isBoardFull())
    {
        gameOver();
        return;
    }

    if (getMaxTileValue() >= 2048)
    {
        win();
        return;
    }
    bool isCreated = false;
    do
    {
        int x = rand() % SIDE;
        int y = rand() % SIDE;
        if (gameField[y][x] == 0)
        {
            gameField[y][x] = (rand() % 10 < 9) ? 2 : 4;
            isCreated = true;
        }
    } while (!isCreated);
}

int getMaxTileValue()
{
    int max = gameField[0][0];
    for (int i = 0; i < SIDE; i++)
    {
        for (int j = 0; j < SIDE; j++)
        {
            if (gameField[i][j] > max)
            {
                max = gameField[i][j];
            }
        }
    }
    return max;
}

void win()
{
    system("cls");
    printf("********************************************\n");
    printf("************ Congratulations! **************\n");
    printf("************     YOU WIN!     **************\n");
    printf("********************************************\n");
    printf("Score: %d\n", currentPlayer == 1 ? userScore : aiScore);

    // Update top score
    if (currentPlayer == 1)
    {
        if (userScore > userTopScore)
        {
            userTopScore = userScore;
            printf("New Top Score! Your highest score: %d\n", userTopScore);
            // Save top score to file
            saveTopScore(currentPlayer);
        }
        else
        {
            printf("Your highest score: %d\n", userTopScore);
        }
    }
    else if (currentPlayer == 2)
    {
        if (aiScore > aiTopScore)
        {
            aiTopScore = aiScore;
            printf("New Top Score! AI's highest score: %d\n", aiTopScore);
            // Save top score to file
            saveTopScore(currentPlayer);
        }
        else
        {
            printf("AI's highest score: %d\n", aiTopScore);
        }
    }

    printf("********************************************\n");
    printf("Would you like to play again? (y/n): ");
    char choice;
    scanf(" %c", &choice);

    if (choice == 'y' || choice == 'Y')
    {
        play();
        // createGame();
        // drawScene();
    }
    else
    {
        exit(0);
    }
}

void drawScene()
{
    system("cls");
    printf("********************************************\n");
    printf("***************** 2048 Game ****************\n");
    printf("********************************************\n");
    // Determine which player is currently playing
    int currentScore, currentTopScore;
    if (currentPlayer == 1)
    {
        currentScore = userScore;
        loadTopScore(1); // Load user's top score
        currentTopScore = userTopScore;
    }
    else if (currentPlayer == 2)
    {
        currentScore = aiScore;
        loadTopScore(2); // Load AI's top score
        currentTopScore = aiTopScore;
    }

    printf("Score: %d\n", currentScore);
    printf("Top Score: %d\n", currentTopScore);

    for (int i = 0; i < SIDE; i++)
    {
        printf("+------+------+------+------+\n");
        for (int j = 0; j < SIDE; j++)
        {
            if (gameField[i][j] == 0)
            {
                printf("|      ");
            }
            else
            {
                printf("|%4d  ", gameField[i][j]);
            }
        }
        printf("|\n");
    }
    printf("+------+------+------+------+\n");
}

void gameOver()
{
    system("cls");
    printf("********************************************\n");
    printf("***************** Game Over ****************\n");
    printf("********************************************\n");
    printf("Score: %d\n", currentPlayer == 1 ? userScore : aiScore);

    // Update top score
    if (currentPlayer == 1)
    {
        if (userScore > userTopScore)
        {
            userTopScore = userScore;
            printf("New Top Score! Your highest score: %d\n", userTopScore);
            // Save top score to file
            saveTopScore(currentPlayer);
        }
        else
        {
            printf("Your highest score: %d\n", userTopScore);
        }
    }
    else if (currentPlayer == 2)
    {
        if (aiScore > aiTopScore)
        {
            aiTopScore = aiScore;
            printf("New Top Score! AI's highest score: %d\n", aiTopScore);
            // Save top score to file
            saveTopScore(currentPlayer);
        }
        else
        {
            printf("AI's highest score: %d\n", aiTopScore);
        }
    }

    printf("********************************************\n");
    printf("Would you like to play again? (y/n): ");
    char choice;
    scanf(" %c", &choice);
    if (choice == 'y' || choice == 'Y')
    {
        play();
        // createGame();
        // drawScene();
    }
    else
    {
        exit(0);
    }
}

bool canUserMove()
{
    // Check if there are empty cells or if adjacent cells have the same value
    for (int i = 0; i < SIDE; i++)
    {
        for (int j = 0; j < SIDE; j++)
        {
            if (gameField[i][j] == 0)
            {
                return true;
            }
            else if (i < SIDE - 1 && gameField[i][j] == gameField[i + 1][j])
            {
                return true;
            }
            else if (j < SIDE - 1 && gameField[i][j] == gameField[i][j + 1])
            {
                return true;
            }
        }
    }
    return false;
}

void applyMove(char move)
{
    switch (move)
    {
    case 'l':
        moveLeft();
        break;
    case 'r':
        moveRight();
        break;
    case 'u':
        moveUp();
        break;
    case 'd':
        moveDown();
        break;
    }
}

void moveLeft()
{
    // Implement moveLeft logic
    // Update the score based on who's playing
    score = 0;
    int *currentScore = (currentPlayer == 1) ? &userScore : &aiScore;

    for (int i = 0; i < SIDE; i++)
    {
        int insertPosition = 0;
        for (int j = 0; j < SIDE; j++)
        {
            if (gameField[i][j] != 0)
            {
                if (j != insertPosition)
                {
                    gameField[i][insertPosition] = gameField[i][j];
                    gameField[i][j] = 0;
                }
                insertPosition++;
            }
        }

        for (int j = 0; j < SIDE - 1; j++)
        {
            if (gameField[i][j] == gameField[i][j + 1] && gameField[i][j] != 0)
            {
                gameField[i][j] *= 2;
                *currentScore += gameField[i][j];
                gameField[i][j + 1] = 0;
            }
        }

        insertPosition = 0;
        for (int j = 0; j < SIDE; j++)
        {
            if (gameField[i][j] != 0)
            {
                if (j != insertPosition)
                {
                    gameField[i][insertPosition] = gameField[i][j];
                    gameField[i][j] = 0;
                }
                insertPosition++;
            }
        }
    }
}

void moveRight()
{
    // Implement moveRight logic
    // Update the score based on who's playing
    score = 0;
    int *currentScore = (currentPlayer == 1) ? &userScore : &aiScore;

    for (int i = 0; i < SIDE; i++)
    {
        int insertPosition = SIDE - 1;
        for (int j = SIDE - 1; j >= 0; j--)
        {
            if (gameField[i][j] != 0)
            {
                if (j != insertPosition)
                {
                    gameField[i][insertPosition] = gameField[i][j];
                    gameField[i][j] = 0;
                }
                insertPosition--;
            }
        }

        for (int j = SIDE - 1; j > 0; j--)
        {
            if (gameField[i][j] == gameField[i][j - 1] && gameField[i][j] != 0)
            {
                gameField[i][j] *= 2;
                *currentScore += gameField[i][j];
                gameField[i][j - 1] = 0;
            }
        }

        insertPosition = SIDE - 1;
        for (int j = SIDE - 1; j >= 0; j--)
        {
            if (gameField[i][j] != 0)
            {
                if (j != insertPosition)
                {
                    gameField[i][insertPosition] = gameField[i][j];
                    gameField[i][j] = 0;
                }
                insertPosition--;
            }
        }
    }
}

void moveUp()
{
    // Implement moveUp logic
    // Update the score based on who's playing
    score = 0;
    int *currentScore = (currentPlayer == 1) ? &userScore : &aiScore;

    for (int j = 0; j < SIDE; j++)
    {
        int insertPosition = 0;
        for (int i = 0; i < SIDE; i++)
        {
            if (gameField[i][j] != 0)
            {
                if (i != insertPosition)
                {
                    gameField[insertPosition][j] = gameField[i][j];
                    gameField[i][j] = 0;
                }
                insertPosition++;
            }
        }

        for (int i = 0; i < SIDE - 1; i++)
        {
            if (gameField[i][j] == gameField[i + 1][j] && gameField[i][j] != 0)
            {
                gameField[i][j] *= 2;
                *currentScore += gameField[i][j];
                gameField[i + 1][j] = 0;
            }
        }

        insertPosition = 0;
        for (int i = 0; i < SIDE; i++)
        {
            if (gameField[i][j] != 0)
            {
                if (i != insertPosition)
                {
                    gameField[insertPosition][j] = gameField[i][j];
                    gameField[i][j] = 0;
                }
                insertPosition++;
            }
        }
    }
}

void moveDown()
{
    // Implement moveDown logic
    // Update the score based on who's playing
    score = 0;
    int *currentScore = (currentPlayer == 1) ? &userScore : &aiScore;

    for (int j = 0; j < SIDE; j++)
    {
        int insertPosition = SIDE - 1;
        for (int i = SIDE - 1; i >= 0; i--)
        {
            if (gameField[i][j] != 0)
            {
                if (i != insertPosition)
                {
                    gameField[insertPosition][j] = gameField[i][j];
                    gameField[i][j] = 0;
                }
                insertPosition--;
            }
        }

        for (int i = SIDE - 1; i > 0; i--)
        {
            if (gameField[i][j] == gameField[i - 1][j] && gameField[i][j] != 0)
            {
                gameField[i][j] *= 2;
                *currentScore += gameField[i][j];
                gameField[i - 1][j] = 0;
            }
        }

        insertPosition = SIDE - 1;
        for (int i = SIDE - 1; i >= 0; i--)
        {
            if (gameField[i][j] != 0)
            {
                if (i != insertPosition)
                {
                    gameField[insertPosition][j] = gameField[i][j];
                    gameField[i][j] = 0;
                }
                insertPosition--;
            }
        }
    }
}

void saveTopScore(int currentPlayer)
{
    char filename[20];
    int topScore;

    if (currentPlayer == 1) // User's top score
    {
        strcpy(filename, "user_topscore.txt");
        topScore = userTopScore;
    }
    else if (currentPlayer == 2) // AI's top score
    {
        strcpy(filename, "ai_topscore.txt");
        topScore = aiTopScore;
    }

    FILE *file = fopen(filename, "w");
    if (file != NULL)
    {
        fprintf(file, "%d", topScore);
        fclose(file);
    }
}

void loadTopScore(int currentPlayer)
{
    char filename[20];
    int *topScore;

    if (currentPlayer == 1) // User's top score
    {
        strcpy(filename, "user_topscore.txt");
        topScore = &userTopScore;
    }
    else if (currentPlayer == 2) // AI's top score
    {
        strcpy(filename, "ai_topscore.txt");
        topScore = &aiTopScore;
    }

    FILE *file = fopen(filename, "r");
    if (file != NULL)
    {
        fscanf(file, "%d", topScore);
        fclose(file);
    }
}

void play()
{

    system("cls");
    printf("********************************************\n");
    printf("***************** 2048 Game ****************\n");
    printf("********************************************\n");
    printf("1. User\n2. AI\n\nChoose whom to play: ");
    currentPlayer;
    scanf("%d", &currentPlayer);

    createGame();
    drawScene();

    while (canUserMove())
    {
        if (currentPlayer == 2) // AI's turn
        {
            char bestMove = chooseBestMove();
            applyMove(bestMove);
            printf("AI's Move: %c\n", bestMove);
            drawScene();
            Sleep(10);
            createNewNumber();
            drawScene();
            if (isBoardFull())
            {
                gameOver();
                break;
            }
        }
        else if (currentPlayer == 1) // User's turn
        {
            char move;
            printf("Enter your move (l - left, r - right, u - up, d - down): ");
            scanf(" %c", &move);

            switch (move)
            {
            case 'l':
            case 'r':
            case 'u':
            case 'd':
                applyMove(move);
                drawScene();
                break;
            default:
                printf("Invalid move!\n");
                continue;
            }

            createNewNumber();
            drawScene();
        }
    }
}

// Main game loop
int main()
{
    // system("cls");
    // printf("********************************************\n");
    // printf("***************** 2048 Game ****************\n");
    // printf("********************************************\n");

    play();

    return 0;
}
