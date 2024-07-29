#ifndef __LISTS_H
#define __LISTS_H
#include "Game.h"
#include "Trees.h"

///// ***** Structs ***** /////

typedef struct _SingleSourceMovesListCell {
	checkersPos* position;
	unsigned short captures;
	struct _singleSourceMovesListCell* next;
}SingleSourceMovesListCell;

typedef struct _SingleSourceMovesList {
	SingleSourceMovesListCell* head;
	SingleSourceMovesListCell* tail;
}SingleSourceMovesList;

typedef struct _multipleSourceMovesListCell {
	SingleSourceMovesList* single_source_moves_list;
	struct _multipleSourceMovesListCell* next;
}MultipleSourceMovesListCell;

typedef struct _multipleSourceMovesList {
	MultipleSourceMovesListCell* head;
	MultipleSourceMovesListCell* tail;
}MultipleSourceMovesList;

///// ***** Functions ***** /////

/*this function recieves a tree of all optional moves (created by function FindSingleSouceMovesTree) and creates
a list of the best route a given player can make. each move (from the best route) is a node in the list*/
SingleSourceMovesList* FindSingleSourceOptimalMove(SingleSourceMovesTree* moves_tree);

/*this function finds the best path for a given player that has only paths with zero captures*/
void findBestPathWithNoCapture(SingleSourceMovesTreeNode* root, SingleSourceMovesTreeNode** maxPath, char player);

/*this function finds the best path for a given player that has only paths with more than captures*/
void findbestPathTNode(SingleSourceMovesTreeNode* root, SingleSourceMovesTreeNode** bestPath, char player);

/*this function helps FindSingleSourceOptimalMove and creates the best list by finding the tree node 
with the most captures. after finding that tree node it returns the level of the best route and fills the
list by levels*/
void helper(SingleSourceMovesTreeNode* root, SingleSourceMovesList* lst, SingleSourceMovesTreeNode* bestPath, int* level, int currLevel, bool* found);

/*this function enters given data to start of list*/
void insertDataToStartList(SingleSourceMovesList* lst, checkersPos* pos, unsigned short captures);

/*this function enters a given node to start of list*/
void insertNodeToStartList(SingleSourceMovesList* lst, SingleSourceMovesListCell* head);

/*this function prints the list*/
void printList(SingleSourceMovesList* lst);

/*this function creates a new list node*/
SingleSourceMovesListCell* createNewListNode(checkersPos* pos, unsigned short captures);

/*this function enters a given node to end of list*/
void insertNodeToEndList(SingleSourceMovesList* lst, SingleSourceMovesListCell* tail);

/*this function enters given data to end of list*/
void insertDataToEndList(SingleSourceMovesList* lst, checkersPos* pos, unsigned short captures);

/*this function creates a new list*/
void createNewList(SingleSourceMovesList* list);

/*this function checks if a list is empty or not*/
bool isEmptyList(SingleSourceMovesList* list);

/*this function frees list memory*/
void freeList(SingleSourceMovesList* list);

/*this function checks if malloc of a list is compleated successfully*/
void check_Malloc_List(SingleSourceMovesList* lst);

/*this function checks if malloc of a list cell is compleated successfully*/
void check_Malloc_ListCell(SingleSourceMovesListCell* checkCell);

// This function returns a list which contains the best move for each piece on the board
MultipleSourceMovesList* FindAllPossiblePlayerMoves(Board board, Player player);

// This function creates a new multiple sources moves list node
MultipleSourceMovesListCell* createNewMovesListNode(SingleSourceMovesList* data);

/*this function checks if malloc of a MultipleSourceMovesListCell is compleated successfully*/
void checkMallocMultiMovesListCell(MultipleSourceMovesListCell* cell);

/*this function checks if malloc of a MultipleSourceMovesList is compleated successfully*/
void checkMallocMultiMovesList(MultipleSourceMovesList* lst);

// This function creates a new multiple moves list
void createNewMovesList(MultipleSourceMovesList* list);

// This function inserts a node to the end of a multiple source moves list
void insertNodeToEndMovesList(MultipleSourceMovesList* lst, MultipleSourceMovesListCell* tail);

// This function inserts data to the end of a moves list
void insertDataToEndMovesList(MultipleSourceMovesList* lst, SingleSourceMovesList* data);

// This function checks if a moves list is empty
bool isEmptyMovesList(MultipleSourceMovesList* list);

// This function copies on single source list to the other
void copyList(SingleSourceMovesList* dest, SingleSourceMovesList* src);

// This function prints out the multiple moves list
void printMovesList(MultipleSourceMovesList* lst);

// This function frees a source moves list
void freeMovesList(MultipleSourceMovesListCell* node);

// This function gets a multiple source moves list and returns the best possible move
// The function will return the list with the most captures.
// If there are more than two lists who has the same most captures, for player B it will take the smallest row and col
// For player T the function will take the list with the highest row and col
SingleSourceMovesList* getOptimalMovesList(MultipleSourceMovesList* list, Player player);

// This function gets a MultipleSourceMovesList and returns the highest amount of captures
int getMaxCaptures(MultipleSourceMovesList* list);

// This function gets a multiplesourcemoveslist and a captures value and returns true if there are more than two lists who has it as total captures mad
bool isInMoreThanTwoLists(MultipleSourceMovesList* list, int total_captures);

// This function gets a multiplesourcemoveslist, a player and total captures. We enter this function knowing that there are more than two ways to get max captures.
// For B it will return the path starting at the lowest row and col value. For T it will return the path starting at the highest row and col value.
SingleSourceMovesList* getPath(MultipleSourceMovesList* list, Player player, int total_captures);

// This function gets a multiplesourcemoveslist, total captures. We enter this function knowing that there is only one good path to make with more captures than the rest.
SingleSourceMovesList* getMaxCapturesPath(MultipleSourceMovesList* list, int total_captures);

#endif // __LISTS_H


