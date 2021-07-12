#include <iostream>
#include <string>
#include <fstream>
#include <vector>   
#include <random>
#include "hexgame.h"

//*********************//
//                     //
//                     //
//  Cell Constructor   //
//                     //
//                     //

// Takes NO Argument only sets cell's state to empty which is '.'
Hex::Cell::Cell() 
{
    set_cellState(empty);
}

//*********************//
//                     //
//                     //
//  Hex Constructors   //
//                     //
//                     //

// Takes NO argument.
// To prevent seg fault sets some important class variables to their initial values
Hex::Hex()
{
    gameFlag = false;
    turn  = u1;
    state = x;
    routeX.resize(MAX_POSSIBLE_MOVE);
    routeO.resize(MAX_POSSIBLE_MOVE);
}

// Takes boardsize and opponent as a parameter.
// Others same as NO argument constructor 
Hex::Hex(int size, char pvx_c)
{
    gameFlag = false;
    turn  = u1;
    state = x;
    routeX.resize(MAX_POSSIBLE_MOVE);
    routeO.resize(MAX_POSSIBLE_MOVE);
    set_boardSize(size);
    set_pvx(pvx_c);
}

// Takes filename as a parameter.
// Sets game name to that filename so it'll be used when continueing a game
Hex::Hex(string filename)
{
    gameName = filename;
    turn  = u1;
    state = x;
    routeX.resize(MAX_POSSIBLE_MOVE);
    routeO.resize(MAX_POSSIBLE_MOVE);
    gameName = filename;

    loadGame(filename, true);
}

//**********************************//
//                                  //
//                                  //
//   Gameplay and helper functions  //
//                                  //
//                                  //

void Hex::playGame(string & name)
{
    initialize_game();

    while(gameFlag == false)
    {
        if(turn == u1)
        {
            userInput();
            play(row, column);
        }
        else
        {
            if(pvx == 'P')
            {
                userInput();
                play(row, column);
            }
            else if(pvx == 'C')
            {
                play();
            }
        }
        addCount(1);

        printBoard();
        isGameOver();
        
        cout << endl << "count = " << getCount() << endl;

        nextTurn();
    }    
    set_gameName(name);
    saveGame(gameName);
}

void Hex::playGame(string & name, bool isExistingGame)
{
    isGameOver();

    if(gameFlag == true)
        cerr << endl << "(This game is already over but you can still print the board though)" << endl;

    cout << endl;
    printBoard();
    while(gameFlag == false)
    {
        if(turn == u1)
        {
            userInput();
            play(row, column);
        }
        else
        {
            if(pvx == 'P')
            {
                userInput();
                play(row, column);
            }
            else if(pvx == 'C')
            {
                play();
            }
        }
        addCount(1);

        printBoard();
        isGameOver();
        
        cout << endl << "count = " << getCount() << endl;

        nextTurn();
    }
    set_gameName(name);
    saveGame(gameName);
}

//just prints out the board nothing fancy
void Hex::printBoard() const
{
    cout << ' '; 
    for(int x = 0; x<boardSize; x++)
        cout << ' ' << columnNames[x];

    cout << endl;
    for(int y = 1; y<=boardSize; y++)
    {
        if(y < 10)
            cout << y << ' ';
        else
            cout << y;
        

        //blank space before dots        
        for(int blank = 0; blank<(y-1); blank++)
			cout << " ";
        
        for(int x = 1; x<=boardSize; x++)
            cout << hexCells[y][x].get_cellState() << ' ';

        cout << endl;
    }
    cout << endl;    
}

void Hex::showGameStatus()
{
    string opponent;
    if(pvx == 'P')
        opponent.append("player  ");
    else
        opponent.append("computer");

    cout << "     " << boardSize << 'x' << boardSize << "\t " << opponent << "\t " << classMoves(); 
}

