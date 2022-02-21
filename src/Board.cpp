#include "Board.h"

/**
 * Function name : initializeGameBoard ()
 * Date Created  : 17 October 2012
 * Usage         : initializeGameBoard ();
 * Definition    : This function displays initializes values of the multi-
 *                 dimensional array called gameBoard.
 * Pre-condition : ROWS & COLS are defined as constant macros
 * Post-condition: Array initialize to (~) tilde representing water
 */
void Board::initializeGameBoard() {
	gameBoard = (Cell**)malloc(ROWS * COLS * sizeof(Cell*));
	for (int i = 0; i < ROWS; i++)
		for (int j = 0; j < COLS; j++) {
			Cell* x = new Cell();
			x->symbol = WATER;
			Coordinate* pos = new Coordinate();
			pos->row = i;  
			pos->column = j; 
			x->position = pos;

			Cell** cellP = gameBoard + i * ROWS + j;
			*cellP = x;
		}
}

/**
 * Function name : printGameBoard ()
 * Date Created  : 17 October 2012
 * Usage         : printGameBoard (Cell [][], Boolean);
 * Definition    : This function prints the game board on the console
 *                 screen with an option of showing or not showing the pegs
 * Pre-condition : Game board is of Cell struct type and showPegs
				   if Boolean enum type
 * Post-condition: Game board printed on console screen
 */
void Board::printGameBoard(Boolean showPegs) {

	printf("  0 1 2 3 4 5 6 7 8 9\n");

	for (int i = 0; i < ROWS; i++) {
		printf("%d ", i);

		for (int j = 0; j < COLS; j++) {
			Cell* cellP = *(gameBoard + i * ROWS + j); 
			if (showPegs == TRUE)
				printf("%c ", cellP->symbol);
			else {
				switch (cellP->symbol) {
				case HIT:   printf("%c ", HIT);   break;
				case MISS:  printf("%c ", MISS);  break;
				case WATER:
				default:    printf("%c ", WATER); break;
				}
			}
		}

		putchar('\n');
	}
}

void Board::outputGameBoard(FILE* stream) {

	fprintf(stream, "  0 1 2 3 4 5 6 7 8 9\n");

	for (int i = 0; i < ROWS; i++) {
		fprintf(stream, "%d ", i);

		for (int j = 0; j < COLS; j++) {
			Cell* cellP = *(gameBoard + i * ROWS + j);

			fprintf(stream, "%c ", cellP->symbol);

		}
		fprintf(stream, "\n");
	}
	fprintf(stream, "\n");
}


/**
 * Function name : putShipOnGameBoard ()
 * Date Created  : 17 October 2012
 * Usage         : putShipOnGameBoard (Cell [][], WaterCraft, Coordinate, int)
 * Definition    : This function allows you to put the 5 types of ship in the
 *                 the game board specified in the formal argument
 * Pre-condition : n/a
 * Post-condition: Specific type of ship place on specificied target cell
 */
void Board::putShipOnGameBoard(WaterCraft* ship, Coordinate* position, int direction) {
	Cell** cPP = gameBoard + position->row * ROWS + position->column;
	WaterCraft* sP = ship;
	for (int i = 0; i < sP->length; i++) {
		if (direction == HORIZONTAL)
		{
			Cell* cP = *(cPP + i); 
			cP->symbol = sP->symbol;
		}
			
		else /* VERTICAL */
		{
			Cell* cP = *(cPP + i * ROWS); 
			cP->symbol = sP->symbol;
		}
			
	}
}

/**
 * Function name : manuallyPlaceShipsOnGameBoard ()
 * Date Created  : 17 October 2012
 * Usage         : manuallyPlaceShipsOnGameBoard (Cell [][], WaterCraft []);
 * Definition    : This function allows user to manually place ship on the specified
 *                 gameboard with specific ship
 * Pre-condition : Assuming user inputs correct coordinates as it does not check
 *                 if input is correctly inputted
 * Post-condition: Ships placed on game board
 */
