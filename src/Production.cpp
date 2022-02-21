#include "Production.h"
#include <iostream>
using namespace std;

bool Production::prod(int argc, char* argv[])
{
	puts("Starting Production\n");
	bool answer = false;
	int maxTurns = 0;
	int turnsPlayed = 0;

	if (argc <= 1) //no interesting information
	{
		cout << "Didn't find any arguments for max turns, we will play until someone finds all the ships." << endl;
		maxTurns = -1;
		answer = false;
	}
	else //there is interesting information
	{
		cout << "Found " << argc - 1 << " interesting arguments." << endl;

		char* eptr = (char*)malloc(sizeof(char*));
		long aL = -1L;

		for (int i = 1; i < argc; i++) //don't want to read argv[0]
		{//argv[i] is a string

			switch (i)
			{
			case 1:
				//this is maximum number of turns

				aL = strtol(argv[i], &eptr, 10);
				maxTurns = (int)aL;
				cout << "Maximum number of rounds in this game is " << maxTurns << endl;
				break;

			default:
				cout << "Unexpected argument count." << endl;
				answer = false;
				break;

			}//end of switch
		}//end of for loop on argument count
	}
	cout << "after reading arguments" << endl;

	/**
 * Two boards exist within the game. Hint: each board should be
 * implemented as a 2-dimensional array. Each 2-dimensional array
 * should be 10 X 10. One represents Player1's board and one represents
 * Player2's board.
 */
	Stats players[2] = { {0, 0, 0, 0.0}, {0, 0, 0, 0.0} };

	//Cell playerOneGameBoard[ROWS][COLS];       /* Player one game board */
	//Cell playerTwoGameBoard[ROWS][COLS];       /* Player two game board */

	Coordinate* target = new Coordinate();             /* x, y value of a target */
	Coordinate* targetTemp = new Coordinate();         /* x, y value that holds a temporary value*/
	Coordinate* targetOrigin = new Coordinate();       /* x, y value of the original target */
	Coordinate* targetAI = new Coordinate();           /* x, y value of the targets using AI technique */

	WaterCraft* carrier = new Carrier();
	WaterCraft* battleship = new Battleship();
	WaterCraft* cruiser = new Cruiser();
	WaterCraft* submarine = new Submarine();
	WaterCraft* destroyer = new Destroyer();

	WaterCraft* ship[NUM_OF_SHIPS] = {
		carrier,
		battleship,
		cruiser,
		submarine,
		destroyer
	};

	Boolean    huntMode = TRUE;                     /* mode of randomly selecting a target */
	Boolean    targetMode = FALSE;                    /* mode when there is a hit */
	Boolean    flipper = TRUE;	                  /* flips values of boolean */
	Boolean    cardinals[4] = { TRUE, TRUE, TRUE, TRUE }; /* represents the 4 cardinals, N, S, W, E */
	Boolean    hasAShipSunked = FALSE;                    /* if a ship has sank */


	short sunkShip[2][NUM_OF_SHIPS] = { {5, 4, 3, 3, 2},
									   {5, 4, 3, 3, 2} };  /* tracks parts of the ship destroyed */

	short player = 0;	           /* 0 -> player1, 1 -> player2 */
	short shot = 0;             /* holds temp value if ship has been shot */
	int   option = 0;             /* option for player to place ship manually or randomly */
	int   north = 0,             /* holds change of value when going north */
		south = 0,             /* holds change of value when going south */
		east = 0,             /* holds change of value when going east */
		west = 0;             /* holds change of value when going west */
	int counter = 0;             /* i and counter are used as counters */

	char  shipSymbol = '\0';       /* temporary value to save character symbol of the ship */

	FILE* outStream = NULL;        /* stream to output file battleship.log */

	/* Start of Program */
	outStream = fopen(LOG_FILE_NAME, "w");


	/**
	 * Print welcome screen
	 */
	welcomeScreen();
	systemMessage("                            Hit <ENTER> to continue!\n");

	cout << endl;

	Board* bP1 = new Board();
	Board* bP2 = new Board();
	/**
	 * Each Player's game board should be initialized to all '~' indicating
	 * no ships have been placed on either board.
	 */
	bP1->initializeGameBoard();
	bP2->initializeGameBoard();

	/**
	 * Both players' fleets consist of 5 ships that are hidden from the enemy.
	 * Each ship may be differentiated by its "size" (besides the Cruiser and
	 * Submarine) or number of cells it expands on the game board.
	 *      The Carrier has 5 cells,
	 *      Battleship has 4 cells,
	 *      Cruiser has 3 cells,
	 *      Submarine has 3 cells, and
	 *      the Destroyer has 2 cells.
	 */

	 /**
	  * The program should be built such that the user is Player1 and the
	  * computer is Player2.
	  */

	  /**
	   * Before the game starts, Player1 should have the option to either
	   * manually place each of the 5 ships in his/her fleet or to have them
	   * randomly placed on the board. If Player1 decides to place the ships
	   * manually, then he/she should be prompted to place the Carrier first,
	   * Battleship second, Cruiser third, Submarine fourth, and the Destroyer
	   * last.
	   */
	cout << "> Please select from the following menu:" << endl;
	cout << "> [1] Manually" << endl;
	cout << "> [2] Randomly" << endl;
	cout << "> Enter Option: " << endl;
	cin >> option;

	switch (option) {
	case 1: bP1->manuallyPlaceShipsOnGameBoard(ship);
		break;
	case 2: bP1->randomlyPlaceShipsOnGameBoard(ship);
		break;
	}

	/**
	 * Note that ships cannot be placed diagonally on the board, they can
	 * only be placed vertically or horizontally. You program must check to
	 * see if the user tries to place a ship outside the boundaries of the
	 * board or on top of a ship that has already been placed. Each cell on
	 * the board that contains part of the ship must be indicated by 'c' for
	 * Carrier, 'b' for Battleship, 'r' for Cruiser, 's' for Submarine, or
	 * 'd' for Destroyer.
	 */

	 /**
	  * Player2's ships must be randomly placed. Note that the placement of
	  * Player2's ships must be unknown. Thus, Player2's board will only
	  * display '~' in each cell after the placement of each ship.
	  */
	bP2->randomlyPlaceShipsOnGameBoard(ship);
	cout << "> Player 2 (Computer's) board has been generated." << endl;

	/**
	 * The program should randomly select Player1 or Player2 to go first.
	 */
	player = bP1->getRandomNumber(0, 1);
	cout << "> Player " << player + 1 << " has been randomly selected to go first." << endl;
	systemMessage("> Hit <ENTER> to continue!\n");
	cout << endl;

	/**
	 * Once it has been decided on which player goes first, the game starts.
	 * Whenever it's Player1's turn, a prompt should be displayed asking for
	 * a position to target (specifying where to "shoot") on the enemy's
	 * (Player2's) board (2-dimensional array). The position should be
	 * specified in terms of a row and a column on the board. The row and
	 * column should always be displayed along with the board. If the
	 * position specified happens to hit a ship, then a '*' should replace
	 * the '-' on Player2's board. If the positioned specified misses any
	 * one of the ships in the fleet, then a 'm' should replace the '-' on
	 * Player2's board. Note that from turn-to-turn each player should NOT
	 * be allowed to enter the same position. Also, between turns clear the
	 * screen (system("cls")). In one turn, a player can only take one shot
	 * at the enemy's (other player's) fleet. When Player2 takes a shot at
	 * Player1's board, each hit should be specified with a '*' and each
	 * miss with a 'm' on Player1's board. The game is over win Player1 or
	 * Player2 has sunk all of the ships in the fleet of the enemy.
	 */

	 /**
	  * For each move made by Player1 and Player2, the results should be
	  * echoed to a file called "battleship.log". In this file, you should
	  * log the targeted position by each player on each move and whether it
	  * was a hit on one of the ships in the fleet. Also, if one of the ships
	  * happens to sink, then note this in the log file.
	  */


	while (TRUE) {

		/* Write to battleship.log */
		fprintf(outStream, "Turn %d: Player %d's turn.\n", turnsPlayed + 1, player + 1);

		/* switches back and forth between player 1 and player 2 */
		switch (player) {

		case PLAYER_ONE:
			/* Print player 2's game board*/
			cout << "> Player 2's Board:" << endl;
			bP2->printGameBoard(FALSE);
			cout << "> PLAYER 1'S TURN: turn " << turnsPlayed + 1 << endl;
			
			/* Prompts user to enter target */ 
			do {
				target = bP2->getTarget();
				shot = bP2->checkShot(target);

				/* prompts user that input is invalid or either a hit or miss cell */
				if (shot == -1)
					cout << "> Try inputting another target!" << endl;

			} while (shot == -1);

			/* saves the character of the targeted cell */

			shipSymbol = (*(bP2->gameBoard + target->row * ROWS + target->column))->symbol;


			break;

		case PLAYER_TWO:

			/**** IMPLEMENTATION OF ARTIFICIAL INTELLIGENCE ****/

			/* Print player 1's game board */
			cout << "> Player 1's Board:" << endl;
			bP1->printGameBoard(TRUE);
			cout << "> COMPUTER'S TURN: turn " << turnsPlayed + 1 << endl;

			/**
			 * check from a previous cycle if a ship has sunk,
			 * if yes, it reinitializes values to go back to hunt mode
			 */
			if (hasAShipSunked) {
				hasAShipSunked = FALSE;
				targetMode = FALSE;
				huntMode = TRUE;
			}
			cout << "Checked if ship has sunked" << endl;

			/**
			 * TARGET MODE
			 * This mode is true when the computer randomly selects a target,
			 * and it happens to be a hit, this mode is set to true.
			 */
			if (targetMode) {
				/* retains value of targetAI */
				target->row = targetAI->row;
				target->column = targetAI->column;

				do {
					if (cardinals[NORTH]) {        /* NORTH */
						target->row = north;
					}
					else if (cardinals[SOUTH]) { /* SOUTH */
						target->row = south;
					}
					else if (cardinals[WEST]) {  /* WEST */
						target->column = west;
					}
					else if (cardinals[EAST]) {  /* EAST */
						target->column = east;
					}
					else if (!cardinals[NORTH] && !cardinals[SOUTH] &&
						!cardinals[WEST] && !cardinals[EAST] &&
						!hasAShipSunked) {
						/* All cardinals are FALSE but has not sunk a ship */

						/* reinitiliazes target to the original target value */
						target->row = targetOrigin->row;
						target->column = targetOrigin->column;
						targetTemp->row = target->row;
						targetTemp->column = target->column;

						/**
						 * Counter increments by one, when the loop cycles again and
						 * all cardinals are still FALSE. This ensures that we are checking
						 * one cell over the adjacent cells
						 */

						north = target->row - counter;
						targetTemp->row = north;

						/* checks if the value NORTH of the target is a hit or miss */
						if (bP1->checkShot(targetTemp) != -1 && north >= 0) {
							cardinals[NORTH] = TRUE;
						}

						targetTemp->row = target->row;
						targetTemp->column = target->column;
						south = target->row + counter;
						targetTemp->row = south;

						/* checks if the value SOUTH of the target is a hit or miss */
						if (bP1->checkShot(targetTemp) != -1 && south <= 9) {
							cardinals[SOUTH] = TRUE;
						}

						targetTemp->row = target->row;
						targetTemp->column = target->column;
						west = target->column - counter;
						targetTemp->column = west;

						/* checks if the value WEST of the target is a hit or miss */
						if (bP1->checkShot(targetTemp) != -1 && west >= 0) {
							cardinals[WEST] = TRUE;
						}

						targetTemp->row = target->row;
						targetTemp->column = target->column;
						east = target->column + counter;
						targetTemp->column = east;

						/* checks if the value EAST of the target is a hit or miss */
						if (bP1->checkShot(targetTemp) != -1 && east <= 9) {
							cardinals[EAST] = TRUE;
						}

						/**
						 * increments counter every cycle, serves as a addend to how
						 * many cells to check from the target cell
						 */
						counter++;

					}
					else {
						/* when all conditions are not met, it reverts back to Hunt mode */
						targetMode = FALSE;
						huntMode = TRUE;
						break;
					}

					/* checks if the shot is a hit or miss */
					shot = bP1->checkShot(target);

				} while (shot == -1 && targetMode == TRUE);

				/**
				 * this loop flips the values of the cardinals when it is not needed
				 * takes the cardinal off the stack for directions to check
				 */
				if (shot == 1 && huntMode == FALSE) {
					for (int i = 0; i < 4; i++) {
						if (flipper == FALSE)
							cardinals[i] = FALSE;

						if (cardinals[i] == flipper)
							flipper = FALSE;
					}
				}
				else {
					for (int i = 0; i < 4; i++) {
						if (flipper == TRUE && cardinals[i] != FALSE) {
							cardinals[i] = FALSE;
							break;
						}
					}
				}

				/* reinitializes the value of flipper */
				flipper = TRUE;
			}

			/**
			 * HUNT MODE
			 * This is starting mode of Player 2's turn. This pertains to the
			 * computer randomly selecting cells as targets and checking if they are
			 * just plain water. When a cell equates to a Hit, then hunt mode is
			 * set to false and target mode is set to true
			 */
			if (huntMode) {
				cout << "Entering hunt mode" << endl;
				/* reinitializes important values */
				counter = 1;
				flipper = TRUE;
				for (int i = 0; i < 4; i++)
					cardinals[i] = TRUE;

				/* loop that gets a random number of a cell as target */
				do {
					target->row = bP1->getRandomNumber(0, 9);
					
					target->column = bP1->getRandomNumber(0, 9);
					cout << "row: "<< target->row<<endl;
					cout << "col: "<<target->column << endl;

					/* checks target if it is a miss or hit cell */
					shot = bP1->checkShot(target);
				} while (shot == -1);

				/* if it is a Hit, this saves the value of the original target */
				if (shot == 1) {
					targetOrigin->row = target->row;
					targetOrigin->column = target->column;
				}
			}

			/**
			 * shot values (1 means craft is hit, 0 means missed and -1 means
			 * it is already a hit or miss cell
			 */
			if (shot == 1) {

				/**
				 * if all cardinals are false and no ship was sunk,
				 * reintializes target from value of original target
				 */
				if (!cardinals[NORTH] && !cardinals[SOUTH] &&
					!cardinals[WEST] && !cardinals[EAST] &&
					!hasAShipSunked) {
					target->column = targetOrigin->column;
					target->row = targetOrigin->row;
				}

				/* hunt mode is false during a target mode */
				huntMode = FALSE;
				targetMode = TRUE;
				targetAI->row = target->row;
				targetAI->column = target->column;

				/**
				 * get values of adjacent cells and ensuring that
				 * that it is withing the bounds of gameboard for
				 * NORTH, SOUTH, WEST & EAST
				 */
				if (cardinals[NORTH] == TRUE) {  /* NORTH */
					north = (targetAI->row - 1);
					bP1->checkBoundsOfCardinal(cardinals, north, NORTH);
					targetTemp->column = target->column;
					targetTemp->row = north;
					if (bP1->checkShot(targetTemp) == -1)
						cardinals[NORTH] = FALSE;
				}

				if (cardinals[SOUTH] == TRUE) {  /* SOUTH */
					south = targetAI->row + 1;
					bP1->checkBoundsOfCardinal(cardinals, south, SOUTH);
					targetTemp->column = target->column;
					targetTemp->row = south;
					if (bP1->checkShot(targetTemp) == -1)
						cardinals[SOUTH] = FALSE;
				}

				if (cardinals[WEST] == TRUE) {   /* WEST */
					west = targetAI->column - 1;
					bP1->checkBoundsOfCardinal(cardinals, west, WEST);
					targetTemp->row = target->row;
					targetTemp->column = west;
					if (bP1->checkShot(targetTemp) == -1)
						cardinals[WEST] = FALSE;
				}

				if (cardinals[EAST] == TRUE) {   /* EAST */
					east = targetAI->column + 1;
					bP1->checkBoundsOfCardinal(cardinals, east, EAST);
					targetTemp->row = target->row;
					targetTemp->column = east;
					if (bP1->checkShot(targetTemp) == -1)
						cardinals[EAST] = FALSE;
				}
			}

			/* saves the character of the targeted cell */

			shipSymbol = (*(bP1->gameBoard + target->row * ROWS + target->column))->symbol;

			break;
		}

		/**
		 * Prompts player if it's a hit or miss
		 */

		if (shot == 1) { /* HIT */
			cout << "> " << target->row << ", " << target->column << " is a hit!" << endl;

			/* Write to battleship.log */
			fprintf(outStream, "%d, %d is a hit!\n", target->row, target->column);

			/* Keeps track so number of hits for stats */
			players[player].numHits++;

			/* Checks if the ship has sank */
			if (player == 1)
				hasAShipSunked = bP1->checkSunkShip(sunkShip, !player, shipSymbol, outStream);
			else
				bP2->checkSunkShip(sunkShip, !player, shipSymbol, outStream);

		}
		else {        /* MISS */
			cout << "> " << target->row << ", "<< target->column << " is a miss!" << endl;

			/* Write to battleship.log */
			fprintf(outStream, "%d, %d is a miss!\n", target->row, target->column);
			players[player].numMisses++;
		}

		if (player == 0)
		{/* If Player 1 then update player 2's gameboard */
			bP2->updateGameBoard(target);
			cout << "Updated board:" << endl;
			bP2->printGameBoard(FALSE);
			fprintf(outStream, "Player 2's board:\n");
			bP2->outputGameBoard(outStream);
		}
		else
		{/* If Player 2 then update player 1's gameboard */
			bP1->updateGameBoard(target);
			cout << "Updated board:" << endl;
			bP1->printGameBoard(TRUE);
			fprintf(outStream, "Player 1's board:\n");
			bP1->outputGameBoard(outStream);

		}

		turnsPlayed++;

		if (maxTurns == -1)
		{
			if (bP1->isWinner(players, player)) {
				cout << "> Player " << player + 1 << " wins!" << endl;

				/* Write to battleship.log */
				fprintf(outStream, "\n>>>>> Player %d wins! <<<<<\n", player + 1);
				break;
			}
		}
		else 
		{
			if (turnsPlayed >= maxTurns) {
				cout << "Maximum turns reached!" << endl;
				/* Determins if there is a winner based on number of hits */
				if (bP1->isWinner(players, player))
				{
					cout << "> Player " << player + 1 << " wins!" << endl;

					/* Write to battleship.log */
					fprintf(outStream, "\n>>>>> Player %d wins! <<<<<\n", player + 1);
					break;
				}
				else
				{
					bP1->checkWin(players);
					break;
				}
			}
		}



		systemMessage("> Hit <ENTER> to continue!\n");

		/* switches from player 1 to player 2 */
		player = !player;

		cout << endl;
	}

	/**
	 * At the end of the game, Player1's and Player2's statistics should be
	 * written to "battleship.log". The stats include total number of hits,
	 * total number of misses, total number of shots, hits to misses ratio
	 * (as a percentage), and won or lost the game. Note that the statistics
	 * should be placed into a structure called Stats. You need two variables
	 * of type Stats, one for Player1 and one for Player2. Once the game has
	 * ended you should write the contents of each struct variable to the
	 * "battleship.log" file.
	 */
	players[0].totalShots = players[0].numHits + players[0].numMisses;
	players[0].hitMissRatio = ((double)players[0].numHits / (double)players[0].numMisses) * 100;
	players[1].totalShots = players[1].numHits + players[1].numMisses;
	players[1].hitMissRatio = ((double)players[1].numHits / (double)players[1].numMisses) * 100;
	fprintf(outStream, "+===================================================\n");
	fprintf(outStream, "|                    PLAYER STATS                   \n");
	fprintf(outStream, "+---------------------------------------------------\n");
	fprintf(outStream, "| PLAYER 1 : %d hits                                \n", players[0].numHits);
	fprintf(outStream, "|            %d misses                              \n", players[0].numMisses);
	fprintf(outStream, "|            %d total shots                         \n", players[0].totalShots);
	fprintf(outStream, "|            %.2lf%% hit/miss ratio                 \n", players[0].hitMissRatio);
	fprintf(outStream, "| PLAYER 2 : %d hits                                \n", players[1].numHits);
	fprintf(outStream, "|            %d misses                              \n", players[1].numMisses);
	fprintf(outStream, "|            %d total shots                         \n", players[1].totalShots);
	fprintf(outStream, "|            %.2lf%% hit/miss ratio                 \n", players[1].hitMissRatio);
	fprintf(outStream, "+===================================================");

	fclose(outStream);

	delete(bP1);
	delete(bP2);
	delete(carrier);
	delete(battleship);
	delete(cruiser);
	delete(destroyer);
	delete(submarine);
	delete(target);
	delete(targetAI);
	delete(targetTemp);
	delete(targetOrigin);

	return answer;
}



