#include "Lists.h"
#include "Game.h"

SingleSourceMovesList* FindSingleSourceOptimalMove(SingleSourceMovesTree* moves_tree)
{
	char player;
	SingleSourceMovesList* newList;
	SingleSourceMovesTreeNode* bestPath;

	newList = (SingleSourceMovesList*)malloc(sizeof(SingleSourceMovesList)); // FREED
	check_Malloc_List(newList);
	createNewList(newList);

	bestPath = moves_tree->source;
	player = getCharByPosition(moves_tree->source->board, moves_tree->source->pos);

	if (moves_tree->source == NULL)
		return NULL;

	findBestPathWithNoCapture(moves_tree->source, &bestPath, player);
	findbestPathTNode(moves_tree->source, &bestPath, player);

	if (bestPath->total_captures_so_far > ZERO) //if the best path contains captures
	{
		int level;
		bool found = false; //returns true if best path is found
		level= bestPath->total_captures_so_far; //checks for the node with best path level 
		helper(moves_tree->source, newList, bestPath, &level, ZERO, &found);
	}
	else if (bestPath == moves_tree->source) //The path has no moves
	{
		return NULL;
	}
	else //The path has moves but no captures
	{
		insertDataToStartList(newList, moves_tree->source->pos, moves_tree->source->total_captures_so_far);
		insertDataToEndList(newList, bestPath->pos, bestPath->total_captures_so_far);
	}

	return newList;
}

void findBestPathWithNoCapture(SingleSourceMovesTreeNode* root, SingleSourceMovesTreeNode** maxPath, char player)
{
	if (player == T_PLAYER)
	{
		if (root->next_move[RIGHT] != NULL)
			*maxPath = root->next_move[RIGHT];

		else if (root->next_move[LEFT] != NULL)
			*maxPath = root->next_move[LEFT];
	}
	else
	{
		if (root->next_move[LEFT] != NULL)
			*maxPath = root->next_move[LEFT];

		else if (root->next_move[RIGHT] != NULL)
			*maxPath = root->next_move[RIGHT];
	}
}

void findbestPathTNode(SingleSourceMovesTreeNode* root, SingleSourceMovesTreeNode** bestPath, char player)
{
	if (root == NULL)
		return;

	if (player == T_PLAYER)
	{
		if (root->total_captures_so_far >= (*bestPath)->total_captures_so_far)
			*bestPath = root;

		findbestPathTNode(root->next_move[LEFT], bestPath, player);
		findbestPathTNode(root->next_move[RIGHT], bestPath, player);
	}
	else //Player is 'B'
	{
		if (root->total_captures_so_far > (*bestPath)->total_captures_so_far)
			*bestPath = root;

		findbestPathTNode(root->next_move[LEFT], bestPath, player);
		findbestPathTNode(root->next_move[RIGHT], bestPath, player);
	}
}

void helper(SingleSourceMovesTreeNode* root, SingleSourceMovesList* lst, SingleSourceMovesTreeNode* bestPath, int* level, int currLevel, bool* found)
{
	if (root == NULL)
		return;

	if (root == bestPath)
	{
		insertDataToStartList(lst, root->pos, root->total_captures_so_far);
		(*level)--;
		(*found) = true;
	}

	helper(root->next_move[LEFT], lst, bestPath, level, currLevel + ONE, found);
	helper(root->next_move[RIGHT], lst, bestPath, level, currLevel + ONE, found);

	if ((*level) == currLevel && *found == true) //if we are at the starting level and if the node with maximum amout of captures is found
	{
		insertDataToStartList(lst, root->pos, root->total_captures_so_far);
		(*level)--;
	}

}

void insertDataToStartList(SingleSourceMovesList* lst, checkersPos* pos, unsigned short captures)
{
	SingleSourceMovesListCell* newHead;
	newHead = createNewListNode(pos, captures);
	insertNodeToStartList(lst, newHead);
}

void insertNodeToStartList(SingleSourceMovesList* lst, SingleSourceMovesListCell* head)
{
	if (isEmptyList(lst))
	{
		lst->head = lst->tail = head;
	}
	else
	{
		head->next = lst->head;
		lst->head = head;
	}
}

void printList(SingleSourceMovesList* lst) {
	SingleSourceMovesListCell* curr = lst->head;
	while (curr != NULL)
	{
		printf("Captures Made: %d\n", curr->captures);
		printf("Position:\n");
		printf("Row: %d\n", curr->position->row);
		printf("Col: %d\n", curr->position->col);
		curr = curr->next;
	}
}

