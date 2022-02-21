#include "Tests.h"
#include "Production.h"
#include <iostream>
using namespace std;

bool Tests::tests()
{
	bool ok1 = testInitBoard();
	bool ok2 = testPutShipOnBoard();
	bool ok3 = testIsValidLocation();
	bool ok4 = testPrintBoard();
	bool ok5 = testConvertStringtoPosition();
	bool ok6 = testManuallyPutShips();
	bool ok7 = testCheckShots();
	bool ok8 = testGetTarget();
	bool ok9 = testIsWinner();
	bool ok10 = testCheckWin();
	bool ok11 = testGenerateRandom();

	bool ok = ok1 && ok2 && ok3 && ok4 && ok5 && ok6 && ok7 && ok8 && ok9 && ok10 && ok11;
	return ok;
}

bool Tests::testInitBoard()
{
	Board* bP = new Board();
	bP->initializeGameBoard();
	int i = 2, j = 3;
	bool okRow = (*(bP->gameBoard + ROWS * i + j))->position->row == i;
	bool okCol = (*(bP->gameBoard + ROWS * i + j))->position->column== j;
	bool okSym = (*(bP->gameBoard + ROWS * i + j))->symbol == '~';
	bool ok = okRow && okCol && okSym;
	if (ok)
	{
		cout << "Test initBoard passed!" << endl;
	}
	else cout << "Test initBoard did not pass" << endl;
	delete(bP);
	return ok;
}

bool Tests::testPutShipOnBoard()
{
	Board* bP = new Board();
	bP->initializeGameBoard();

	WaterCraft* carrier = new Carrier();
	Coordinate* position = new Coordinate();
	position->row = 1;
	position->column = 6;
	bP->putShipOnGameBoard(carrier,position, VERTICAL);
	bool ok = ((*(bP->gameBoard + ROWS * 1 + 6))->symbol == 'c') &&
		((*(bP->gameBoard + ROWS * 2 + 6))->symbol == 'c')&&
		((*(bP->gameBoard + ROWS * 3 + 6))->symbol == 'c')&&
		((*(bP->gameBoard + ROWS * 4 + 6))->symbol == 'c')&&
		((*(bP->gameBoard + ROWS * 5 + 6))->symbol == 'c');
	if (ok)
	{
		cout << "Test putShipOnBoard passed!" << endl;
	}
	else cout << "Test putShipOnBoard did not pass" << endl;
	delete(position);
	delete(carrier);
	delete(bP);
	return ok;
}

bool Tests::testIsValidLocation()
{
	Board* bP = new Board();
	bP->initializeGameBoard();

	Coordinate* position = new Coordinate();
	position->row = 1;
	position->column = 6;

	bool ok1 = bP->isValidLocation(position, HORIZONTAL, 3);
	bool ok2 = !(bP->isValidLocation(position, HORIZONTAL, 5));
	bool ok3 = bP->isValidLocation(position, VERTICAL, 3);

	bool ok = ok1 && ok2 && ok3;
	if (ok)
	{
		cout << "Test isValidLocation passed!" << endl;
	}
	else cout << "Test isValidLocation did not pass" << endl;
	delete(position);
	delete(bP);
	return ok;
}

bool Tests::testPrintBoard()
{
	Board* bP = new Board();
	bP->initializeGameBoard();

	WaterCraft* carrier = new Carrier();
	Coordinate* position = new Coordinate();
	position->row = 1;
	position->column = 6;
	bP->putShipOnGameBoard(carrier, position, VERTICAL);

	WaterCraft* battleship = new Battleship();
	position->row = 0;
	position->column = 1;
	bP->putShipOnGameBoard(battleship, position, HORIZONTAL);

	bP->printGameBoard(TRUE);
	int ok = -1;
	cout << "One carrier should be vertical from <1,6> to <5,6> " << endl;
	cout << "One battleship should be horizontal from <0,1> to <0,4>  " << endl;
	cout << "Is this correct? Type 0 for false and 1 for true: " << endl;
	cin >> ok;

	if (ok)
	{
		cout << "Test printBoard passed!" << endl;
	}
	else cout << "Test printBoard did not pass." << endl;

	delete(position);
	delete(battleship);
	delete(carrier);
	delete(bP);
	return ok;
}

bool Tests::testConvertStringtoPosition()
{
	Board* bP = new Board();
	bP->initializeGameBoard();

	Coordinate position[5];
	char stringPosition[9] = "23242526";
	int length = 4;

	bP->convertStringtoPosition(position, stringPosition, length);

	bool ok1 = position->row == 2 && position->column == 3;
	bool ok2 = (position + 1)->row == 2 && (position + 1)->column == 4;
	bool ok3 = (position + 2)->row == 2 && (position + 2)->column == 5;
	bool ok4 = (position + 3)->row == 2 && (position + 3)->column == 6;

	bool ok = ok1 && ok2 && ok3 && ok4;

	if (ok)
	{
		cout << "Test ConvertStringtoPosition passed!" << endl;
	}
	else cout << "Test ConvertStringtoPosition did not pass." << endl;

	delete(bP);
	return ok;
}

