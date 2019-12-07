/*
Christopher Custer
Assignment 10
Program Description: Making Minesweeper with functions and multidimesional arrays.

*/

#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

const int BOARD_WIDTH = 12;
const int BOARD_HEIGHT = 9;


struct tile
{
	char letter;
	bool hasMine;
};

struct minesweeper
{
	tile tiles[BOARD_WIDTH][BOARD_HEIGHT];
	int width, height, mineCount, flagCount, unrevealedCount, moves;
};

minesweeper ms;

void displayBoard(minesweeper ms)
{
	// adjusting height for extra rows to display
	for (int h = 0; h < BOARD_HEIGHT * 2 + 2; h++)
	{
		for (int w = 0; w < BOARD_WIDTH + 1; w++)
		{
			// top row
			if (h == 0)
			{
				if (w == 0)
					cout << "     " << w + 1 << "  ";
				else if (w < 9)
					cout << " " << w + 1 << "  ";
				else if (w < BOARD_WIDTH)
					cout << w + 1 << "  ";
			}
			else if (h % 2 == 0)
			{
				if (w == 0)
					cout << " " << h / 2 << " |";
				else
					cout << " " << ms.tiles[w - 1][(h / 2) - 1].letter << " |";
			}
			else // odd row
			{
				if (w == 0)
					cout << "   +";
				else
					cout << "---+";

			}
			if (w == BOARD_WIDTH)
				cout << endl;
		}
	}


}

void displayMines(minesweeper ms)
{
	tile mineDisplay[BOARD_WIDTH][BOARD_HEIGHT];
	// set arrays
	for (int w = 0; w < BOARD_WIDTH; w++)
	{
		for (int h = 0; h < BOARD_HEIGHT; h++)
		{
			if (ms.tiles[w][h].hasMine)
				mineDisplay[w][h].letter = '*';
			else
				mineDisplay[w][h].letter = ' ';
		}
	}

	displayBoard(ms);
	cout << endl << "You hit a mine! Game over." << endl;
}

bool hasPlayerWon(minesweeper ms)
{
	//check the number of ? to see if play has won
	int numberOfQuestionMarks = 0;
	for (int w = 0; w < BOARD_WIDTH; w++)
	{
		for (int h = 0; h < BOARD_HEIGHT; h++)
		{
			if (ms.tiles[w][h].letter == '?')
				numberOfQuestionMarks++;
		}
	}

	return numberOfQuestionMarks == 10;
}


int main(int argc, char* argv[])
{
	//set the variables for the game 
	bool checkAgain = false;
	int userWidth = 0;
	int userHeight = 0;

	// if argc>1 debug seed 0 in rand, else use time
	if (argc > 1)
	{
		srand(0);
	}
	else
	{
		srand(time(NULL));
	}



	// set arrays
	for (int w = 0; w < BOARD_WIDTH; w++)
	{
		for (int h = 0; h < BOARD_HEIGHT; h++)
		{
			ms.tiles[w][h].hasMine = false;
			ms.tiles[w][h].letter = '?';
		}
	}

	// get the random h/w coordinates for mines 
	int wCoordinate;
	int hCoordinate;
	int numberOfMines = 0;

	do {
		wCoordinate = rand() % 12;
		hCoordinate = rand() % 9;
		if (ms.tiles[wCoordinate][hCoordinate].hasMine == false)
		{
			numberOfMines++;
			ms.tiles[wCoordinate][hCoordinate].hasMine = true;
		}
	} while (numberOfMines < 10);


	ms.moves = 1;
	char userDecision = ' ';
	// start game
	do
	{
		
		userDecision = ' ';
		//user input checks and 0 base the input
		displayBoard(ms);

		cout << "Move #: " << ms.moves << endl;
		cout << "[D]ig up Tile" << endl;
		cout << "[F]lag/Unflag Tile. " << ms.flagCount << " flag(s) placed." << endl;
		cout << "[Q]uit" << endl;
		cout << "Choose an action: ";
		cin >> userDecision;
		cout << endl;

		switch (userDecision)
		{
		case 'd':
		case 'D':
			cout << "Dig at... \nRow: ";
			cin >> userWidth;
			cout << endl;
			cout << "Column: ";
			cin >> userHeight;
			cout << endl;

			userWidth--;
			userHeight--;

			if (ms.tiles[userWidth][userHeight].hasMine)
			{
				displayMines(ms);
				return 0;
			}

			if (ms.tiles[userWidth][userHeight].letter != '?'
				|| userWidth < 0 || userWidth>11 || userHeight < 0 || userHeight>8)
			{
				cout << "Invlalid input, please try again..." << endl;
				continue;
			}

			ms.tiles[userWidth][userHeight].letter = 'c';

			//Checks the spaces for mines or clears if none
			do
			{
				checkAgain = false;

				for (int i = 0; i < BOARD_WIDTH; i++)
				{
					for (int j = 0; j < BOARD_HEIGHT; j++)
					{
						if (ms.tiles[i][j].letter == 'c')
						{
							int mineCount = 0;
							for (int k = -1; k <= 1; k++)
							{
								for (int m = -1; m <= 1; m++)
								{
									int adjWidth = i + k;
									int adjHeight = j + m;
									if (adjWidth<0 || adjWidth>BOARD_WIDTH - 1 || adjHeight<0 || adjHeight>BOARD_HEIGHT - 1)
										continue;

									if (ms.tiles[adjWidth][adjHeight].hasMine == true)
									{
										mineCount++;
									}
								}
							}
							if (mineCount == 0)
							{
								ms.tiles[i][j].letter = ' ';
								for (int k = -1; k <= 1; k++)
								{
									for (int m = -1; m <= 1; m++)
									{
										int adjWidth = i + k;
										int adjHeight = j + m;
										if (adjWidth<0 || adjWidth>BOARD_WIDTH - 1 || adjHeight<0 || adjHeight>BOARD_HEIGHT - 1)
											continue;

										if (ms.tiles[adjWidth][adjHeight].letter == '?')
										{
											ms.tiles[adjWidth][adjHeight].letter = 'c';
											checkAgain = true;

										}
									}
								}
							}
							else
							{
								ms.tiles[i][j].letter = '0' + mineCount;
							}
						}
					}
				}
				if (hasPlayerWon(ms))
				{
					cout << "You Won!!!" << endl;
					return 0;
				}
				ms.moves++;
			} while (checkAgain);
			break;
		case 'f':
		case 'F':
			cout << "Place Flag at... \nRow: ";
			cin >> userWidth;
			cout << endl;
			cout << "Column: ";
			cin >> userHeight;
			cout << endl;

			userWidth--;
			userHeight--;

			if (ms.tiles[userWidth][userHeight].letter == '?'
				&& userWidth > 0 && userWidth < 11 && userHeight > 0 && userHeight < 8)
			{
				ms.tiles[userWidth][userHeight].letter = 'P';
				ms.flagCount++;
			}
			else if (ms.tiles[userWidth][userHeight].letter == 'P'
				&& userWidth > 0 && userWidth < 11 && userHeight > 0 && userHeight < 8)
			{
				ms.tiles[userWidth][userHeight].letter = '?';
				ms.flagCount--;
			}
			else
			{
				cout << "Invalid Response. Please try again." << endl;
			}
			break;

		case 'q':
		case 'Q':
			cout << "Quitting..." << endl;
			return 0;

		default:
			cout << "Invalid Response. Please enter a value in the []." << endl;
			continue;
		}


	} while (true);
}