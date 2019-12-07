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

void displayBoard(tile tiles[][BOARD_HEIGHT], int width, int height)
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
					cout << " " << tiles[w - 1][(h / 2) - 1].letter << " |";
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

void displayMines(tile tiles[][BOARD_HEIGHT], int width, int height)
{
	tile mineDisplay[BOARD_WIDTH][BOARD_HEIGHT];
	// set arrays
	for (int w = 0; w < BOARD_WIDTH; w++)
	{
		for (int h = 0; h < BOARD_HEIGHT; h++)
		{
			if (tiles[w][h].hasMine)
				mineDisplay[w][h].letter = '*';
			else
				mineDisplay[w][h].letter = ' ';
		}
	}

	displayBoard(mineDisplay, BOARD_WIDTH, BOARD_HEIGHT);
	cout << endl << "You hit a mine! Game over." << endl;
}

bool hasPlayerWon(tile tiles[][BOARD_HEIGHT], int width, int height)
{
	//check the number of ? to see if play has won
	int numberOfQuestionMarks = 0;
	for (int w = 0; w < BOARD_WIDTH; w++)
	{
		for (int h = 0; h < BOARD_HEIGHT; h++)
		{
			if (tiles[w][h].letter == '?')
				numberOfQuestionMarks++;
		}
	}

	return numberOfQuestionMarks == 10;
}


int main(int argc, char* argv[])
{
	//set the variables for the game 
	tile tiles[BOARD_WIDTH][BOARD_HEIGHT];

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
			tiles[w][h].hasMine = false;
			tiles[w][h].letter = '?';
		}
	}

	// get the random h/w coordinates for mines 
	int wCoordinate;
	int hCoordinate;
	int numberOfMines = 0;

	do {
		wCoordinate = rand() % 12;
		hCoordinate = rand() % 9;
		if (tiles[wCoordinate][hCoordinate].hasMine == false)
		{
			numberOfMines++;
			tiles[wCoordinate][hCoordinate].hasMine = true;
		}
	} while (numberOfMines < 10);


	ms.moves = 1;
	char userDecision = ' ';
	// start game
	do
	{
		
		userDecision = ' ';
		//user input checks and 0 base the input
		displayBoard(tiles, BOARD_WIDTH, BOARD_HEIGHT);

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

			if (tiles[userWidth][userHeight].hasMine)
			{
				displayMines(tiles, BOARD_WIDTH, BOARD_HEIGHT);
				return 0;
			}

			if (tiles[userWidth][userHeight].letter != '?'
				|| userWidth < 0 || userWidth>11 || userHeight < 0 || userHeight>8)
			{
				cout << "Invlalid input, please try again..." << endl;
				continue;
			}

			tiles[userWidth][userHeight].letter = 'c';

			//Checks the spaces for mines or clears if none
			do
			{
				checkAgain = false;

				for (int i = 0; i < BOARD_WIDTH; i++)
				{
					for (int j = 0; j < BOARD_HEIGHT; j++)
					{
						if (tiles[i][j].letter == 'c')
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

									if (tiles[adjWidth][adjHeight].hasMine == true)
									{
										mineCount++;
									}
								}
							}
							if (mineCount == 0)
							{
								tiles[i][j].letter = ' ';
								for (int k = -1; k <= 1; k++)
								{
									for (int m = -1; m <= 1; m++)
									{
										int adjWidth = i + k;
										int adjHeight = j + m;
										if (adjWidth<0 || adjWidth>BOARD_WIDTH - 1 || adjHeight<0 || adjHeight>BOARD_HEIGHT - 1)
											continue;

										if (tiles[adjWidth][adjHeight].letter == '?')
										{
											tiles[adjWidth][adjHeight].letter = 'c';
											checkAgain = true;

										}
									}
								}
							}
							else
							{
								tiles[i][j].letter = '0' + mineCount;
							}
						}
					}
				}
				if (hasPlayerWon(tiles, BOARD_WIDTH, BOARD_HEIGHT))
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

			if (tiles[userWidth][userHeight].letter == '?'
				&& userWidth > 0 && userWidth < 11 && userHeight > 0 && userHeight < 8)
			{
				tiles[userWidth][userHeight].letter = 'P';
				ms.flagCount++;
			}
			else if (tiles[userWidth][userHeight].letter == 'P'
				&& userWidth > 0 && userWidth < 11 && userHeight > 0 && userHeight < 8)
			{
				tiles[userWidth][userHeight].letter = '?';
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