bool Hex::compareGameWith(Hex & game)
{
    int myGame = classMoves();
    int opGame = game.classMoves();

    if(myGame > opGame)
        return true;

    else if(opGame >= myGame)
        return false;

    else
        return true;        
}







//******************************************//
//                                          //
//                                          //
//  Individual hex class move count getter  //
//                                          //
//                                          //

int Hex::classMoves()
{
	auto count = 0;

	for(int i = 1; i<=boardSize; i++)
		for(int j = 1; j<=boardSize; j++)
			if(hexCells[i][j].get_cellState() == x || hexCells[i][j].get_cellState() == o)
				count++;

	return count;
}







//******************//
//                  //
//                  //
//  Game Mechanics  //
//                  //
//                  //

void Hex::play(int & row, int & column)
{
    hexCells[row][column].set_cellState(state);
}

void Hex::play()
{
    goRandom();

    hexCells[row][column].set_cellState(o); 
}

void Hex::userInput()
{
    string column_s, row_s;
    int column_i = 0, row_i = 0;
    bool inflag = false;

    do
    {
        cout << "User" << turn << "'s turn: ";
        cin >> column_s >> row_s;

        if(column_s == "SAVE")
            saveGame(row_s);

        else if(column_s == "LOAD")
            loadGame(row_s);
        
        else if(column_s == "MENU")
        {
            menu();
            return;
        }

        else if(isColumnValid(column_s) && isRowValid(row_s))
        {
            column_i = column_s[0] - 64;
            row_i    = std::stoi(row_s);
            if(isEmpty(row_i, column_i))
                inflag   = true;
            else
                cerr << "This place is not empty!" << endl;            
        }
    } 
    while(inflag == false);

    column = column_i;
    row    = row_i;

    cout << endl << "[ User" << turn << " placed to (" << column_s[0] << row << ") ]" << endl << endl ;
}

void Hex::nextTurn()
{
    if(turn == u1)
    {
        turn = u2;
        state = o;
    }
    else
    {
        turn = u1;    
        state = x;
    }
}












//*********************//
//                     //
//                     //
//  Game Initializers  //
//                     //
//                     //

void Hex::initialize_game()
{
    string size_s;
    do 
    {
        cout << "\nBoard size? (min: 5 , max: 52)\n"
             << "Enter: ";

        cin >> size_s;
    } 
    while (isSizeValid(size_s) == false);
    
    int s = std::stoi(size_s); 
    set_boardSize(s);
    
    string pvx_s;
    do 
    {
        cout << "\nPlay with? (P: Player , C: Computer)\n"
             << "Enter: ";
        
        cin >> pvx_s;
    } 
    while (isPVXValid(pvx_s) == false);
    
    char p = pvx_s[0];
    set_pvx(p);

    initialize_board();
    printBoard();
}

// initializes hexCells vector with empty('.') states.
void Hex::initialize_board()
{
    //all possible column names
    char column[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
                      'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
                      'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
                      'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', };

    //inserting column names to its variable. variable will bu used in printBoard()
    for(int i=0; i<boardSize; i++)
        columnNames.push_back(column[i]);

    hexCells.resize(boardSize+2);

    for(int i = 0; i<boardSize+2; i++)
        hexCells[i].resize(boardSize+2);
}

// opens a file named as "filename" then fills the file with necessary information about game(sizeofboard, board state, etc.)
void Hex::saveGame(string & filename)
{
    int clMoves = classMoves();

    gameName = filename;

   	ofstream input;
	input.open(filename);

	input << boardSize << endl << turn << endl << pvx << endl << clMoves << endl << gameFlag << endl;
	for(int i = 1; i<=boardSize; i++)
		for(int j = 1; j<=boardSize; j++)
			input << hexCells[i][j].get_cellState();
	input << endl;
	
	input.close();

	cout << "(Game Saved!)\n" << endl;

}