void Board::manuallyPlaceShipsOnGameBoard(WaterCraft** ship) {
	char       stringPosition[11] = "";

	Coordinate position[5];
	Boolean    isValid = FALSE;

	fflush(stdin);

	for (int i = 0; i < NUM_OF_SHIPS; i++) {
		WaterCraft* sP = *(ship + i);
		while (TRUE) {
			system("cls");
			printGameBoard(TRUE);
			printf("> Please enter the %d cells to place the %s across (no spaces):\n", sP->length, sP->name);
			printf("> ");
			scanf("%s", stringPosition);

			/* convertStringtoPosition returns false if unsuccessful */
			if (convertStringtoPosition(position, stringPosition, sP->length)) {

				isValid = TRUE;

				for (int j = 0; j < sP->length; j++) {
					
					Coordinate* pP = position + j;
					Cell* cP = *(gameBoard + ROWS * (pP->row) + pP->column);

					if (cP->symbol == WATER) {
						cP->symbol = sP->symbol;
					}
					else {
					isValid = FALSE;
					printf("> Invalid input!\n");

					if (j != 0)
						while (j >= 0) {
							cP->symbol = WATER;
							j--;
						}

					break;
					}
				}
			}
			else {
			isValid = FALSE;
			printf("> Invalid input!\n");
			}

			if (isValid == TRUE) break;
		}

	}
}

/**
 * Function name : randomlyPlaceShipsOnGameBoard ()
 * Date Created  : 17 October 2012
 * Usage         : randomlyPlaceShipsOnGameBoard (Cell [][], WaterCraft []);
 * Definition    : This function lets the computer randomly place ship on the
 *                 game board
 * Pre-condition : n/a
 * Post-condition: Ships placed on game board
 */
void Board::randomlyPlaceShipsOnGameBoard(WaterCraft** ship) {

	for (int i = 0; i < NUM_OF_SHIPS; i++) {
		WaterCraft* sP = *(ship + i);
		while (TRUE) {
			int direction = getRandomNumber(0, 1); /* 0 -> horizontal, 1 -> vertical */
			Coordinate* position = generatePosition(direction, sP->length);

			if (isValidLocation(position, direction, sP->length)) {
				putShipOnGameBoard(sP, position, direction);
				break;
			}
		}
	}
}

/**
 * Function name : updateGameBoard ()
 * Date Created  : 17 October 2012
 * Usage         : updateGameBoard (Cell [][], Coordinate);
 * Definition    : This function updates the game board whether
 *                 its a hit or miss
 * Pre-condition : n/a
 * Post-condition: Game board updated with proper symbol
 */
void Board::updateGameBoard(Coordinate* target) {
	Cell* cP = *(gameBoard + target->row * ROWS + target->column);
	switch (cP->symbol) {
		/* miss */
	case WATER:
		cP->symbol = MISS;
		break;

		/* hit */
	case CARRIER:
	case BATTLESHIP:
	case CRUISER:
	case SUBMARINE:
	case DESTROYER:
		cP->symbol = HIT;
		break;

	case HIT:
	case MISS:
	default:
		break;
	}
}

/**
 * Function name : isValidLocation ()
 * Date Created  : 17 October 2012
 * Usage         : isValidLocation (Cell [][], Coordinate, int, int);
 * Definition    : This function checks if specified position, direction and
 *                 length is valid on location specified on the game board
 * Pre-condition : n/a
 * Post-condition: n/a
 */
Boolean Board::isValidLocation(Coordinate* position, int direction, int length) {
	Cell* cP;
	Boolean isValid = TRUE;

	for (int i = 0; isValid && i < length; i++) {

		if (direction == HORIZONTAL) {
			if ((position->column + i) < COLS)
			{
				cP = *(gameBoard + position->row * ROWS + position->column + i);
				if (cP->symbol != WATER)
					isValid = FALSE;
			}
			else 
				isValid = FALSE;
		}
		else { /* VERTICAL */
			if ((position->row + i) < ROWS)
			{
				cP = *(gameBoard + (position->row + i) * ROWS + position->column);
				if (cP->symbol != WATER)
					isValid = FALSE;
			}
			else
				isValid = FALSE;
		}
	}

	return isValid;
}

