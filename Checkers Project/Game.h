#ifndef __GAME_H
#define __GAME_H
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

///// ***** Defines ***** /////

#define DEFAULT_VALUE 0
#define BOARD_SIZE 8
#define A_CHAR 'A'
#define H_CHAR 'H'
#define ONE_CHAR '1'
#define ZERO_CHAR '0'
#define SEVEN_CHAR '7'
#define B_PLAYER 'B'
#define T_PLAYER 'T'
#define EMPTY_SLOT ' '
#define ONE 1
#define NINE 9
#define LEFT 0
#define RIGHT 1
#define ZERO 0
#define SEVEN 7
#define BY_TWO 2
#define UPWARD -1
#define DOWNWARD 1
#define RIGHT_DIAGONAL 1
#define LEFT_DIAGONAL -1
#define SPECIAL_DEFAULT -1
#define TEN 10

// 8x8 Board Representation Variable
typedef unsigned char Board[BOARD_SIZE][BOARD_SIZE];

// Player Representation Variable
typedef unsigned char Player;

// Square Representation Struct
typedef struct _checkersPos
{
	char row, col;
}checkersPos;

///// ***** Functions ***** /////

// This function checks if a given int is in the y axis bounds.
// For example, -1 is not between 0 and 7, so it will return false.
bool isInYAxis(int y_value);

// This function checks if a given int is in the x axis bounds.
// For example, -1 is not between 0 and 7, so it will return false.
bool isInXAxis(int x_value);

// This function copies one board to the other.
// This function assumes valid input.
void copyBoard(Board dest, Board src);

// This function gets a board and a position, and will return the character in it
char getCharByPosition(Board board, checkersPos* pos);

// This function gets a board, a player and a position. 
// The function will return true if the player and the player in the position are opponents
bool isOpponent(Board board, Player player, checkersPos* pos);

// This function gets a board and a position of a checkers piece.
// The function will return the moving direction of the piece. B goes up so it returns 1, T goes down so it returns -1.
int getMovingDirection(Board board, checkersPos* pos);

// This function checks if a move is valid. A move is 'valid' if the destination has an enemy piece or an empty cell.
bool isValidMove(Board board, checkersPos* curr, checkersPos* dest, bool madeCapture);

// This function prints out a board
void printBoard(Board board);

// this function receives a board, a row and a collum. the function will check if a given player has moves to play
bool hasMoves(Board board, int row, int col);

// This function makes a turn according to the games' rules
void Turn(Board board, Player player);

// This function is the main function of the game. It gets an opening board and a starting player, and it plays a checkers game according to the question's orders
void playGame(Board board, Player starting_player);

// This function checks if there is a winner in the checkers game. The function will print out the winner, will write how many moves the winner has made
// And also the one who has made the most moves and their number.
bool isWinner(Board board, int T_moves, int B_moves);

// This function sets the starting player in a game. T player is playerTurn = true, B player is playerTurn = false
void setStartingPlayer(bool* playerTurn, Player starting_player);

// This function prints out the player who has made the most captures at the end of the game.
void printMaxCaptures();

#endif // !__GAME_H


