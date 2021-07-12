#include <iostream>
#include "hexgame.h"
using namespace std;

int main()
{
	char board[MAX_SIZE][MAX_SIZE];
	char player_or_computer;
	char column;

	int  sizeOfBoard;
	int  row;

	bool user = 0;

	initialize_game(&sizeOfBoard, &player_or_computer);
	initialize_board(board, sizeOfBoard);

	while(isGameOver(board, sizeOfBoard, (player_or_computer-'0')) == false)
	{
		if(player_or_computer == '0')
		{
			askUser(board, &row, &column, user, sizeOfBoard);
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
				askUser(board, &row, &column, user, sizeOfBoard);
				insertTo(row, column, board, user, sizeOfBoard);
			}
			else
			{
				askComputer(board, &row, &column, sizeOfBoard);
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