// loads the game with given filename. 
// if game exists works without printing board.
void Hex::loadGame(string filename, bool isExistingGame)
{
    int size = 0;
	ifstream output;

	output.open(filename);

	output >> size;

    int clMoves = 0;
    remCount(clMoves);

	if(size > 52 || size < 5)
	{
		cerr << endl << "The game named \"" << filename << "\" doesn't exist.\n" << endl;
		return;
	}
	else
	{
		output >> turn >> pvx >> clMoves >> gameFlag;
		boardSize = size;
	}

	initialize_board();
    addCount(clMoves);

    char ch;
	for(int i=1; i<=boardSize; i++)
		for(int j=1; j<=boardSize; j++)	
		{
            output >> ch;
            hexCells[i][j].set_cellState(ch);
        }	

	output.close();
}

// loads the game with given filename. 
void Hex::loadGame(string & filename)
{
	int size = 0;
	ifstream output;

	output.open(filename);

	output >> size;

    int clMoves = classMoves();
    remCount(clMoves);

	if(size > 52 || size < 5)
	{
		cerr << endl << "The game named \"" << filename << "\" doesn't exist.\n" << endl;
		return;
	}
	else
	{
		output >> turn >> pvx >> clMoves >> gameFlag;
		boardSize = size;
	}

	initialize_board();
    addCount(clMoves);

    char ch;
	for(int i=1; i<=boardSize; i++)
		for(int j=1; j<=boardSize; j++)	
		{
            output >> ch;
            hexCells[i][j].set_cellState(ch);
        }	

	output.close();

    cout << endl << "(Game Loaded!)" << endl << endl;
    printBoard();
}








//****************//
//                //
//                //
//  Win Checkers  //
//                //
//                //

// when a route found gameflag becomes true
void Hex::isGameOver()
{
    gameFlag  = false;

    int cellIndex = 1; // holds position for a single cell. used when searching possible winning routes.

    deleteRoute();
    if(turn == u1)
        searchX(cellIndex);
    else
        searchO(cellIndex);    
}

// search for 'x' letters recursively. 
// calls itself until no x is found or a winning path found
void Hex::searchX(int cellIndex)
{
	if(isPathCompleted(cellIndex-1) == true)
	{
		convertToUpper();
	
      	cout << "\n-------------------------------------------\n" << endl;
	
    	printBoard();
    
    	cout << "\nUser1 won the game!\n\n";
	
		gameFlag = 1;
	}

    //only for first 'x' location searchs every y value.
    else if(cellIndex == 1)
		for(int i=1; i<=boardSize; i++)
		{
			deleteRoute();
			if(hexCells[i][cellIndex].get_cellState() == x)
			{
				routeX[cellIndex] = 1;
				routeO[cellIndex] = i;

				searchX(cellIndex+1);
			}
		}
    
	//recursively calls searchRouteU1 function until move count reachs MAX_POSSIBLE_MOVE and looks for nearby 'x' values. 
	else if(cellIndex > 1 && cellIndex < (boardSize*boardSize)/2)
    {
		for(int i=1; i<=6; i++)
			if(checkNeighbors(cellIndex, i, x) == true)
				searchX(cellIndex+1);		

    }
}

// search for 'o' letters recursively. 
// calls itself until no o is found or a winning path found
void Hex::searchO(int cellIndex)
{
	if(isPathCompleted(cellIndex-1) == true)
	{
		convertToUpper();

		cout << "\n-------------------------------------------\n" << endl;

		printBoard();
        
        if(pvx == 'P')
		    cout << "\nUser2 won the game!\n\n";
        else
		    cout << "\nComputer won the game!\n\n";
		
        gameFlag = 1;
	}

    //only for first 'x' location searchs every y value.
    else if(cellIndex == 1)
		for(int i=1; i<=boardSize; i++)
		{
			deleteRoute();
			if(hexCells[cellIndex][i].get_cellState() == o)
			{
				routeX[cellIndex] = i;
				routeO[cellIndex] = 1;

				searchO(cellIndex+1);
			}
		}
    
	//recursively calls searchRouteU1 function until move count reachs MAX_POSSIBLE_MOVE and looks for nearby 'x' values. 
	else if(cellIndex > 1 && cellIndex < (boardSize*boardSize)/2)
    {
		for(int i=1; i<=6; i++)
			if(checkNeighbors(cellIndex, i, o) == true)
				searchO(cellIndex+1);		

    }
}














