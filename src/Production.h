#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>

#include "Board.h"
#include "Cell.h"
#include "Coordinate.h"
#include "Stats.h"
#include "WaterCraft.h"
#include "Battleship.h"
#include "Carrier.h"
#include "Cruiser.h"
#include "Destroyer.h"
#include "Submarine.h"

class Production
{
public:
	bool		 prod(int argc, char* argv[]);
	void         welcomeScreen(void);
	void         systemMessage(const char* message);

};

