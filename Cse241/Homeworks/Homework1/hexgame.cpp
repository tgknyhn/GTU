#include <iostream>
#include "hexgame.h"

using namespace std;

//game utilities
void initialize_game (int * sizeOfBoard, char * player_or_computer);
void initialize_board(char board[MAX_SIZE][MAX_SIZE], int sizeOfBoard);
void printBoard      (char board[MAX_SIZE][MAX_SIZE], int sizeOfBoard);

//user1 or user2 moves
void askUser         (char board[MAX_SIZE][MAX_SIZE], int * row, char * column, bool user, int sizeOfBoard);
void insertTo        (int row, char column, char board[MAX_SIZE][MAX_SIZE], bool user, int sizeOfBoard);
void deleteRoute     (int * routeX, int * routeY);
void searchRouteU1   (char board[MAX_SIZE][MAX_SIZE], int * routeX, int * routeY, int currIndex, bool * flag, int sizeOfBoard);
void searchRouteU2   (char board[MAX_SIZE][MAX_SIZE], int * routeX, int * routeY, int currIndex, bool * flag, int sizeOfBoard, bool user);

//computer moves
void askComputer     (char board[MAX_SIZE][MAX_SIZE], int * row, char * column, int sizeOfBoard);
void findLastMove    (char board[MAX_SIZE][MAX_SIZE], char letter, int * routeX, int * routeY, int & currX, int & currY);
bool isNeighborsEmpty(char board[MAX_SIZE][MAX_SIZE], int x, int y, int neighbors[6]);
void goStraightO     (char board[MAX_SIZE][MAX_SIZE], int * row, char * column);
void placeToNeighbor (int x, int y, int * row, char * column, int neighbor);

//condition check or basic task functions
void convertToUpper  (char board[MAX_SIZE][MAX_SIZE], int * routeX, int * routeY, bool user);
int  countMoves      (char board[MAX_SIZE][MAX_SIZE], char letter);
int  countLetter     (char board[MAX_SIZE][MAX_SIZE], char letter);
bool checkNeighbors  (char board[MAX_SIZE][MAX_SIZE], int * routeX, int * routeY, int currIndex, int neighbor, char letter);
bool isGameOver      (char board[MAX_SIZE][MAX_SIZE], int sizeOfBoard, bool user);
bool isEmpty         (char board[MAX_SIZE][MAX_SIZE], int x, int y);
bool isOnRoute       (int x, int y, int * routeX, int * routeY, int currIndex);
bool isPathCompleted (int * routeX, int * routeY, int user, int lastMove, int lastXorY);
bool isInputValid    (char x, int y, int sizeOfBoard);

// ------ Game Utilities -------

//asks size and who to play against then fills the board according to size
void initialize_game(int * sizeOfBoard, char * player_or_computer)
{
	do
	{
		cout << "\nWelcome to the game HEX!\n"
		     << "Please enter size of the board(6-12): ";
	
		cin >> * sizeOfBoard;
	
		cout << "Who you want to play against? (0 = player2, 1 = computer): ";

		cin >> * player_or_computer;

		if(  (*sizeOfBoard > 12 || *sizeOfBoard < 6)  )
			cerr << "\nInvalid value for size!\n";


		else if (  (*player_or_computer != '1' && *player_or_computer != '0')  )
			cerr << "\nInvalid value for opponent!\n";

	} while (  (*sizeOfBoard > 12 || *sizeOfBoard < 6) || (*player_or_computer != '1' && *player_or_computer != '0')  );
}