//******************************//
//                              //
//                              //
//  Win Check Helper Functions  //
//                              //
//                              //

// let's say boardSize = 5
// for ex: if x is the letter we searching routeX[firstMove] must be equal to one of the rows first column(like A1 A2 A3)
// then we look for routeX[lastMove] and it must be one of the rows of last column(E1 E2 E3)
// if these 2 is satisfied then that means a complete route for 'x' letter has been found. and func return true. 
// else return false. 
bool Hex::isPathCompleted(int cellIndex)
{
	int firstMove = 1;
	int firstX    = 1;
	int firstY    = 1;
    int lastXY    = boardSize;

	if(turn == u1 && routeX[firstMove] == firstX && routeX[cellIndex] == lastXY)
		return true;
	else if(turn == u2 && routeO[firstMove] == firstY && routeO[cellIndex] == lastXY)
		return true;
	else 
		return false;
}

// return true if given cell is empty '.'
bool Hex::isEmpty(int & row_i, int & column_i)
{
    if( (column_i <= boardSize && row_i <= boardSize) && (column_i >= 1 && row_i >= 1) )
        if( hexCells[row_i][column_i].get_cellState() == empty)
            return true;

    return false;
}

// if given coords has 'x' or 'o' that already gotten in the route returns true
// else false
bool Hex::isOnRoute(int x, int y, int cellIndex)
{
	for(int i=1; i<cellIndex; i++)
		if(routeX[i] == x && routeO[i] == y)
			return true;

	return false;
}

// deletes all values of both routeX and routeO
void Hex::deleteRoute()
{
    int i = 0;
    while(routeX[i] != 0 && routeO[i] != 0)
    {
        routeX[i] = 0;
        routeO[i] = 0;

        i++;
    }
}

// chechks neighbors for given letter
// if given letter is found for the specified neighbor then returns true
// else false
bool Hex::checkNeighbors(int cellIndex, int neighbor, char letter)
{
	int flag = 1;
	//																		Ex:  (for B5)																						
	//neighbors 				             								    column  row
	int n1_y = routeO[cellIndex-1]     ; int n1_x = routeX[cellIndex-1] - 1; //   A      5 
	int n2_y = routeO[cellIndex-1] + 1 ; int n2_x = routeX[cellIndex-1] - 1; //   A      6
	int n3_y = routeO[cellIndex-1] - 1 ; int n3_x = routeX[cellIndex-1]    ; //   B      4
	int n4_y = routeO[cellIndex-1] + 1 ; int n4_x = routeX[cellIndex-1]    ; //   B      6
	int n5_y = routeO[cellIndex-1] - 1 ; int n5_x = routeX[cellIndex-1] + 1; //   C      4
	int n6_y = routeO[cellIndex-1]     ; int n6_x = routeX[cellIndex-1] + 1; //   C      5

	if(hexCells[n1_y][n1_x].get_cellState() == letter && isOnRoute(n1_x, n1_y, cellIndex) == false && neighbor == 1)
	{
		routeO[cellIndex] = n1_y;
		routeX[cellIndex] = n1_x;
	}
	else if(hexCells[n2_y][n2_x].get_cellState() == letter && isOnRoute(n2_x, n2_y, cellIndex) == false && neighbor == 2)
	{
		routeO[cellIndex] = n2_y;
		routeX[cellIndex] = n2_x;
	}
	else if(hexCells[n3_y][n3_x].get_cellState() == letter && isOnRoute(n3_x, n3_y, cellIndex) == false && neighbor == 3)
	{
		routeO[cellIndex] = n3_y;
		routeX[cellIndex] = n3_x;
	}
	else if(hexCells[n4_y][n4_x].get_cellState() == letter && isOnRoute(n4_x, n4_y, cellIndex) == false && neighbor == 4)
	{
		routeO[cellIndex] = n4_y;
		routeX[cellIndex] = n4_x;
	}
	else if(hexCells[n5_y][n5_x].get_cellState() == letter && isOnRoute(n5_x, n5_y, cellIndex) == false && neighbor == 5)
	{
		routeO[cellIndex] = n5_y;
		routeX[cellIndex] = n5_x;
	}
	else if(hexCells[n6_y][n6_x].get_cellState() == letter && isOnRoute(n6_x, n6_y, cellIndex) == false && neighbor == 6)
	{
		routeO[cellIndex] = n6_y;
		routeX[cellIndex] = n6_x;
	}
	else 
		flag = 0;

	return flag;
}

