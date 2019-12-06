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


void displayBoard(char board[][BOARD_HEIGHT], int width, int height)
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
					cout << " " << board[w - 1][(h / 2) - 1] << " |";
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

void displayMines(bool mines[][BOARD_HEIGHT], int width, int height)
{
	char mineDisplay[BOARD_WIDTH][BOARD_HEIGHT];
	// set arrays
	for (int w = 0; w < BOARD_WIDTH; w++)
	{
		for (int h = 0; h < BOARD_HEIGHT; h++)
		{
			if (mines[w][h])
				mineDisplay[w][h] = '*';
			else
				mineDisplay[w][h] = ' ';
		}
	}

	displayBoard(mineDisplay, BOARD_WIDTH, BOARD_HEIGHT);
	cout << endl << "You hit a mine! Game over." << endl;
}

bool hasPlayerWon(char board[][BOARD_HEIGHT], bool mines[][BOARD_HEIGHT], int width, int height)
{
	//check the number of ? to see if play has won
	int numberOfQuestionMarks = 0;
	for (int w = 0; w < BOARD_WIDTH; w++)
	{
		for (int h = 0; h < BOARD_HEIGHT; h++)
		{
			if (board[w][h] == '?')
				numberOfQuestionMarks++;
		}
	}

	return numberOfQuestionMarks == 10;
}


int main(int argc, char* argv[])
{
	//set the variables for the game 
	bool mines[BOARD_WIDTH][BOARD_HEIGHT];
	char board[BOARD_WIDTH][BOARD_HEIGHT];
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
			mines[w][h] = false;
			board[w][h] = '?';
		}
	}
	

	

	// get the random h/w coordinates for mines 
	int wCoordinate;
	int hCoordinate;
	int numberOfMines = 0;

	do {
		wCoordinate = rand() % 12;
		hCoordinate = rand() % 9;
		if (mines[wCoordinate][hCoordinate] == false)
		{
			numberOfMines++;
			mines[wCoordinate][hCoordinate] = true;
		}
	} while (numberOfMines < 10);




	// start game
	do
	{ 
		//user input checks and 0 base the input
		displayBoard(board, BOARD_WIDTH, BOARD_HEIGHT);
		cout << "Dig at X: ";
		cin >> userWidth;
		cout << endl;
		cout << "Dig at Y: ";
		cin >> userHeight;
		cout << endl;

		userWidth--;
		userHeight--;

		if (mines[userWidth][userHeight])
		{
			displayMines(mines, BOARD_WIDTH, BOARD_HEIGHT);
			return 0;
		}

		if (board[userWidth][userHeight] != '?'
			|| userWidth < 0 || userWidth>11 || userHeight < 0 || userHeight>8)
		{
			cout << "Invlalid input, please try again..." << endl;
			continue;
		}

		board[userWidth][userHeight] = 'c';
		
		//Checks the spaces for mines or clears if non
		do
		{
			checkAgain = false;

			for (int i = 0; i < BOARD_WIDTH; i++)
			{
				for (int j = 0; j < BOARD_HEIGHT; j++)
				{
					if (board[i][j] == 'c')
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

								if (mines[adjWidth][adjHeight] == true)
								{
									mineCount++;
								}
							}
						}
						if (mineCount == 0)
						{
							board[i][j] = ' ';
							for (int k = -1; k <= 1; k++)
							{
								for (int m = -1; m <= 1; m++)
								{
									int adjWidth = i + k;
									int adjHeight = j + m;
									if (adjWidth<0 || adjWidth>BOARD_WIDTH - 1 || adjHeight<0 || adjHeight>BOARD_HEIGHT - 1)
										continue;

									if (board[adjWidth][adjHeight] == '?')
									{
										board[adjWidth][adjHeight] = 'c';
										checkAgain = true;

									}
								}
							}
						}
						else
						{
							board[i][j] = '0' + mineCount;
						}
					}
				}
			}
			if (hasPlayerWon(board, mines, BOARD_WIDTH, BOARD_HEIGHT))
			{
				cout << "You Won!!!"<<endl;
				return 0;
			}

		} while (checkAgain);
	} while (true);
}