SingleSourceMovesListCell* createNewListNode(checkersPos* pos, unsigned short captures)
{
	SingleSourceMovesListCell* newNode;
	newNode = (SingleSourceMovesListCell*)malloc(sizeof(SingleSourceMovesListCell)); // FREED
	check_Malloc_ListCell(newNode);
	newNode->position = pos;
	newNode->captures = captures;
	newNode->next = NULL;
	return newNode;
}

void insertNodeToEndList(SingleSourceMovesList* lst, SingleSourceMovesListCell* tail) {
	if (isEmptyList(lst) == true) {
		lst->head = lst->tail = tail;
	}
	else {
		lst->tail->next = tail;
		lst->tail = tail;
	}
	tail->next = NULL;
}

void insertDataToEndList(SingleSourceMovesList* lst, checkersPos* pos, unsigned short captures)
{
	SingleSourceMovesListCell* newTail;
	newTail = createNewListNode(pos, captures);
	insertNodeToEndList(lst, newTail);
}

void createNewList(SingleSourceMovesList* list) {
	list->head = list->tail = NULL;
}

bool isEmptyList(SingleSourceMovesList* list) {
	if (list->head == NULL) {
		return true;
	}
	else
		return false;
}

void freeList(SingleSourceMovesList* list) {
	SingleSourceMovesListCell* curr = NULL;
	SingleSourceMovesListCell* saver = NULL;
	curr = list->head;
	while (curr != NULL) {
		saver = curr->next;
		free(curr->position);
		free(curr);
		curr = saver;
	}
}

void check_Malloc_ListCell(SingleSourceMovesListCell* checkCell) {
	if (checkCell == NULL) {
		printf("Memory allocation failed!");
		exit(ONE);
	}
}

void check_Malloc_List(SingleSourceMovesList* lst) {
	if (lst == NULL) {
		printf("Memory allocation failed!");
		exit(ONE);
	}
}

MultipleSourceMovesList* FindAllPossiblePlayerMoves(Board board, Player player) {
	// Create new moves list
	MultipleSourceMovesList* new_list = (MultipleSourceMovesList*)malloc(sizeof(MultipleSourceMovesList)); // FREED
	checkMallocMultiMovesList(new_list);
	createNewMovesList(new_list);

	for (int row = DEFAULT_VALUE; row < BOARD_SIZE; row++) {
		for (int col = DEFAULT_VALUE; col < BOARD_SIZE; col++) {
			if (board[row][col] == player) { // Check if current position has the player
				if (hasMoves(board, row, col)) { // Check if the player has moves
					// Create moves tree
					checkersPos src = { row, col };
					SingleSourceMovesTree* movesTree = FindSingleSourceMoves(board, &src); // FREED

					// Create best moves list
					SingleSourceMovesList* single_source_moves_list = FindSingleSourceOptimalMove(movesTree); // FREED

					// Free tree
					freeTree(movesTree->source);
					free(movesTree);

					// Add best moves to the list
					if(single_source_moves_list != NULL)
						insertDataToEndMovesList(new_list, single_source_moves_list);
				}
			}
		}
	}
	return new_list;
}

MultipleSourceMovesListCell* createNewMovesListNode(SingleSourceMovesList* data) {
	// Create new cell
	MultipleSourceMovesListCell* newCell = (MultipleSourceMovesListCell*)malloc(sizeof(MultipleSourceMovesListCell)); // FREED
	check_Malloc_ListCell(newCell);

	// Assign values
	newCell->single_source_moves_list = (SingleSourceMovesList*)malloc(sizeof(SingleSourceMovesList)); // FREED
	check_Malloc_ListCell(newCell->single_source_moves_list);
	newCell->single_source_moves_list->head = data->head;
	newCell->single_source_moves_list->tail = data->tail;
	/*createNewList(newCell->single_source_moves_list);
	copyList(newCell->single_source_moves_list, data);*/
	newCell->next = NULL;

	return newCell;
}

void checkMallocMultiMovesListCell(MultipleSourceMovesListCell* cell) {
	if (cell == NULL) {
		printf("Memory allocation failed!");
		exit(ONE);
	}
}

void checkMallocMultiMovesList(MultipleSourceMovesList* lst) {
	if (lst == NULL) {
		printf("Memory allocation failed!");
		exit(ONE);
	}
}

void insertNodeToEndMovesList(MultipleSourceMovesList* lst, MultipleSourceMovesListCell* tail) {
	if (isEmptyMovesList(lst) == true) {
		lst->head = lst->tail = tail;
	}
	else {
		lst->tail->next = tail;
		lst->tail = tail;
	}
	tail->next = NULL;
}

void insertDataToEndMovesList(MultipleSourceMovesList* lst, SingleSourceMovesList* data) {
	SingleSourceMovesListCell* newTail;
	newTail = createNewMovesListNode(data);
	insertNodeToEndMovesList(lst, newTail);
}

