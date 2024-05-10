# 2048-using-AI-Technique

This project aims to implement the game 2048 in C and develop an AI player using basic AI techniques.

## Game Implementation

The game will be implemented in C using standard input/output for simplicity. It will include:

- Game board representation
- Game logic (movement of tiles, merging, etc.)
- User input handling
- Displaying the game state

## AI Player

The AI player will be based on a simple heuristic approach, likely using a search algorithm such as minimax or expectimax combined with a suitable evaluation function.

## Goals

- Implement a playable version of 2048 in C.
- Develop an AI player that can achieve high scores.
- Experiment with different AI strategies and algorithms.

## Running the Game

To compile and run the game:

1. Clone this repository:
   git clone https://github.com/yourusername/2048-ai-c.git
   
2. Navigate to the project directory:
   cd 2048-ai-c

3. Compile the source code:
   gcc -o 2048 main.c game.c ai.c

4. Run the game:
   ./2048
