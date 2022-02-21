#pragma once
#include "Production.h"

class Tests
{
public:
	bool tests();
	bool testInitBoard();
	bool testPutShipOnBoard();
	bool testIsValidLocation();
	bool testPrintBoard();
	bool testConvertStringtoPosition();
	bool testManuallyPutShips();
	bool testCheckShots();
	bool testGetTarget();
	bool testIsWinner();
	bool testCheckWin();
	bool testGenerateRandom();

};