// converts letters to uppercase for the winning route
void Hex::convertToUpper()
{
    int i = 1;

	if(turn == u1)
		while(routeX[i] != 0)
        {
			hexCells[routeO[i]][routeX[i]].set_cellState(bigX);
            i++;
        }
	else
		while(routeX[i] != 0)
        {
			hexCells[routeO[i]][routeX[i]].set_cellState(bigO);
            i++;
        }
}









//*****************//
//                 //
//                 //
//  Computer Move  //
//                 //
//                 //

// creates random row and column numbers until it finds an empty cell.
void Hex::goRandom()
{
    random_device random;
    mt19937 exe(random());
    uniform_real_distribution<double> range(1, boardSize+1);

    int row_rand    = 0;
    int column_rand = 0;

    do
    {
        row_rand    = static_cast<int>(range(exe));
        column_rand = static_cast<int>(range(exe));
    } 
    while (isEmpty(row_rand, column_rand) == false);

    row    = row_rand;
    column = column_rand;

    cout << "Computers turn...\n\n" 
         << "[ Computer placed to (" << static_cast<char>(column+64) << row << ") ]\n" << endl;
}


















//**************************//
//                          //
//                          //
//  Input Validty Checkers  //
//                          //
//                          //

// if given char is a number return true else false
bool Hex::isNumber(char & num)
{
    int num_i = static_cast<int>(num) - 48;

    for(int i=0; i<10; i++)
        if(num_i == i)
            return true;
    
    return false;
}

// if given string is valid for size returns true else false
bool Hex::isSizeValid(string & size_s)
{
    if(size_s.size() == 2 && isNumber(size_s[0]) && isNumber(size_s[0]))
    {
        int num = std::stoi(size_s);

        if( num >= 10 && num <= 52 )
            return true;
    }
    
    else if(size_s.size() == 1 && isNumber(size_s[0]) )
    {
        int num = std::stoi(size_s);

        if( num >= 5 && num <= 9 )
            return true;
    }        
    
    cerr << endl
         << "*****************************\n"
         << "Invalid input for board size!" << endl
         << "*****************************\n";
    return false;
}

// if given string is valid for pvx returns true else false
bool Hex::isPVXValid(string & pvx_s)
{
    if( pvx_s.size() == 1 && (pvx_s[0] == 'P' || pvx_s[0] == 'C') )
        return true;

    cerr << endl
         << "*************************************\n"
         << "Invalid input for oppenent selection!" << endl
         << "*************************************\n";

    return false;
}

