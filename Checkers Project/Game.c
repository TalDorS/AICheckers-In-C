#include "Game.h"
#include "Source.h"

// Global variables for stats
int maxCapturesT = DEFAULT_VALUE;
int maxCapturesB = DEFAULT_VALUE;

bool isInYAxis(int y_value) {
	if (y_value >= ZERO && y_value <= SEVEN) {
		return true;
	}
	else
	{
		return false;
	}
}

bool isInXAxis(int x_value) {
	if (x_value >= ZERO && x_value <= SEVEN) {
		return true;
	}
	else
	{
		return false;
	}
}

void copyBoard(Board dest, Board src) {
	for (int i = DEFAULT_VALUE; i < BOARD_SIZE; i++) {
		for (int j = DEFAULT_VALUE; j < BOARD_SIZE; j++) {
			dest[i][j] = src[i][j];
		}
	}
}

char getCharByPosition(Board board, checkersPos* pos) {
	return board[pos->row][pos->col];
}

bool isOpponent(Board board, Player player, checkersPos* pos)
{
	Player temp_player = board[pos->row][pos->col];
	if (temp_player != EMPTY_SLOT && temp_player != player) {
		return true;
	}
	else
	{
		return false;
	}
}

int getMovingDirection(Board board, checkersPos* pos) {
	switch (board[pos->row][pos->col])
	{
	case B_PLAYER:
		return UPWARD;
		break;
	case T_PLAYER:
		return DOWNWARD;
		break;
	default:
		break;
	}

	return DEFAULT_VALUE;
}

void printBoard(Board board) {
	char ch = A_CHAR;

	printf("+-+-+-+-+-+-+-+-+-+");
	printf("\n+ |1|2|3|4|5|6|7|8|");
	for (int i = DEFAULT_VALUE; i < BOARD_SIZE; i++) {
		printf("\n+-+-+-+-+-+-+-+-+-+");
		for (int j = DEFAULT_VALUE; j < BOARD_SIZE; j++) {
			if (j == DEFAULT_VALUE)
				printf("\n|%c|%c|",(ch + i) ,board[i][j]);
			else
				printf("%c|", board[i][j]);
		}
	}
	printf("\n+-+-+-+-+-+-+-+-+-+");
	printf("\n");
}

bool hasMoves(Board board, int row, int col) {
	// Initiate player in the row and col and his position
	Player player = board[row][col];
	checkersPos pos = { row, col };

	// Get moving direction
	int direction = getMovingDirection(board, &pos);

	// Check if there are two same players infront of player
	if (player == board[row + direction][col + LEFT_DIAGONAL] && player == board[row + direction][col + RIGHT_DIAGONAL]) {
		return false;
	}
	else if (player == board[row + direction][col + LEFT_DIAGONAL]) {
		if (!isInXAxis(col + RIGHT_DIAGONAL))
			return false;
	}
	else if (player == board[row + direction][col + RIGHT_DIAGONAL]) {
		if (!isInXAxis(col + LEFT_DIAGONAL))
			return false;
	}

	// Check if there are two enemy players infront of player and two enemy players behind them
	switch (player)
	{
	case T_PLAYER:
		if (board[row + direction][col + LEFT_DIAGONAL] == B_PLAYER && board[row + direction][col + RIGHT_DIAGONAL] == B_PLAYER
			&& board[row + TWO * direction][col + TWO * LEFT_DIAGONAL] == B_PLAYER && board[row + TWO * direction][col + TWO * RIGHT_DIAGONAL] == B_PLAYER)
			return false;
		break;
	case B_PLAYER:
		if (board[row + direction][col + LEFT_DIAGONAL] == T_PLAYER && board[row + direction][col + RIGHT_DIAGONAL] == T_PLAYER
			&& board[row + TWO * direction][col + TWO * LEFT_DIAGONAL] == T_PLAYER && board[row + TWO * direction][col + TWO * RIGHT_DIAGONAL] == T_PLAYER)
			return false;
	default:
		break;
	}

	return true;
}