void createNewMovesList(MultipleSourceMovesList* list) {
	list->head = list->tail = NULL;
}

bool isEmptyMovesList(MultipleSourceMovesList* list) {
	if (list->head == NULL) {
		return true;
	}
	else
		return false;
}

void copyList(SingleSourceMovesList* dest, SingleSourceMovesList* src) {
	SingleSourceMovesListCell* temp = src->head;

	while (temp != NULL) {
		insertNodeToEndList(dest, temp);
		temp = temp->next;
	}
}

void printMovesList(MultipleSourceMovesList* lst) {
	MultipleSourceMovesListCell* curr = lst->head;
	while (curr != NULL)
	{
		printList(curr->single_source_moves_list);
		curr = curr->next;
	}
}

void freeMovesList(MultipleSourceMovesListCell* node) {
	MultipleSourceMovesListCell* curr = NULL;
	MultipleSourceMovesListCell* saver = NULL;
	curr = node;
	while (curr != NULL) {
		saver = curr->next;
		freeList(curr->single_source_moves_list);
		free(curr->single_source_moves_list);
		free(curr);
		curr = saver;
	}
}

SingleSourceMovesList* getOptimalMovesList(MultipleSourceMovesList* list, Player player) {
	int maxCaptures = DEFAULT_VALUE;

	// Iterate once to find the highest captures number
	maxCaptures = getMaxCaptures(list);

	// Iterate again to find if there are more than two single source moves lists with the same amount of max captures
	if (isInMoreThanTwoLists(list, maxCaptures))
		return getPath(list, player, maxCaptures);
	else
		return getMaxCapturesPath(list, maxCaptures);
}

int getMaxCaptures(MultipleSourceMovesList* list) {
	int max = DEFAULT_VALUE;
	int temp = DEFAULT_VALUE;

	MultipleSourceMovesListCell* curr = list->head;
	while (curr != NULL) {
		// Get captures amount in cell
		temp = curr->single_source_moves_list->tail->captures;

		// Compare it to max value. if it's higher, put it in max
		if (temp > max)
			max = temp;

		curr = curr->next;
	}

	return max;
}

bool isInMoreThanTwoLists(MultipleSourceMovesList* list, int total_captures) {
	int count = DEFAULT_VALUE;
	int temp = DEFAULT_VALUE;

	MultipleSourceMovesListCell* curr = list->head;
	while (curr != NULL) {
		// Get captures amount in cell
		temp = curr->single_source_moves_list->tail->captures;

		// Compare it to max value. if it's higher, put it in max
		if (temp == total_captures)
			count++;

		if (count == TWO)
			return true;

		curr = curr->next;
	}

	return false;

}

SingleSourceMovesList* getPath(MultipleSourceMovesList* list, Player player, int total_captures) {
	int temp = DEFAULT_VALUE;
	SingleSourceMovesList* tempPointer = NULL;

	// player is T player case
	if (player == T_PLAYER) {
		MultipleSourceMovesListCell* curr = list->head;
		while (curr != NULL) {
			// Get captures amount in cell
			temp = curr->single_source_moves_list->tail->captures;

			if (temp == total_captures) // Hold the value of the last list you come across that has the num of captures we want
				tempPointer = curr->single_source_moves_list;

			curr = curr->next;
		}
		return tempPointer;
	}

	int currRow, currCol;
	int lowestCol = TEN, lowestRow = TEN; // high starting values

	// player is B player case
	if (player == B_PLAYER) {
		MultipleSourceMovesListCell* curr = list->head;
		while (curr != NULL) {
			// Get captures amount and pos in cell
			temp = curr->single_source_moves_list->tail->captures;
			currRow = curr->single_source_moves_list->head->position->row;
			currCol = curr->single_source_moves_list->head->position->col;

			if (temp == total_captures) {  // Hold the value of the last list you come across that has the num of captures we want
				if (currRow < lowestRow || (currRow == lowestRow && currCol < lowestCol)) {
					tempPointer = curr->single_source_moves_list;
					lowestRow = currRow;
					lowestCol = currCol;
				}
			}

			curr = curr->next;
		}

		return tempPointer;
	}

	return tempPointer;
}

SingleSourceMovesList* getMaxCapturesPath(MultipleSourceMovesList* list, int total_captures) {
	int temp;
	MultipleSourceMovesListCell* curr = list->head;
	while (curr != NULL) {
		// Get captures amount in cell
		temp = curr->single_source_moves_list->tail->captures;

		// Compare it to max value. if it's higher, put it in max
		if (temp == total_captures)
			return curr->single_source_moves_list;

		curr = curr->next;
	}

	return NULL;
}