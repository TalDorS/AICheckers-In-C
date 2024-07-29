#include "Trees.h"

SingleSourceMovesTree* FindSingleSourceMoves(Board board, checkersPos* src) {
	// Create temp board for the root of the tree
	Board temp_board;
	copyBoard(temp_board, board);

	// Check if there is a player on a given slot
	if (board[src->row][src->col] == EMPTY_SLOT)
		return NULL;

	// Create Tree
	SingleSourceMovesTree* new_tree = (SingleSourceMovesTree*)malloc(sizeof(SingleSourceMovesTree));
	checkTreeAllocation(new_tree);

	// Create Tree Root
	new_tree->source = createTreeNode(temp_board, src, ZERO);

	// Get all available moves recursively
	getMoves(new_tree->source, false);

	// Return tree
	return new_tree;
}

SingleSourceMovesTreeNode* createTreeNode(Board board, checkersPos* pos, unsigned short total_captures) {

	// Allocate memory + check memory allocation
	SingleSourceMovesTreeNode* new_node = (SingleSourceMovesTreeNode*)malloc(sizeof(SingleSourceMovesTreeNode));
	checkTreeNodeAllocation(new_node);

	// Allocate memory for pos pointer
	new_node->pos = (checkersPos*)malloc(sizeof(checkersPos));
	checkCheckersPosAllocation(new_node->pos);

	// Assign new values to new node
	copyBoard(new_node->board, board);
	new_node->pos->col = pos->col;
	new_node->pos->row = pos->row;
	new_node->total_captures_so_far = total_captures;
	new_node->next_move[LEFT] = NULL;
	new_node->next_move[RIGHT] = NULL;

	// Return
	return new_node;
}

void checkTreeNodeAllocation(SingleSourceMovesTreeNode* node) {
	if (node == NULL) {
		printf("Memory allocation failed!");
		exit(ONE);
	}
}

void checkCheckersPosAllocation(checkersPos* pos) {
	if (pos == NULL) {
		printf("Memory allocation failed!");
		exit(ONE);
	}
}

void checkTreeAllocation(SingleSourceMovesTree* tree) {
	if (tree == NULL) {
		printf("Memory allocation failed!");
		exit(ONE);
	}
}

void getMoves(SingleSourceMovesTreeNode* root, bool madeCapture) {
	int currRow = root->pos->row;
	int currCol = root->pos->col;
	bool isCapture = false;

	// Get the moving direction of the current piece, T moves downward, and B moves upward
	int direction = getMovingDirection(root->board, root->pos);

	// Get new positions for left diagonal and right diagonal moves
	checkersPos leftMove = { currRow + direction, currCol + LEFT_DIAGONAL };
	checkersPos rightMove = { currRow + direction, currCol + RIGHT_DIAGONAL };

	// Base case - no moves available
	if (!isValidMove(root->board, root->pos, &leftMove, madeCapture) && !isValidMove(root->board, root->pos, &rightMove, madeCapture)) {
		return;
	}

	// Left move is valid case
	if (isValidMove(root->board, root->pos, &leftMove, madeCapture)) {
		// Make left move and create the left node
		handleLeftMove(root->board, root->pos, &leftMove, madeCapture, direction, root);
	}

	// Right move is valid case
	if (isValidMove(root->board, root->pos, &rightMove, madeCapture)) {
		// Make right move and create the right node
		handleRightMove(root->board, root->pos, &rightMove, madeCapture, direction, root);
	}
}

void handleLeftMove(Board board, checkersPos* curr_pos, checkersPos* leftMove, bool madeCapture, int direction, SingleSourceMovesTreeNode* root) {
	// Check if the valid move has a capturing or not
	bool isCapture = isOpponent(root->board, root->board[curr_pos->row][curr_pos->col], leftMove);

	// If capturing is legal, make the piece jump after the enemy piece.
	if (isCapture)
	{
		leftMove->col += LEFT_DIAGONAL;
		leftMove->row += direction;
	}

	if (isCapture || !madeCapture) {
		// Create node for the next move
		root->next_move[LEFT] = createTreeNode(root->board, leftMove, root->total_captures_so_far + isCapture);

		// Make move on the new node's board
		makeMove(root->next_move[LEFT]->board, root->pos, leftMove, isCapture);
	}

	// Continue the recursion only if there was a capture move
	if (isCapture)
		getMoves(root->next_move[LEFT], true);
}