/**
 * Function name : convertStringtoPosition ()
 * Date Created  : 17 October 2012
 * Usage         : convertStringtoPosition (Cell [], char *, int);
 * Definition    : This function converts string coordinates into individual
 *                 integer coordinates
 * Pre-condition : Assuming that user inputs correct coordinates since program
 *                 does not check for inccorrect coordinates
 * Post-condition: n/a
 */
Boolean Board::convertStringtoPosition(Coordinate* position, char* stringPosition, int length) {
	Boolean flag = TRUE;
	char temp = '\0';
	int j = 0, k = 1;

	/* checks if length of input is good */
	if (strlen(stringPosition) / 2 == length) {
		/* loops through the length of the ship */
		for (int i = 0; i < length && flag; i++) {
			/* checks if each cell is a digit */
			Coordinate* pP = position + i;
			if (isdigit(*(stringPosition+j)) && isdigit(*(stringPosition+k))) 
			{
				pP->row = (int) (*(stringPosition + j)-'0');
				pP->column = (int) (*(stringPosition + k) - '0');

				j += 2;
				k += 2;
			}
			else {
				flag = FALSE;
			}
		}
	}
	else {
		flag = FALSE;
	}

	return flag;
}

/**
 * Function name : generatePosition ()
 * Date Created  : 17 October 2012
 * Usage         : generatePosition (int, int);
 * Definition    : This function generates position based on the
 *                 direction and length specified and it can't be
 *                 more than the game board size
 * Pre-condition : n/a
 * Post-condition: n/a
 */
Coordinate* Board::generatePosition(int direction, int length) {
	Coordinate* position = new Coordinate();

	if (direction == HORIZONTAL) {
		position->row = getRandomNumber(0, ROWS - 1);
		position->column = getRandomNumber(0, COLS - length - 1);

	}
	else { /* VERTICAL */
		position->row = getRandomNumber(0, ROWS - length - 1);
		position->column = getRandomNumber(0, COLS - 1);
	}

	return position;
}

/**
 * Function name : getRandomNumber ()
 * Date Created  : 17 October 2012
 * Usage         : getRandomNumber (int, int);
 * Definition    : This function returns a random number between and
 *                 including the lowest to the highest number
 * Pre-condition : n/a
 * Post-condition: n/a
 */

int Board::getRandomNumber(int lowest, int highest) {
	int randomInt;
	while (true)
	{
		randomInt = abs(rand()) % ++highest;
		if (randomInt <= highest && randomInt >= lowest)
			break;
	}
	printf("%d\n", randomInt);
	return randomInt;
}



/**
 * Function name : checkShot ()
 * Date Created  : 17 October 2012
 * Usage         : checkShot (Cell [][], Coordinate);
 * Definition    : This function checks if the coordinates on the
 *                 game board is a miss, hit, water or water craft
 * Pre-condition : n/a
 * Post-condition: n/a
 */
short Board::checkShot(Coordinate* target) {
	int hit = -2;
	Cell* cP = *(gameBoard + target->row * ROWS + target->column); 


	switch (cP->symbol) {
		/* miss */
	case WATER:
		hit = 0;
		break;

		/* hit */
	case CARRIER:
	case BATTLESHIP:
	case CRUISER:
	case SUBMARINE:
	case DESTROYER:
		hit = 1;
		break;

	case HIT:
	case MISS:
	default:
		hit = -1;
		break;
	}

	return hit;
}

/**
 * Function name : isWinner ()
 * Date Created  : 17 October 2012
 * Usage         : isWinner (Stats [], int);
 * Definition    : This function determines if there is a winner based
 *                 on the 17 total pegs for each player
 * Pre-condition : n/a
 * Post-condition: n/a
 */
Boolean Board::isWinner(Stats* players, int player) {
	Boolean isWin = FALSE;

	if ((players+player)->numHits == 17)
		isWin = TRUE;

	return isWin;
}

void Board::checkWin(Stats* players)
{
	 if ((players)->numHits > (players + 1)->numHits)
	 {
		 puts("Player 1 won by hitting more targets!");

	 }
	 else if ((players)->numHits < (players + 1)->numHits)
	 {
		 puts("Player 2 won by hitting more targets!");

	 }
	 else
	 {
		 printf("It was a tie! You both hit %d targets.\n", players->numHits);
	 }
		
}