void Turn(Board board, Player player) {
	// Get all possible best moves list
	MultipleSourceMovesList* bestMovesList = FindAllPossiblePlayerMoves(board, player);
	
	// Get the head node of the best move list
	SingleSourceMovesList* bestMove = getOptimalMovesList(bestMovesList, player);

	// Perform move
	SingleSourceMovesListCell* curr = bestMove->head;
	SingleSourceMovesListCell* next;
	SingleSourceMovesListCell* afterNext;

	// Update max total captures made if necessary
	if (player == T_PLAYER && bestMove->tail->captures > maxCapturesT)
		maxCapturesT = bestMove->tail->captures;
	else if (player == B_PLAYER && bestMove->tail->captures > maxCapturesB)
		maxCapturesB = bestMove->tail->captures;

	while (curr->next != NULL) {
		next = curr->next;

		// Get position
		int currRow;
		int currCol;
		int destRow;
		int destCol;

		// Make move
		if ((next != NULL) && (curr->captures < next->captures)) { // Next move is capture
			if (next == bestMove->tail) {
				// Print next turn
				currRow = bestMove->head->position->row;
				currCol = bestMove->head->position->col;
				destRow = bestMove->tail->position->row;
				destCol = bestMove->tail->position->col;
				printf("%c%c->%c%c\n", (A_CHAR + currRow), (ONE_CHAR + currCol), (A_CHAR + destRow), (ONE_CHAR + destCol));
			}
			// Make turn
			makeMove(board, curr->position, next->position, true);
		}
		else if ((next != NULL) && (curr->captures == next->captures)) { // Next move isn't a capture
			// Print next turn
			currRow = bestMove->head->position->row;
			currCol = bestMove->head->position->col;
			destRow = next->position->row;
			destCol = next->position->col;
			printf("%c%c->%c%c\n", (A_CHAR + currRow), (ONE_CHAR + currCol), (A_CHAR + destRow), (ONE_CHAR + destCol));

			// Make turn
			makeMove(board, curr->position, next->position, false);
		}
		curr = next;
	}
	// Free all lists
	freeMovesList(bestMovesList->head);
	free(bestMovesList);
}

void playGame(Board board, Player starting_player)
{	
	int T_moves = DEFAULT_VALUE, B_moves = DEFAULT_VALUE;
	bool playerTurn;

	// Print starting board
	printBoard(board);

	// Set starting player. T player is playerTurn = true, B player is playerTurn = false
	setStartingPlayer(&playerTurn, starting_player);

	// Play untill there is a winner
	while (!isWinner(board, T_moves, B_moves)) {
		// If it's T player's turn
		if (playerTurn) {
			// Make T player turn
			printf("\nT's turn:\n");
			Turn(board, T_PLAYER);
			T_moves++;
		}

		// If it's B player's turn
		if (!playerTurn) {
			// Make B player turn
			printf("\nB's turn:\n");
			Turn(board, B_PLAYER);
			B_moves++;
		}

		// Print board after turn
		printBoard(board);

		// Set next turn
		playerTurn = !(playerTurn);
	}
}

bool isWinner(Board board, int T_moves, int B_moves) {
	bool winner = false;

	// Check if B won
	for (int i = DEFAULT_VALUE; i < BOARD_SIZE; i++) {
		if (board[ZERO][i] == B_PLAYER) {
			printf("B wins!\n");
			printf("B performed %d moves.\n", B_moves);
			printMaxCaptures();
			winner = true;
		}
	}

	// Check if T won
	for (int i = DEFAULT_VALUE; i < BOARD_SIZE; i++) {
		if (board[SEVEN][i] == T_PLAYER) {
			printf("T wins!\n");
			printf("T performed %d moves.\n", T_moves);
			printMaxCaptures();
			winner = true;
		}
	}

	return winner;
}

void setStartingPlayer(bool* playerTurn, Player starting_player) {
	if (starting_player == T_PLAYER)
		*playerTurn = true;
	else
		*playerTurn = false;
}

void printMaxCaptures() {
	// Print max moves
	if (maxCapturesB > maxCapturesT)
		printf("B performed the highest number of captures in a single move - %d", maxCapturesB);
	else if(maxCapturesB < maxCapturesT)
		printf("T performed the highest number of captures in a single move - %d", maxCapturesT);
	else
		printf("both B and T performed the highest number of captures in a single move - %d", maxCapturesT);

}