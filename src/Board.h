#pragma once

#include "Cell.h"
#include "Watercraft.h"
#include "Coordinate.h"
#include "Stats.h"
#include "Production.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>

/* CONSTANT MACROS */

#define ROWS 10
#define COLS 10

#define LOG_FILE_NAME "battleship.log"

#define NUM_OF_SHIPS  5

#define HORIZONTAL    0
#define VERTICAL      1

#define PLAYER_ONE    0
#define PLAYER_TWO    1


#define WATER         '~'
#define HIT           '*'
#define MISS          'm'

#define CARRIER       'c'
#define BATTLESHIP    'b'
#define CRUISER       'r'
#define SUBMARINE     's'
#define DESTROYER     'd'

#define NORTH 0
#define SOUTH 1
#define WEST  2
#define EAST  3

/* ENUMERATED TYPES */
typedef enum {
	FALSE, 
	TRUE
} Boolean;

typedef enum {
	CARRIER_L = 5,
	BATTLESHIP_L = 4,
	CRUISER_L = 3,
	SUBMARINE_L = 3,
	DESTROYER_L = 2
} ShipType;

class Board
{

public:
	Cell** gameBoard;


	void         initializeGameBoard();
	void         printGameBoard(Boolean showPegs);
	void         putShipOnGameBoard(WaterCraft* ship, Coordinate* position, int direction);
	void         manuallyPlaceShipsOnGameBoard(WaterCraft** ship);
	void         randomlyPlaceShipsOnGameBoard(WaterCraft** ship);
	void         updateGameBoard(Coordinate* target);

	int          getRandomNumber(int lowest, int highest);
	Boolean      isValidLocation(Coordinate* position, int direction, int length);
	Boolean      convertStringtoPosition(Coordinate* position, char* stringPosition, int length);
	Coordinate*   generatePosition(int direction, int length);
	short        checkShot(Coordinate* target);
	Boolean      isWinner(Stats* players, int player);
	void		 checkWin(Stats* players);

	Coordinate*   getTarget();
	void         checkBoundsOfCardinal(Boolean cardinals[], int bound, int direction);
	Boolean      checkSunkShip(short sunkShip[][NUM_OF_SHIPS], short player, char shipSymbol, FILE* stream);
	void	     outputGameBoard(FILE* stream);
};