//fills board with dots. also puts letters into collumns.
void initialize_board(char board[MAX_SIZE][MAX_SIZE], int sizeOfBoard)
{
	char column[MAX_SIZE] = {' ', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l'};

	for(int i=0; i<=sizeOfBoard; i++)
		for(int j=0; j<=sizeOfBoard; j++)
			board[i][j] = '0';

	for(int i=0; i<=sizeOfBoard; i++)
		board[0][i] = column[i];
	for(int i=1; i<=sizeOfBoard; i++)
		for(int j=1; j<=sizeOfBoard; j++)
			board[i][j] = '.';


	printBoard(board, sizeOfBoard);
}

//just prints out the board nothing fancy
void printBoard(char board[MAX_SIZE][MAX_SIZE], int sizeOfBoard)
{
	cout << "  ";
	
	for(int y=0; y<=sizeOfBoard; y++)
	{
		if(y > 0 && y < 10)
			cout << y << ' ';
		else if(y > 9)
			cout << y;
		else
			cout << " ";
		

		for(int blank = 0; blank<y; blank++)
			cout << " ";	

		for(int x=1; x<=sizeOfBoard; x++)
			cout << board[y][x] << " " ;

		cout << endl;
	}
}


// ------- User1 or User2 Moves -------


//asks to user where to place their move
void askUser(char board[MAX_SIZE][MAX_SIZE], int * row, char * column, bool user, int sizeOfBoard)
{
	cout << "(user" << user+1 << "'s turn) " << "|| move to -> ";

	cin >> *column >> *row;

	int x = *column - 64; // ex: a == 97(ASCII value)
	int y = *row;

	if(isEmpty(board, x, y) == false)
	{
		cerr << "This place is not empty! Choose somewhere else \n";
		askUser(board, row, column, user, sizeOfBoard);
		return;
	}
		
	else if(isInputValid(*column, *row, sizeOfBoard) == false)
	{
		cerr << "Invalid input! (valid ex: B3)\n";
		askUser(board, row, column, user, sizeOfBoard);
		return;
	}

	else
		cout << "\n--> user" << user+1 << " placed to " << *column << *row << " <--\n" << endl;
	
	*column = *column + 32; // ex: 'A' -> 'a'
}

//inserts user's input to board array then prints the board.
void insertTo(int row, char column, char board[MAX_SIZE][MAX_SIZE], bool user, int sizeOfBoard)
{
	for(int i=0; i<=sizeOfBoard; i++)
		if(column == board[0][i])
		{
			if(user == 0)
				board[row][i] = 'x';	
			else
				board[row][i] = 'o';			
		}

	printBoard(board, sizeOfBoard);
}

//to refresh the route before following another path.
void deleteRoute(int * routeX, int * routeY)
{
	for(int i=0; i<MAX_POSSIBLE_MOVE; i++)
	{
		routeX[i] = 0; 
		routeY[i] = 0;
	}
}

//searchs for the 'x' value around the neighbors until all path completes.
void searchRouteU1(char board[MAX_SIZE][MAX_SIZE], int * routeX, int * routeY, int currIndex, bool * flag, int sizeOfBoard)
{
	if(isPathCompleted(routeX, routeY, 0, currIndex-1, sizeOfBoard) == true)
	{
		convertToUpper(board, routeX, routeY, 0);
		cout << "\n-------------------------------------------\n" << endl;
		printBoard(board, sizeOfBoard);
		cout << "\nUser1 won the game!\n\n";
		*flag = 1;
	}

	//only for first 'x' location searchs every y value.
	else if(currIndex == 1)
	{
		for(int y=1; y<=sizeOfBoard; y++)
		{
			deleteRoute(routeX, routeY);
			if(board[y][currIndex] == 'x')
			{
				routeX[currIndex] = 1;
				routeY[currIndex] = y;
			
				searchRouteU1(board, routeX, routeY, currIndex+1, flag, sizeOfBoard);
			}
		}
	}
	
	//recursively calls searchRouteU1 function until move count reachs MAX_POSSIBLE_MOVE and looks for nearby 'x' values. 
	else if(currIndex > 1 && currIndex < (sizeOfBoard*sizeOfBoard)/2)
	{
		for(int i=1; i<=6; i++)
			if(checkNeighbors(board, routeX, routeY, currIndex, i, 'x') == true)
				searchRouteU1 (board, routeX, routeY, currIndex+1, flag, sizeOfBoard);		
	}

}

void searchRouteU2(char board[MAX_SIZE][MAX_SIZE], int * routeX, int * routeY, int currIndex, bool * flag, int sizeOfBoard, bool user)
{
	if(isPathCompleted(routeX, routeY, 1, currIndex-1, sizeOfBoard) == true)
	{
		convertToUpper(board, routeX, routeY, 1);
		cout << "\n-------------------------------------------\n" << endl;
		printBoard(board, sizeOfBoard);
		if(user == 0)
			cout << "\nUser2 won the game!\n\n";
		else if(user == 1)
			cout << "\nComputer won the game!\n\n";
	
		*flag = 1;
	}

	//only for first 'x' location searchs every y value.
	else if(currIndex == 1)
	{
		for(int x=1; x<=sizeOfBoard; x++)
		{
			deleteRoute(routeX, routeY);
			if(board[currIndex][x] == 'o')
			{
				routeX[currIndex] = x;
				routeY[currIndex] = 1;
			
				searchRouteU2(board, routeX, routeY, currIndex+1, flag, sizeOfBoard, user);
			}
		}
	}
	
	//recursively calls searchRouteU2 function until move count reachs MAX_POSSIBLE_MOVE and looks for nearby 'y' values. 
	else if(currIndex > 1 && currIndex < (sizeOfBoard*sizeOfBoard)/2)
	{
		for(int i=1; i<=6; i++)
			if(checkNeighbors(board, routeX, routeY, currIndex, i, 'o') == true)
				searchRouteU2 (board, routeX, routeY, currIndex+1, flag, sizeOfBoard, user);		
	}
}


//  ------ Computer Moves ------ 


int countMoves(char board[MAX_SIZE][MAX_SIZE], char letter)
{
	int count = 0;

	for(int i=1; i<MAX_SIZE; i++)
		for(int j=1; j<MAX_SIZE; j++)
			if(board[i][j] == letter)
				count++;

	return count;
}

//ex:(sizeofboard == 6) func starts from A1 and goes to F6 step by step.
//if finds any selected letter on the route then looks if func picked this letter before or not. 
//if not, func assigns x and y coords to currX - currY - routeX - routeY. also increments index_route by 1.
//if it it is picked before then skips that letter and keeps searching throughout the board.
void findLastMove(char board[MAX_SIZE][MAX_SIZE], char letter, int * routeX, int * routeY, int & currX, int & currY)
{
	static int index_route = 0;

	for(int y=1; y<MAX_SIZE; y++)
		for(int x=1; x<MAX_SIZE; x++)
			if(board[y][x] == letter && isOnRoute(x, y, routeX, routeY, countMoves(board, letter)) == false)
			{
				routeX[index_route] = x;
				routeY[index_route] = y; 
				currX = x;
				currY = y;
				index_route++;
			}
}

//if neighbors are empty returns true and fills the array, else returns false;
bool isNeighborsEmpty(char board[MAX_SIZE][MAX_SIZE], int x, int y, int neighbors[6])
{
	int index = 0;
	int flag = 0;

	if(board[y][x-1] == '.')
	{
		neighbors[index] = 1;
		index++;
		flag = 1;
	}
	if(board[y][x+1] == '.')
	{
		neighbors[index] = 6;
		index++;
		flag = 1;
	}
	if(board[y+1][x-1] == '.')
	{
		neighbors[index] = 2;
		index++;
		flag = 1;
	}
	if(board[y-1][x+1] == '.')
	{
		neighbors[index] = 5;
		index++;
		flag = 1;
	}
	if(board[y-1][x] == '.')
	{
		neighbors[index] = 3;
		index++;
		flag = 1;
	}
	if(board[y+1][x] == '.')
	{
		neighbors[index] = 4;
		index++;
		flag = 1;
	}

	return flag;
}

void goStraightO(char board[MAX_SIZE][MAX_SIZE], int * row, char * column)
{
	for(int x=1; x<MAX_SIZE; x++)
		for(int y=1; y<MAX_SIZE; y++)
			if(isEmpty(board, x, y) == true)
			{
				*column = x + 64;
				*row    = y;
				cout << x << y << endl;
			}
}

void placeToNeighbor(int x, int y, int * row, char * column, int neighbor)
{
	switch(neighbor)
	{
		case 1:
			*column = x + 64 - 1;
			*row    = y;
			break;
		case 2:
			*column = x + 64 - 1;
			*row    = y + 1;
			break;
		case 3:
			*column = x + 64;
			*row    = y - 1;
			break;
		case 4:
			*column = x + 64;
			*row    = y + 1;
			break;
		case 5:
			*column = x + 64 + 1;
			*row    = y - 1;
			break;
		case 6:
			*column = x + 64 + 1;
			*row    = y;
			break;
	}
}

//computer move depends on how many 'x' are there on the board. 
void askComputer(char board[MAX_SIZE][MAX_SIZE], int * row, char * column, int sizeOfBoard)
{
	static int routeX[MAX_POSSIBLE_MOVE] = {0};
	static int routeY[MAX_POSSIBLE_MOVE] = {0};
	int x, y;
	int empty_neighbors[6];

	cout << "(Computer's turn)\n";

	findLastMove(board, 'x', routeX, routeY, x, y);

	if(isNeighborsEmpty(board, x, y, empty_neighbors) == true)
		placeToNeighbor(x, y, row, column, empty_neighbors[0]);
	else
		goStraightO(board, row, column);

	cout << "\n--> computer placed to " << *column << *row << " <--\n" << endl;

	*column = *column + 32; // ex: 'A' -> 'a'
}


// ------- Condition check or basic task functions --------


void convertToUpper(char board[MAX_SIZE][MAX_SIZE], int * routeX, int * routeY, bool user)
{
	if(user == 0)
		for(int i=1; i<MAX_POSSIBLE_MOVE; i++)
			board[routeY[i]][routeX[i]] = 'X';

	else if(user == 1)
		for(int i=1; i<MAX_POSSIBLE_MOVE; i++)
			board[routeY[i]][routeX[i]] = 'O';
}

//checking every neighbor, if they have 'x' this time checks if that neighbor already is on the route.
//if not then adds that 'x' coords to route. 
bool checkNeighbors(char board[MAX_SIZE][MAX_SIZE], int * routeX, int * routeY, int currIndex, int neighbor, char letter)
{
	int flag = 1;
	//																		Ex:  (for B5)																						
	//neighbors 				             								    column  row
	int n1_y = routeY[currIndex-1]     ; int n1_x = routeX[currIndex-1] - 1; //   A      5 
	int n2_y = routeY[currIndex-1] + 1 ; int n2_x = routeX[currIndex-1] - 1; //   A      6
	int n3_y = routeY[currIndex-1] - 1 ; int n3_x = routeX[currIndex-1]    ; //   B      4
	int n4_y = routeY[currIndex-1] + 1 ; int n4_x = routeX[currIndex-1]    ; //   B      6
	int n5_y = routeY[currIndex-1] - 1 ; int n5_x = routeX[currIndex-1] + 1; //   C      4
	int n6_y = routeY[currIndex-1]     ; int n6_x = routeX[currIndex-1] + 1; //   C      5

	if     (board[n1_y][n1_x] == letter && isOnRoute(n1_x, n1_y, routeX, routeY, currIndex) == false && neighbor == 1)
	{
		routeY[currIndex] = n1_y;
		routeX[currIndex] = n1_x;
	}
	else if(board[n2_y][n2_x] == letter && isOnRoute(n2_x, n2_y, routeX, routeY, currIndex) == false && neighbor == 2)
	{
		routeY[currIndex] = n2_y;
		routeX[currIndex] = n2_x;
	}
	else if(board[n3_y][n3_x] == letter && isOnRoute(n3_x, n3_y, routeX, routeY, currIndex) == false && neighbor == 3)
	{
		routeY[currIndex] = n3_y;
		routeX[currIndex] = n3_x;
	}
	else if(board[n4_y][n4_x] == letter && isOnRoute(n4_x, n4_y, routeX, routeY, currIndex) == false && neighbor == 4)
	{
		routeY[currIndex] = n4_y;
		routeX[currIndex] = n4_x;
	}
	else if(board[n5_y][n5_x] == letter && isOnRoute(n5_x, n5_y, routeX, routeY, currIndex) == false && neighbor == 5)
	{
		routeY[currIndex] = n5_y;
		routeX[currIndex] = n5_x;
	}
	else if(board[n6_y][n6_x] == letter && isOnRoute(n6_x, n6_y, routeX, routeY, currIndex) == false && neighbor == 6)
	{
		routeY[currIndex] = n6_y;
		routeX[currIndex] = n6_x;
	}
	else 
		flag = 0;

	return flag;
}

//will return false until searchRouteU1 or searchRouteO returns true.
bool isGameOver(char board[MAX_SIZE][MAX_SIZE], int sizeOfBoard, bool user)
{
	static int countMove = 0;

	bool flag = 0;
	int index = 1;
	int routeX[MAX_POSSIBLE_MOVE] = {0};
	int routeY[MAX_POSSIBLE_MOVE] = {0};
	
	searchRouteU1(board, routeX, routeY, index, &flag, sizeOfBoard);
	searchRouteU2(board, routeX, routeY, index, &flag, sizeOfBoard, user);

	countMove++;

	if(flag == 1)
		return true;
	else if(flag == 0 && countMove == MAX_POSSIBLE_MOVE)
	{
		cout << "No possible moves left. Tie" << endl; 
		return true;
	}
	else
		return false;

	
}

//checks if one of the neighbors is already in the route or not. if it is then returns true.
bool isOnRoute(int x, int y, int * routeX, int * routeY, int currIndex)
{
	for(int i=1; i<currIndex; i++)
		if(routeX[i] == x && routeY[i] == y)
			return true;

	return false;
}

// for x, first and last column is known(A and 'lastXorY'-depends on size-) 
// if route's first and last value satisfies then we can say path is completed. Same thing also works for 'o'.
bool isPathCompleted(int * routeX, int * routeY, int user, int lastMove, int lastXorY)
{
	int firstMove = 1;
	int firstX    = 1;
	int firstY    = 1;

	if(user == 0 && routeX[firstMove] == firstX && routeX[lastMove] == lastXorY)
		return true;
	else if(user == 1 && routeY[firstMove] == firstY && routeY[lastMove] == lastXorY)
		return true;
	else 
		return false;
}

//if board(x,y) has '.' then returns true. If a letter ('x' or 'o') already placed this time returns false.
bool isEmpty(char board[MAX_SIZE][MAX_SIZE], int x, int y)
{
	if(board[y][x] == '.')
		return true;
	else
		return false;
}

//returns '0' if given input is invalid. Otherwise returns '1'(true).
bool isInputValid(char x, int y, int sizeOfBoard)
{
	int flag = 1;

	if(x != 'A' && x != 'B' && x != 'C' && x != 'D' && x != 'E' && x != 'F' && x != 'G' && x != 'H' && x != 'I' && x != 'J' && x != 'K' && x != 'L')
		flag = 0;

	else if(y < 1 || y > sizeOfBoard)
		flag = 0;

	return flag;
}