/**
 * Function name : welcomeScreen ()
 * Date Created  : 17 October 2012
 * Usage         : welcomeScreen ();
 * Definition    : This function displays a welcome message along with the
 *                 Rules of Battleship.
 */
void Production::welcomeScreen(void) {
	printf("XXXXX   XXXX  XXXXXX XXXXXX XX     XXXXXX  XXXXX XX  XX XX XXXX\n");
	printf("XX  XX XX  XX   XX     XX   XX     XX     XX     XX  XX XX XX  XX\n");
	printf("XXXXX  XX  XX   XX     XX   XX     XXXX    XXXX  XXXXXX XX XXXX\n");
	printf("XX  XX XXXXXX   XX     XX   XX     XX         XX XX  XX XX XX\n");
	printf("XXXXX  XX  XX   XX     XX   XXXXXX XXXXXX XXXXX  XX  XX XX XX\n");
	printf("\n\n");
	printf("RULES OF THE GAME:\n");
	printf("1. This is a two player game.\n");
	printf("2. Player 1 is you and Player 2 is the computer.\n");
	printf("3. Player 1 will be prompted if user wants to manually input coordinates\n");
	printf("   for the game board or have the computer randomly generate a game board\n");
	printf("4. There are five types of ships to be placed by longest length to the\n");
	printf("   shortest; [c] Carrier has 5 cells, [b] Battleship has 4 cells, [r] Cruiser\n");
	printf("   has 3 cells, [s] Submarine has 3 cells, [d] Destroyer has 2 cells\n");
	printf("5. The computer randomly selects which player goes first\n");
	printf("6. The game begins as each player tries to guess the location of the ships\n");
	printf("   of the opposing player's game board; [*] hit and [m] miss\n");
	printf("7. First player to guess the location of all ships wins\n\n");
}

/**
 * Function name : systemMessage ()
 * Date Created  : 17 October 2012
 * Usage         : systemMessage (char *);
 * Definition    : This function prints a message on the console and awaits
 *                 for user to press the enter key
 * Pre-condition : n/a
 * Post-condition: n/a
 */
void Production::systemMessage(const char* message) {
	char ch = '\0';

	do {
		cout << message;
	} while ((ch = getch()) != '\r');
}

