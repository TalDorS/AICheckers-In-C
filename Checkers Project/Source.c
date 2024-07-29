// Project made by Tal Dor 314630336
#define CRT_SECURE_NO_WARNINGS
#include "Source.h"


void main() {
    Board board = {
        {' ', 'T', ' ', 'T', ' ', 'T', ' ', 'T'}, // Row 0
        {'T', ' ', 'T', ' ', 'T', ' ', 'T', ' '}, // Row 1
        {' ', ' ', ' ', 'T', ' ', 'T', ' ', 'T'}, // Row 2
        {'T', ' ', ' ', ' ', ' ', ' ', ' ', ' '}, // Row 3
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}, // Row 4 
        {'B', ' ', 'B', ' ', 'B', ' ', 'B', ' '}, // Row 5
        {' ', 'B', ' ', 'B', ' ', 'B', ' ', 'B'}, // Row 6
        {'B', ' ', 'B', ' ', 'B', ' ', 'B', ' '}  // Row 7
    };

    playGame(board, B_PLAYER);

}