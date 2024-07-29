#ifndef __TREES_H
#define __TREES_H
#include "Game.h"

///// ***** Defines ***** /////

#define TWO_OBJECTS 2
#define TWO 2

///// ***** Structs ***** /////

// Tree Node of Single Source Move
typedef struct _SingleSourceMovesTreeNode {
	Board board;
	checkersPos* pos;
	unsigned short total_captures_so_far;
	struct _SingleSourceMovesTreeNode* next_move[TWO_OBJECTS];
}SingleSourceMovesTreeNode;

// Tree of Single Source Moves
typedef struct _SingleSourceMovesTree {
	SingleSourceMovesTreeNode* source;
}SingleSourceMovesTree;

///// ***** Functions ***** /////

// This receives a game board with a given state and a slot in the board.
// If there is a game piece in the slot, the function returns a binary tree of all its move options.
// If there is no game piece, it returns NULL.
SingleSourceMovesTree* FindSingleSourceMoves(Board board, checkersPos* src);

// This function creates a new single source moves tree node
SingleSourceMovesTreeNode* createTreeNode(Board board, checkersPos* pos, unsigned short total_captures);

// This function checks the memory allocation of a tree node
void checkTreeNodeAllocation(SingleSourceMovesTreeNode* node);

// This function checks the memory allocation of a tree
void checkTreeAllocation(SingleSourceMovesTree* tree);

// This function frees a tree
void freeTree(SingleSourceMovesTreeNode* root);

// This function gets a board, a starting position (src) and ending position (dest) and isCapture boolean.
// The function will make the wanted move, and will remove the captured piece from the board.
// The function assumes valid input!!! please use 'isValidMove' before 
void makeMove(Board board, checkersPos* src, checkersPos* dest, bool isCapture);

// This function prints the SingleSourceMovesTree by (DLR) fashion
void printTree(SingleSourceMovesTreeNode* root);

// This function gets a root of a tree.
// The function will check for every possible move for a given piece and create a tree for all available moves.
void getMoves(SingleSourceMovesTreeNode* root, bool madeCapture);

// This function gets a board, current location and a destination location (that has an opponent on it)
// This function checks if the cell after the enemy is empty, and legal, and will return true if the capture move is legal.
bool isCaptureLegal(Board board, checkersPos* curr, checkersPos* dest);

// This function handles the left move of a piece, and it is part of the "makeMove" function, it was made to make it smaller.
void handleLeftMove(Board board, checkersPos* curr_pos, checkersPos* leftMove, bool madeCapture, int direction, SingleSourceMovesTreeNode* root);

// Symmetrical function to handleLeftMove
void handleRightMove(Board board, checkersPos* curr_pos, checkersPos* rightMove, bool madeCapture, int direction, SingleSourceMovesTreeNode* root);

// This function checks memory allocation of checkersPos
void checkCheckersPosAllocation(checkersPos* pos);

#endif // __TREES_H