// if given string is valid for row returns true else false
bool Hex::isRowValid(string & row_s)
{
    int valueOne =  49; // which equals to char 1
    int valueNine = 57; // which equals to char 9
    int maxRowDecValue = 49 + boardSize; // if boardsize = 5, then equals to 5 (for one digit values)
    int maxValue = boardSize/10;         // for example if boardsize = 42, then maximum value for first digit of 2 digit values will be 2.

    if(row_s.size() == 1 && row_s[0] >= valueOne && row_s[0] < maxRowDecValue)
        return true;
    else if (row_s.size() == 2 && row_s[0] >= valueOne && row_s[0] <= maxValue && row_s[1] >= valueOne && row_s[1] <= valueNine)
        return true;

    cerr << "(Invalid input!)" << endl;   
    return false;
}

// if given string is valid for column returns true else false
bool Hex::isColumnValid(string & column_s)
{
    int minColumnDecValue = 65;             // which is equal = 'A'
    int maxColumnDecValue = 65 + boardSize; // if boardSize = 5, then an user can give maximum value as 70 which is 'E'

    if(column_s.size() == 1 && column_s[0] < maxColumnDecValue && column_s[0] >= minColumnDecValue)
        return true;

    cerr << "(Invalid input!)" << endl;   
    return false;
}



//*********************//
//                     //
//                     //
//  GLOBAL FUNCTIONS   //
//                     //
//                     //

// creates a new name with increased gameNum
string newName(int & gameNum)
{
    string name;

    gameNum += 1;

    string p1("game");

    string p2 = to_string(gameNum);

    string p3(".txt");

    name.append(p1);
    name.append(p2);
    name.append(p3);

    return name;
}

//Validty check
bool isChoiceValid(string & choice)
{
    if(choice.size() == 1 && choice[0] >= 49 && choice[0] <= 54)
        return true;

    cerr << "(Invalid input!)" << endl;
    return false;
}

//Prints out the current games and their status.
void printGamesStatus(vector<Hex> & games)
{
    cout << endl;
    cout << " \t     Board \t Opponent    Move Count" << endl
         << " \t     ----- \t --------    ----------" << endl;
    for(int i=0; i<games.size(); i++)
    {
        cout << i+1 << ") Game" << i+1 << ' ';
        games[i].showGameStatus();
        cout << endl;
    }
}



// ********** MENU PART **********

// CASE 1
// adds a new game and a name for this game
void newGame(vector<Hex> & games, string & name)
{
    games.push_back(Hex());

    int i = games.size() - 1;

    games[i].playGame(name);
}

// CASE 2
void continueGame(vector<Hex> & games)
{
    int select;

    printGamesStatus(games);
    cout << endl << "Select a game: ";

    cin >> select;

    string name;

    string p1("game");
    string p2 = to_string(select);
    string p3(".txt");

    name.append(p1);
    name.append(p2);
    name.append(p3);

    games[select-1].playGame(name, true);
}

// CASE 3
void compareGames(vector<Hex> & games)
{
    int game1, game2;

    bool flag;

    printGamesStatus(games);
    cout << endl << "Select two games to compare(ex:2 5): ";

    cin >> game1 >> game2;

    int g1_count = games[game1-1].classMoves();
    int g2_count = games[game2-1].classMoves();

    flag = games[game1-1].compareGameWith(games[game2-1]);

    if(flag == true)
        cout << endl << "[ Game" << game1 << " has more moves(" << g1_count << ") than Game" << game2 << "(" << g2_count << "). ]" << endl << endl;

    else
        cout << endl << "[ Game" << game2 << " has more moves(" << g2_count << ") than Game" << game1 << "(" << g1_count << "). ]" << endl << endl;
}

// CASE 4
void printTotalMoveCount(vector<Hex> & games)
{
    printGamesStatus(games);

    cout << "\t\t\t\t\t+" << endl;
    cout << "\t\t\t\t\t----" << endl;
    cout << "\t\t      Total Move Count = " << Hex::getCount() << endl << endl;
}

// CASE 5
void printSelectedBoard(vector<Hex> & games)
{
    int game;
    printGamesStatus(games);

    cout << endl << "Select a game to print it's board: ";
    cin >> game;

    cout << endl;
    games[game-1].printBoard();
    cout << endl;
}