bool Tests::testManuallyPutShips()
{
	int a = -1;
	cout << "Would you like to do test ManuallyPutShips? Type 0 for no and 1 for yes" << endl;
	cin >> a;
	if (a)
	{
		Board* bP = new Board();
		bP->initializeGameBoard();

		WaterCraft* carrier = new Carrier();
		WaterCraft* battleship = new Battleship();
		WaterCraft* cruiser = new Cruiser();
		WaterCraft* submarine = new Submarine();
		WaterCraft* destroyer = new Destroyer();

		WaterCraft* ship[NUM_OF_SHIPS] =
		{
			carrier,
			battleship,
			cruiser,
			submarine,
			destroyer
		};

		bP->manuallyPlaceShipsOnGameBoard(ship);
		bP->printGameBoard(TRUE);

		int ok = -1;
		cout << "Does this look correct?" << endl;
		cin >> ok;
		if (ok)
		{
			cout << "Test ManuallyPutShips passed!" << endl;
		}
		else cout << "Test ManuallyPutShips did not pass." << endl;

		delete(carrier);
		delete(battleship);
		delete(cruiser);
		delete(destroyer);
		delete(submarine);
		delete(bP);

		return ok;
	}
	else return true;
}

bool Tests::testCheckShots()
{
	Board* bP = new Board();
	bP->initializeGameBoard();

	WaterCraft* carrier = new Carrier();
	Coordinate* position = new Coordinate();
	position->row = 1;
	position->column = 6;
	bP->putShipOnGameBoard(carrier, position, VERTICAL);

	bool ok1 = bP->checkShot(position) == 1;

	position->row = 1;
	position->column = 7;
	bool ok2 = bP->checkShot(position) == 0;

	bool ok = ok1 && ok2;
	if (ok)
	{
		cout << "Test CheckShots passed!" << endl;
	}
	else cout << "Test CheckShots did not pass." << endl;

	delete(position);
	delete(carrier);
	delete(bP);
	return ok;
}

bool Tests::testGetTarget()
{
	cout << "Please enter \"3 4\" for this test" << endl;
	Board* bP = new Board();
	bP->initializeGameBoard();

	Coordinate* target = bP->getTarget();
	bool ok = target->row == 3 && target->column == 4;

	if (ok)
	{
		cout << "Test GetTarget passed!" << endl;
	}
	else cout << "Test GetTarget did not pass." << endl;

	delete(bP);
	delete(target);
	return ok;
}

bool Tests::testIsWinner()
{
	Board* bP = new Board();

	Stats players[2] = {
		{17, 1, 18, 17},
		{1, 1, 2, 1}
	};
	bool ok1 = !(bP->isWinner(players, 1));
	bool ok2 = bP->isWinner(players, 0);

	bool ok = ok1 && ok2;
	if (ok)
	{
		cout << "Test IsWinner passed!" << endl;
	}
	else cout << "Test IsWinner did not pass." << endl;

	delete(bP);
	return ok;
}

bool Tests::testCheckWin() 
{
	int a = -1;
	cout << "Would you like to do test CheckWin? Type 0 for no and 1 for yes:" << endl;
	cin >> a;
	if (a) {
		Board* bP = new Board();

		Stats players1[2] = {
			{2, 1, 18, 17},
			{1, 1, 2, 1}
		};
		cout << "Two players are made, player one has 2 hits and player 2 has 1 hit" << endl;
		cout << "It should return: Player 1 won by hitting more targets!" << endl;
		bool ok1 = -1;
		bP->checkWin(players1);
		cout << "Is the output correct? Type 0 for no and 1 for yes:" << endl;
		cin >> ok1;


		Stats players2[2] = {
			{2, 1, 18, 17},
			{13, 1, 2, 1}
		};
		cout << "Two players are made, player one has 2 hits and player 2 has 13 hits" << endl;
		cout << "It should return: Player 2 won by hitting more targets!" << endl;
		bool ok2 = -1;
		bP->checkWin(players2);
		cout << "Is the output correct? Type 0 for no and 1 for yes:" << endl;
		cin >> ok2;

		Stats players3[2] = {
			{13, 1, 18, 17},
			{13, 1, 2, 1}
		};
		cout << "Two players are made, player one has 13 hits and player 2 has 13 hits" << endl;
		cout << "It should return: It was a tie! You both hit 13 targets." << endl;
		bool ok3 = -1;
		bP->checkWin(players3);
		cout << "Is the output correct? Type 0 for no and 1 for yes:" << endl;
		cin >> ok3;

		bool ok = ok1 && ok2 && ok3;

		if (ok)
		{
			cout << "Test CheckWin passed!" << endl;
		}
		else cout << "Test CheckWin did not pass." << endl;

		delete(bP);
		return ok;
	}
	else
		return true;
}

bool Tests::testGenerateRandom()
{
	Board* bP = new Board();
	int a = bP->getRandomNumber(0, 4);
	int b = bP->getRandomNumber(0, 5);
	int c = bP->getRandomNumber(0, 9);
	int d = bP->getRandomNumber(0, 4);

	cout << a;
	cout << b;
	cout << c;
	cout << d << endl;

	bool ok1 = a <= 4 && a >= 0;
	bool ok2 = b <= 5 && b >= 0;
	bool ok3 = c <= 9 && c >= 0;
	bool ok4 = d <= 4 && c >= 0;

	bool ok = ok1 && ok2 && ok3 && ok4;
	if (ok)
	{
		cout << "Test GenerateRandom passed!" << endl;
	}
	else cout << "Test GenerateRandom did not pass." << endl;

	delete(bP);
	return ok;
}
