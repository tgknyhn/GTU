#include <iostream>
#include "hexgame.h"
using namespace std;

int main()
{
	char board[MAX_SIZE][MAX_SIZE];
	char player_or_computer;
	char column;

	int size;
	int row;


	bool user = 0;

	initialize_game(size, player_or_computer);

	auto sizeOfBoard = size; 

	initialize_board(board, sizeOfBoard);
	printBoard(board, sizeOfBoard);
	
	bool pveORpvp = player_or_computer-'0';

	while(isGameOver(board, sizeOfBoard, pveORpvp) == false)
	{
		if(pveORpvp == 0)
		{
			if(askUser(board, row, column, user, sizeOfBoard, pveORpvp) == true)
				insertTo(row, column, board, user, sizeOfBoard);			

			if(user == 0)
				user = 1;
			else
				user = 0;
		}
		else
		{
			if(user == 0)
			{
				if(askUser(board, row, column, user, sizeOfBoard, pveORpvp) == true)
					insertTo(row, column, board, user, sizeOfBoard);
			}
			else
			{
				askComputer(board, row, column, sizeOfBoard);
				insertTo(row, column, board, user, sizeOfBoard);
			}

			if(user == 0)
				user = 1;
			else
				user = 0;
		}
	}

	return 0;
}