void handleRightMove(Board board, checkersPos* curr_pos, checkersPos* rightMove, bool madeCapture, int direction, SingleSourceMovesTreeNode* root) {
	// Check if the valid move has a capturing or not
	bool isCapture = isOpponent(root->board, root->board[curr_pos->row][curr_pos->col], rightMove);

	// If capturing is legal, make the piece jump after the enemy piece.
	if (isCapture == true)
	{
		rightMove->col += RIGHT_DIAGONAL;
		rightMove->row += direction;
	}

	// Make a new node, only if the next move is a capture move, and there was no capture mode previously, meaning that it's the first move
	if (isCapture || !madeCapture) {
		// Create node for the next move
		root->next_move[RIGHT] = createTreeNode(root->board, rightMove, root->total_captures_so_far + isCapture);

		// Make move on the new node's board
		makeMove(root->next_move[RIGHT]->board, root->pos, rightMove, isCapture);
	}

	// Continue the recursion only if there was a capture move
	if (isCapture)
		getMoves(root->next_move[RIGHT], true);
}

bool isValidMove(Board board, checkersPos* curr, checkersPos* dest, bool madeCapture) {
	int currRow = curr->row;
	int currCol = curr->col;
	int destRow = dest->row;
	int destCol = dest->col;

	// Get current position's player
	Player curr_player = board[currRow][currCol];
	checkersPos temp_position;

	// Check if destination is not in bounds
	if (destRow < DEFAULT_VALUE || destRow > BOARD_SIZE || destCol < DEFAULT_VALUE || destCol >= BOARD_SIZE)
		return false;

	// Check if destination is empty cell and a capture was made in the previous turn
	if (board[destRow][destCol] == EMPTY_SLOT && madeCapture)
		return false;

	// Check if destination is empty cell
	if (board[destRow][destCol] == EMPTY_SLOT)
		return true;

	// Check if destination has an opponent and if it's legal to make the capture
	if (isCaptureLegal(board, curr, dest)) {
		return true;
	}

	return false;
}

bool isCaptureLegal(Board board, checkersPos* curr, checkersPos* dest) {
	int currRow = curr->row;
	int currCol = curr->col;
	int destRow = dest->row;
	int destCol = dest->col;

	Player curr_player = board[currRow][currCol];

	if (isOpponent(board, board[currRow][currCol], dest)) {
		switch (curr_player)
		{
		case B_PLAYER:
			if (currCol > destCol) { // Enemy to the upper-left case
				return (board[destRow + UPWARD][destCol + LEFT_DIAGONAL] == EMPTY_SLOT) && (isInXAxis(destCol + LEFT_DIAGONAL) && (isInYAxis(destRow + UPWARD)));
			}
			else // Enemy to the upper-right case
			{
				return (board[destRow + UPWARD][destCol + RIGHT_DIAGONAL] == EMPTY_SLOT) && (isInXAxis(destCol + RIGHT_DIAGONAL) && (isInYAxis(destRow + UPWARD)));
			}
			break;
		case T_PLAYER:
			if (currCol > destCol) { // Enemy to the lower-left case
				return (board[destRow + DOWNWARD][destCol + LEFT_DIAGONAL] == EMPTY_SLOT) && (isInXAxis(destCol + LEFT_DIAGONAL) && (isInYAxis(destRow + DOWNWARD)));
			}
			else // Enemy to the lower-right case
			{
				return (board[destRow + DOWNWARD][destCol + RIGHT_DIAGONAL] == EMPTY_SLOT) && (isInXAxis(destCol + RIGHT_DIAGONAL) && (isInYAxis(destRow + DOWNWARD)));
			}
			break;
		default:
			break;
		}

	}
}

void makeMove(Board board, checkersPos* src, checkersPos* dest, bool isCapture) {
	// Make move on the board
	board[dest->row][dest->col] = board[src->row][src->col];
	board[src->row][src->col] = EMPTY_SLOT;

	// In case of a capture, remove the captured piece from the board
	if (isCapture) {
		int capturedRow = (src->row + dest->row) / BY_TWO;
		int capturedCol = (src->col + dest->col) / BY_TWO;
		board[capturedRow][capturedCol] = EMPTY_SLOT;
	}
}

void freeTree(SingleSourceMovesTreeNode* root) {
	if (root == NULL)
		return;

	freeTree(root->next_move[LEFT]);
	freeTree(root->next_move[RIGHT]);
	free(root);
}

void printTree(SingleSourceMovesTreeNode* root) {
	if (root == NULL)
		return;

	printf("Captures Made: %d\n", root->total_captures_so_far);
	printf("Position:\n");
	printf("Row: %d\n", root->pos->row);
	printf("Col: %d\n", root->pos->col);
	printBoard(root->board);
	printTree(root->next_move[LEFT]);
	printTree(root->next_move[RIGHT]);
}

