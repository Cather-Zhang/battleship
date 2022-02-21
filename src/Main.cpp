/*==============================================================================
 | Filename: main.c
 | Programmer: Gabriel V. de a Cruz Jr.
 | Class: CptS 121, Fall 2012 ; Lab Section 7
 | Programming Assignment 4: Basic Game of Battleship
 | Date: October 17, 2012
 +------------------------------------------------------------------------------
 | Description: This program simulates the game of Battleship. The game will be
 |              completely text-based. Battleship is a two player Navy game.
 |              The objective of the game is to sink all ships in your enemy's
 |              fleet. The Player to sink his/her enemy's fleet first wins. Both
 |              players' fleets consist of 5 ships that are hidden from the
 |              enemy. Each ship may be differentiated by its "size" (besides
 |              the Cruiser and Submarine) or number of cells it expands on the
 |              game board. The Carrier has 5 cells, Battleship has 4 cells,
 |              Cruiser has 3 cells, Submarine has 3 cells, and the Destroyer
 |              has 2 cells.
 ==============================================================================*/


#include "Production.h"
#include "Tests.h"
#include <iostream>

int main(int argc, char* argv[]) {
	srand((unsigned int)time(NULL));
	std::cout << "!!!Hello World,  we are on HW6!!!" << std::endl; // prints !!!Hello World!!!
	Tests* tsP = new Tests();
	if (tsP->tests())
	{
		std::cout << "All tests passed!" << std::endl;

		Production* pP = new Production();
		if (pP->prod(argc, argv))
		{
			std::cout << "Production passed." << std::endl;
		}
		delete(pP);
			
	}
	else
	{
		std::cout << "Not all tests passed." << std::endl;
	}
	delete(tsP);
	return 0;
}