/**
 * Function name : getTarget ()
 * Date Created  : 17 October 2012
 * Usage         : getTarget ();
 * Definition    : This function reads a row and column values from
 *                 the user
 * Pre-condition : n/a
 * Post-condition: n/a
 */
Coordinate* Board::getTarget() {
	Coordinate* target = new Coordinate();
	int row = -1;
	int col = -1;

	fflush(stdin);
	while (true)
	{
		printf("> Enter Target (ex. > 3 4):\n");
		printf("> ");
		scanf("%d %d", &row, &col);

		bool rowValid = row < 10 && row >= 0;
		bool colValid = col < 10 && col >= 0;

		if (rowValid && colValid)
		{
			target->row = row; 
			target->column = col; 
			break;
		}
	}

	return target;
}


/**
 * Function name : checkBoundsOfCardinal ()
 * Date Created  : 17 October 2012
 * Usage         : checkBoundsOfCardinal (Boolean [], int, int);
 * Definition    : This function checks if the specified direction is
 *                 not over the bounds and sets the cardinals with TRUE
 *                 if its within bounds, otherwise FALSE
 * Pre-condition : n/a
 * Post-condition: Updates the cardinals array
 */
void Board::checkBoundsOfCardinal(Boolean cardinals[], int bound, int direction) {
	switch (direction) {
	case NORTH:
		if (bound < 0)
			cardinals[0] = FALSE;
		else
			cardinals[0] = TRUE;
		break;

	case SOUTH:
		if (bound > 9)
			cardinals[1] = FALSE;
		else
			cardinals[1] = TRUE;
		break;

	case WEST:
		if (bound < 0)
			cardinals[2] = FALSE;
		else
			cardinals[2] = TRUE;
		break;

	case EAST:
		if (bound > 9)
			cardinals[3] = FALSE;
		else
			cardinals[3] = TRUE;
		break;
	}
}


/**
 * Function name : checkSunkShip ()
 * Date Created  : 17 October 2012
 * Usage         : checkSunkShip (shor [][], short, char, FILE *);
 * Definition    : This function check if a ship has sunk based on the
 *                 how of the length of ship is left. If a ship was sunk
 *                 it is logged on an output file
 * Pre-condition : stream to output file was created
 * Post-condition: n/a
 */
Boolean Board::checkSunkShip(short sunkShip[][NUM_OF_SHIPS], short player, char shipSymbol, FILE* stream) {
	Boolean sunked = FALSE;

	switch (shipSymbol) {
	case CARRIER:
		if (--sunkShip[player][0] == 0) {
			printf("> Player %d's Carrier sunked!\n", player + 1);

			/* Write to battleship.log */
			fprintf(stream, "Player %d's Carrier sunked!\n", player + 1);

			sunked = TRUE;
		}
		break;

	case BATTLESHIP:
		if (--sunkShip[player][1] == 0) {
			printf("> Player %d's Battleship sunked!\n", player + 1);

			/* Write to battleship.log */
			fprintf(stream, "Player %d's Battleship sunked!\n", player + 1);

			sunked = TRUE;
		}
		break;

	case CRUISER:
		if (--sunkShip[player][2] == 0) {
			printf("> Player %d's Cruiser sunked!\n", player + 1);

			/* Write to battleship.log */
			fprintf(stream, "Player %d's Cruiser sunked!\n", player + 1);

			sunked = TRUE;
		}
		break;

	case SUBMARINE:
		if (--sunkShip[player][3] == 0) {
			printf("> Player %d's Submarine sunked!\n", player + 1);

			/* Write to battleship.log */
			fprintf(stream, "Player %d's Submarine sunked!\n", player + 1);

			sunked = TRUE;
		}
		break;

	case DESTROYER:
		if (--sunkShip[player][4] == 0) {
			printf("> Player %d's Destroyer sunked!\n", player + 1);

			/* Write to battleship.log */
			fprintf(stream, "Player %d's Destroyer sunked!\n", player + 1);

			sunked = TRUE;
		}
		break;
	}

	return sunked;
}
