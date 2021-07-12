#include "hexgame.h"

// [Hex constructors]

Hex::Hex()
{ 
    *this = 52;
}

Hex::Hex(int size) : boardSize(size)
{
    hexCells = allocateCells();
    route = allocateRoute();
    playedPath = allocateRoute();
}

Hex::Hex(string name)
{
    *this = 52;
    if(loadGame(name) == false)
        cerr << "Error: Initially added game couldn't added. Please check if your giving a valid .txt file." << endl;
}



// [Cell constructors]

Hex::Cell::Cell() : cellState(empty), row(0), column(0) 
{ /* cell will be initialized to empty state and row - column numbers will be 0 */ }

Hex::Cell::Cell(int row, int column, char state = empty) : row(row), column(column), cellState(state)
{ /* unless state is given, cell will be initialized to given state. also row and column has to be explicitly given. */ }








// [Validity Checker]

const bool isInputValid(const int input, const int lowerBound, const int upperBound) 
{
    // clearing cin to prevent infinite loop in case of cin fails    
    if(cin.fail())
    {
        cout << endl << "Error! Invalid input." << endl;
        cin.clear();
        cin.ignore(128, '\n'); // ignoring until '\n' character
        return false;
    }
    else if(input < lowerBound || input > upperBound)
    {
        cout << endl << "Error! A valid input should be between " << lowerBound << '-' << upperBound << '.' << endl; 
        return false;
    }

    return true;
}












//  [Initializiers] 

Hex::Cell ** Hex::allocateCells()
{
    Cell ** cells;

    const int size = getBoardSize();

    // allocating +3 Cell then normally for nullptr at the end 
    cells = new Cell * [size+3];
    cells[size+2] = nullptr;

    // allocating cells for each row as much as size number
    for(int i=0; i<size+2; i++)
        cells[i] = new Cell[size+2];
    

    return cells;
}

int ** Hex::allocateRoute()
{
    const int size = getBoardSize() ;
    const int MAX_POSSIBLE_MOVE = (size*size);
    
    int ** r;
    r = new int * [2]; // r[0] for y-axis and and r[1] for x-axis 
    
    for(int i=0; i<2; i++)
        r[i] = new int[MAX_POSSIBLE_MOVE]; // every axis can't have more move than MAX_POSSIBLE_MOVE


   for(int i=0; i<2; i++)
        for(int j=0; j<MAX_POSSIBLE_MOVE; j++)
            r[i][j] = 0;

    return r;
} 

void Hex::initializeGame()
{
    // initializing to prevent getting trash numbers
    int size = -1;
    int op = -1;

    // get input until a valid size is given
    while(size < 5 || size > 52)
    {
        // getting size from the user
        cout << "Board size? (min: 5 ~ max: 52)" << endl;
        cout << "Enter: ";  
        cin  >> size;

        // error text
        if(size < 5 || size > 52)
            cerr << endl << "Error: Invalid input for size" << endl << endl;

        // clearing cin to prevent infinite loop in case of cin fails    
        if(cin.fail())
        {
            cin.clear();
            cin.ignore(128, '\n'); // ignoring until '\n' character
        }
    }

    // inserting valid size to boardSize class member
    setBoardSize(size);

    // get input until a valid opponent is given
    while(op != user && op != computer)
    {
        // getting opponent choise from the user
        cin.clear();
        cout << "Play with? (Player: 1 ~ Computer: 2)" << endl;
        cout << "Enter: ";
        cin  >> op;
        
        // error text
        if(op != user && op != computer)
            cerr << endl << "Error: Invalid input for opponent" << endl << endl;
        
        // clearing cin to prevent infinite loop if cin fails    
        if(cin.fail())
        {
            cin.clear();
            cin.ignore(128, '\n'); // ignoring until '\n' character
        }
    }
    // inserting valid size to opponent class member
    setOpponent(op);    
}












// [Fundamental play functions]

void Hex::playGame(bool isExistingGame) 
{
    int row, column; // for userInput(row, column) and play(row, column)

    // initializing part
    if(!isExistingGame)
    {
        initializeGame(); 
        hexCells   = allocateCells(); // dynamically allocates hexCells   
        route      = allocateRoute(); // dynamically allocates route      
        playedPath = allocateRoute(); // dynamically allocates playedPath 
    }
    else
        changeTurn();

    if(getFinish() == true)
    {
        cout << endl << "This game is already finished but you can still see the board." << endl << endl;
        cout << *this;
    }
    
    Cell * temp = new Cell; // creating a temporary cell
    while(!isGameOver())
    {
        plusMove(1);
        changeTurn(); 

        cout << *this; // prints board
    
        if(getTurn() == user1 || (getTurn() == user2 && opponent == user))
            userInput(row, column); // play function for user is in this function

        else if(getTurn() == ai && opponent == computer)
        {
            temp  = play(); // getting the Cell where the change is made
            *temp = Cell(getRow(), getColumn(), letterO); // changing state of the hexCells[y][x]
        }

        if(getGoTo()) // if user inputted "MENU" it will break loop to stop the game
        {
            setGoTo(false);
            break;
        }
    }
}

Hex::Cell * Hex::play()
{
    goRandom(); //finds valid row and column and assigns it

    int y = getRow();    
    int x = getColumn(); 

    cout << "Computers turn..." << endl << endl; 
    if(x < 26)
        cout << "[ Computer placed to (" << static_cast<char>(x+64) << y << ") ]" << endl;
    else
        cout << "[ Computer placed to (" << static_cast<char>(x+70) << y << ") ]" << endl;

    return &hexCells[y][x];
}

void Hex::play(int & row, int & column)
{
    setColumn(column); 
    setRow(row);     

    // if user gives a valid location it will be added to the playedPath
    const int currIndex = getMove() - 1;
    playedPath[0][currIndex] = row;
    playedPath[1][currIndex] = column;
    
    char letter = (getTurn() == 1)? letterX:letterO;   // getting right letter before assigning to a cell
    hexCells[row][column] = Cell(row, column, letter); // initializing hexCell with right letter

    cout << endl << "[ User" << getTurn() << " placed to (" << getColumn(' ') << getRow() << ") ]" << endl;
}

void Hex::userInput(int & row, int & column)
{
    const int lowerBound = 1;
    const int upperBound = getBoardSize();

    int column_i;  // function converts column_s to column_i before setting in the class member so we can check it's validty
    int row_i;     // same as row..

    string fileName;  // for LOAD and SAVE conditions it will hold the name of the file
    string column_s;  // takes column as string so it is easy to check column's validty
    string row_s;     // same is valid for row

    while(true) // loops continue until a valid input is given
    {
        cout << "User" << getTurn() << "'s turn: ";
        cin >> column_s;

        if(column_s == "SAVE" || column_s == "LOAD")
        {
            cin >> fileName;

            if(column_s == "SAVE" && saveGame(fileName) == false)
                cerr << "Error: Game \"" << fileName << "\" couldn't saved. Name is invalid." << endl;
            else if(column_s == "LOAD")
            {
                if(loadGame(fileName) == true)
                   cout << endl << "Game Loaded!" << endl;
                else
                   cerr << "Error: Game \"" << fileName << "\" doesn't exist. Please check if you are giving a valid .txt file" << endl; 
            }

            minusMove(1);
            changeTurn();
            return;
        }
        else if(column_s == "MENU")
        {
            setGoTo(true); // to break isGameOver() loop in the playGame()
            minusMove(1);  // so it doesn't count this as a move

            cout << endl << "Returning to the menu..." << endl;
            
            return;
        }
        else if(column_s == "BACK")
        {
            if(countMoves() == 0 || countMoves() == 1)
            {
                cerr << "Error: There aren't enough letters on the board" << endl;
                changeTurn();
                minusMove(1);  // doesn't count this as a move
            }
            else 
                moveBack();
            return;
        }
        else
        {
            cin >> row_s;

            if(row_s.size() > 2 || row_s[0]-48 < lowerBound || row_s[0]-48 > upperBound || column_s.size() != 1)
            {
                cerr << "Error: Invalid input" << endl;
                changeTurn(); // changing turn 2 times so same user can play until a valid input is given
                minusMove(1); // this doesn't count as a move so we substract it
                return;
            }
            column_i = column_s[0] - 64; // (char)column to (int)column
            row_i    = std::stoi(row_s); // (string)row  to (int)row
            
            if(column_i > 26) // After letter Z, column doesn't continue with 'a' so we skip until 'a' by adding extra 6
                column_i -= 6;

            if( !isInputValid(column_i, lowerBound, upperBound) || !isInputValid(row_i, lowerBound, upperBound)) // validty check
            {
                changeTurn(); // changing turn 2 times so same user can play until a valid input is given
                minusMove(1); // this doesn't count as a move so we substract it
                return;
            }
            else if(isEmpty(row_i, column_i)) // play and break the loop if given cell location is empty
            {
                play(row_i, column_i);
                break;
            }
            else
                cerr << "This place is not empty!" << endl; 
        }

    }
}

bool Hex::saveGame(string & name)
{
    ofstream stream;
    stream.open(name);

    if(stream.fail())
        return false;

    setName(name);

    stream << *this;

    cout << endl << "Game Saved!" << endl;
    stream.close();

    return true;
}

bool Hex::loadGame(string & name)
{
    ifstream stream;
    stream.open(name);

    if(stream.fail())
        return false;

    // substracting all move count before loading a game 
    int moves = countMoves();
    minusMove(moves);
    
    Hex * newGame = new Hex(52);


    stream >> *newGame;

    *this = *newGame;

    stream.close();

    return true;
}

void Hex::moveBack()
{
    const int index1 = getMove() - 2;
    const int index2 = getMove() - 3;

    const int row1 = playedPath[0][index1]; int column1 = playedPath[1][index1];
    const int row2 = playedPath[0][index2]; int column2 = playedPath[1][index2];

    // it is necessery to increase extra 6 after Z letter. 
    if(column1 > 26)
        column1 += 6;
    if(column2 > 26)
        column2 += 6;

    // getting letters ascii decimal values
    column1 += 64; 
    column2 += 64;

    cout << "Deleted moves: (" << (char)column1 << ' ' << row1 << ") and (";
    cout << (char)column2 << ' ' << row2 << ")" << endl;
 
    --*this;    // pre  operator overload
    (*this)--;  // post operator overload
            
    minusMove(1);  
    changeTurn();
}


int Hex::countMoves() const // returns number of moves
{
    int count = 0;
    const int size = getBoardSize(); 
    
    for(int i=1; i<size+1; i++)
        for(int j=0; j<size+1; j++)
            if(hexCells[i][j].getState() != empty)
                count++;
    
    return count;
}












// [AI Moves] 

void Hex::goRandom() 
{
    int size = getBoardSize() + 1;

    //initializing random number generator
    random_device rand;
    mt19937 exe(rand());
    uniform_real_distribution<double> range(1, size);
    
    int x = 0, y = 0;
    do // loops continue until a valid cell is found. when found row and colum changes
    {
        y = static_cast<int>(range(exe)); // normally it will return double we don't want that 
        x = static_cast<int>(range(exe)); // so we cast it to int
    } 
    while (isEmpty(y, x) == false);

    setColumn(x);
    setRow(y);

    // adding computer move to the playedPath
    const int currIndex = getMove() - 1;
    playedPath[0][currIndex] = y;
    playedPath[1][currIndex] = x;
}












// [Score Functions]

const int Hex::getScoreX()
{
    int score = 0;
    bool isFinished;

    const int size = getBoardSize();

    //if there is a big letter returns 100 else assigns isFinished to false
    for(int i=1; i<size+1; i++)
    {
        if(hexCells[i][1].getState() == bigLetterX)
            return 100;
        else
            isFinished = false;        
    }

    // if inner loop finds any letterX in the same column then increases score and breaks the loop for that column 
    for(int i=1; i<size+1; i++)
        for(int j=1; j<size+1; j++)
            if(hexCells[j][i].getState() == letterX)
            {
                score++;
                break;
            }

    const int percentScore  = (score*100)/size;  // for ex: score = 3, size = 5 then the percentage is (3*100)/5 = 60%
    const int percentScore2 = ((score-1)*100)/size; 

    if(isFinished == false && percentScore == 100)
        return percentScore2;

    return percentScore;
}

const int Hex::getScoreO()
{
    int score = 0;
    bool isFinished;

    const int size = getBoardSize();

    //if there is a big letter returns 100%
    for(int i=1; i<size+1; i++)
    {
        if(hexCells[1][i].getState() == bigLetterO)
            return 100;
        else
            isFinished = false;        
    }
    
    // if inner loop finds any letterO in the same row then increases score and breaks the loop for that row 
    for(int i=1; i<size+1; i++)
        for(int j=1; j<size+1; j++)
            if(hexCells[i][j].getState() == letterO)
            {
                score++;
                break;
            }

    const int percentScore  = (score*100)/size;  // for ex: score = 3, size = 5 then the percentage is (3*100)/5 = 60%
    const int percentScore2 = ((score-1)*100)/size; 

    if(isFinished == false && percentScore == 100)
        return percentScore2;
    
        
    return percentScore;
}












// [Win Check Functions]

bool Hex::isGameOver()
{
    int index = 1;
    searchRoute(index);
    
    return getFinish();
}

void Hex::searchRoute(int index)
{
    const int size  = getBoardSize();
    const int turn  = getTurn();
    const int MAX_POSS_MOVE = (size*size) / 2;

    if(isPathCompleted(index-1))
    {
        setFinish(true);    

        if(turn == user1)
            xWon();
        else
            oWon();
    }
    else if(index == 1)
    {
        for(int i=1; i<=size; i++)
        {
            cleanRoute();
			if(hexCells[i][index].getState() == letterX && turn == user1)
			{
				route[1][index] = 1;
				route[0][index] = i;
                searchRoute(index+1);
			}
			else if(hexCells[index][i].getState() == letterO && turn == user2)
			{
				route[1][index] = i;
				route[0][index] = 1;
                searchRoute(index+1);
			}
        }
    } 
	else if(index > 1 && index < MAX_POSS_MOVE){
		for(int i=1; i<=6; i++)
			if(isNeighborFriend(index, i))
				searchRoute(index+1);		
    }
}












// [Win Check Helper Functions]

const bool Hex::isPathCompleted(int index)
{
    const int currTurn     = getTurn(); 
    const int lastLocation = getBoardSize();


    // let's say boardSize = 5(also lastLocation == 5)
    // for ex: if x is the letter we searching route[1][1] must be equal to one of the rows first column(like A1 A2 A3)
    // then we look for route[1][Index] and it must be one of the rows of last column(E1 E2 E3)
    // if these 2 is satisfied then that means a complete route for 'x' letter has been found. and function returns true. 
	if(currTurn == user1 && route[1][1] == 1 && route[1][index] == lastLocation)
		return true;
	else if(currTurn == user2 && route[0][1] == 1 && route[0][index] == lastLocation)
		return true;
	
    return false;
}

const bool Hex::isNeighborFriend(int index, int neighbor)
{
	// neighbor locations
    int * y = new int[7]; //    				 	    	        Ex:  (for B5)
    int * x = new int[7]; //   				                         column  row
	y[1] = route[0][index-1]     ; x[1] = route[1][index-1] - 1; //   A      5 
	y[2] = route[0][index-1] + 1 ; x[2] = route[1][index-1] - 1; //   A      6
	y[3] = route[0][index-1] - 1 ; x[3] = route[1][index-1]    ; //   B      4
	y[4] = route[0][index-1] + 1 ; x[4] = route[1][index-1]    ; //   B      6
	y[5] = route[0][index-1] - 1 ; x[5] = route[1][index-1] + 1; //   C      4
	y[6] = route[0][index-1]     ; x[6] = route[1][index-1] + 1; //   C      5

    char letter = (getTurn() == user1)? letterX:letterO; // getting current player's letter
   
    int currX = x[neighbor];
    int currY = y[neighbor];
    int currState = hexCells[currY][currX].getState();

    delete []x;
    delete []y;

    if(currState == letter && !isOnRoute(currX, currY, index))
    {
        route[0][index] = currY;
        route[1][index] = currX;
    }
    else
        return false;

    return true;
}

const bool Hex::isOnRoute(int & x, int & y, int & index) // if given coords is on the route returns true. if not returns false
{
	for(int i=1; i<=index; i++)
		if(route[0][i] == y && route[1][i] == x)
			return true;

	return false;
}

const bool Hex::isEmpty(int & y, int & x) // return true if given cell is empty '.'
{
    int size = getBoardSize();

    if( (x <= size && y <= size) && (x >= 1 && y >= 1) )
        if( hexCells[y][x].getState() == empty)
            return true;

    return false;
}

void Hex::cleanRoute()
{
    delete []route[0]; // deleting route[0][MAX_POSSIBLE_MOVE]
    delete []route[1]; // deleting route[1][MAX_POSSIBLE_MOVE]
    delete []route ; // deleting route itself

    route = allocateRoute(); // reallocating
}

void Hex::convertToUpper() // converts letters to uppercase for the winning route
{
    const int currTurn  = getTurn();
    int x, y, i=1; 

    while(route[0][i] != 0)
    {
        x = route[1][i]; // getting route's column values
        y = route[0][i]; // getting route's row values

        if(currTurn == user1)
			hexCells[y][x] = Cell(y, x, bigLetterX); // assignin 'O' letter to gotten cell 
        else
			hexCells[y][x] = Cell(y, x, bigLetterO); // assignin 'X' letter to gotten cell
        
        i++;
    }
}

void Hex::xWon()
{
    convertToUpper();
    cout << "\n-------------------------------------------\n" << endl;
    cout << *this << endl;
    cout << "User1 won the game!" << endl << endl;
}

void Hex::oWon()
{
    convertToUpper();
    cout << "\n-------------------------------------------\n" << endl;
    cout << *this << endl;
    if(opponent == user)
        cout << "User2 won the game!" << endl << endl;
    else 
        cout << "Computer won the game!" << endl << endl;
}












// [Overloaded Operators]

Hex Hex::operator--(int) // post decrease operator
{
    Hex temp = *this; // getting a copy of the current object before changing it

    --*this;  // rest is same with pre operator

    return temp; // returning copy
}

Hex & Hex::operator--() // pre decrease operator
{
    minusMove(1); // removing move from move counts

    const int index1 = getMove() - 1;     // index of last move

    const int row1 = playedPath[0][index1];    // getting last move's row location
    const int column1 = playedPath[1][index1]; // getting last move's column location

    // assigning empty to these locations
    hexCells[row1][column1].setState(empty);
    
    // also cleaning these locations from playedPath 
    playedPath[0][index1] = 0;
    playedPath[1][index1] = 0;

    return *this;
}

bool operator > (const Hex & game1, const Hex & game2)
{
    const int moveCountG1 = game1.countMoves();
    const int moveCountG2 = game2.countMoves();

    return (moveCountG1 > moveCountG2);
}

bool operator < (const Hex & game1, const Hex & game2)
{
    const int moveCountG1 = game1.countMoves();
    const int moveCountG2 = game2.countMoves();

    return (moveCountG1 < moveCountG2);
}

bool operator == (const Hex & game1, const Hex & game2)
{
    const int moveCountG1 = game1.countMoves();
    const int moveCountG2 = game2.countMoves();

    return (moveCountG1 == moveCountG2);
}

ifstream & operator >> (ifstream & stream, Hex & game)
{
    game.changeTurn();

    char c;
    int gameSize, opponent, currTurn;
    string gameName;

    stream >> gameName >> gameSize >> opponent >> currTurn; // getting game information to the variables;

    // initializing and allocating game object with given information
    game.hexCells = game.allocateCells();
    game.route    = game.allocateRoute();
    game.setBoardSize (gameSize);
    game.setName      (gameName);
    game.setOpponent  (opponent);
    game.setTurn      (currTurn);
    
    // changing the cell states
    for(int i=1; i<=gameSize; i++)
        for(int j=1; j<=gameSize; j++)
        {
            stream >> c;
            game.hexCells[i][j].setState(c); 
        }

    // adding loaded game's move count to the class member
    int moves = game.countMoves();
    game.plusMove(moves);

    // adding loaded game's playedPath to the class member
    int maxPath = (gameSize*gameSize);
    for(int j=0; j<maxPath; j++)
        for(int i=0; i<2; i++) 
            stream >> game.playedPath[i][j]; 

    return stream;
}   

ofstream & operator << (ofstream & stream, Hex & game)
{
    // getting class members from game object
    const int gameSize = game.getBoardSize();
    const int opponent = game.getOpponent();
    const int currTurn = game.getTurn();
    string gameName = game.getName();

    stream << gameName << endl << gameSize << endl << opponent << endl << currTurn << endl; // copying members to the stream

    // copying cell states to the stream
    for(int i=1; i<=gameSize; i++)
        for(int j=1; j<=gameSize; j++)
            stream << game.hexCells[i][j].getState();
    stream << endl;

    // copying playedPath to the stream
    int maxPath = (gameSize*gameSize);
    for(int j=0; j<maxPath; j++)
    {
        for(int i=0; i<2; i++) 
            stream << game.playedPath[i][j] << ' '; 
        stream << endl;
    }

    return stream;
}

ostream & operator << (ostream & stream, Hex & game) // gives current board state to the stream
{
    const int size = game.getBoardSize();
    const int scoreX = game.getScoreX();
    const int scoreO = game.getScoreO();

    // column names
    cout << endl << ' ';
    for(int i=0; i<size; i++)
    {
        if(i < 26)
            cout << ' ' << static_cast<char>(i + 65);
        else 
            cout << ' ' << static_cast<char>(i + 71);
    }


    cout << endl;
    for(int i=1; i<=size; i++)
    {
        // between 1-10 we need one extra space 
        if(i < 10)
            cout << i << ' ';
        else
            cout << i;
        
        // the blank space before cells
        for(int blank=1; blank<i; blank++)
			cout << " ";
        
        // cells
        for(int j=1; j<=size; j++)
            cout << game.hexCells[i][j].getState() << ' ';

        
        // scores 
        if(i == 1)
            cout << "          Scores";
        else if(i == 2)
            cout << "         ------";
        else if(i == 3)
            cout << "        x completed: " << scoreX << "%";
        else if(i == 4)
            cout << "       o completed: " << scoreO << "%";

        cout << endl;
    }
    cout << endl;

    return stream;
}












// [Menu Functions]

Hex * Hex::newGame()
{
    Hex * game = new Hex;
    
    game->playGame(false);    

    return game;
}

void Hex::continueGame(Hex * games)
{
    // for validty check
    const int lowerBound = 1;
    const int upperBound = getnumofGames();

    int game;

    Hex::gamesStatus(games);
    
    cout << endl << "Select a game to continue(ex: 2) " << endl;
    cout << "Enter: ";
    cin  >> game;

    
    // for validty check
    if( !isInputValid(game, lowerBound, upperBound) )
        return;

    games[game-1].playGame(true);
}

void Hex::compareGames(Hex * games)
{
    // for validty check
    const int lowerBound = 1;
    const int upperBound = getnumofGames();
    int game1, game2;

    Hex::gamesStatus(games);

    cout << endl << "Select two games to compare (ex: 2 1)" << endl;
    cout << "Enter: ";
    cin  >> game1 >> game2;

    // for validty check
    if(!isInputValid(game1, lowerBound, upperBound) || !isInputValid(game2, lowerBound, upperBound))
        return;

    game1 -= 1; // arr index start with 0 so we decrease selected games by 1
    game2 -= 1;

    if(games[game1] == games[game2])
        cout << endl << "[ Game" << game1+1 << " has same amount of moves with Game" << game2+1 << " ]\n" << endl;
    else if(games[game1] > games[game2])
        cout << endl << "[ Game" << game1+1 << " has more moves than Game" << game2+1 << " ]\n" << endl;
    else if(games[game1] < games[game2])
        cout << endl << "[ Game" << game2+1 << " has more moves than Game" << game1+1 << " ]\n" << endl;
}

void Hex::moveCounts(Hex * games)
{
    const int move = Hex::getTotalMove();
    
    Hex::gamesStatus(games);
    
    cout << "\t\t\t\t       +" << endl;
    cout << "\t\t\t\t       -----" << endl;
    cout << "\t\t     Total Move Count = " << move << endl << endl;
}

void Hex::printBoard(Hex * games)
{
    // for validty check
    const int lowerBound = 1;
    const int upperBound = getnumofGames();

    int game;

    Hex::gamesStatus(games);
    cout << endl << "Select the game you want to print(ex: 2)" << endl;
    cout << "Enter: ";
    cin  >> game;
    
    if(!isInputValid(game, lowerBound, upperBound))
        return;

    cout << games[game-1];
}

void Hex::commandScreen()
{
    cout << endl;
    cout << "\tCommands" << endl;
    cout << "\t--------" << endl;
    cout << "MENU           : goes to the menu screen  " << endl;
    cout << "BACK           : takes last two move back" << endl;
    cout << "SAVE ____.txt  : saves the current game. can be loaded anytime with LOAD command" << endl;
    cout << "LOAD ____.txt  : loads the game with given name." << endl;
}












// [Menu Helper Functions]

Hex * Hex::addGame(Hex * games, Hex * game)
{
    const int gameNumber = Hex::getnumofGames(); // getting number of existing game
    Hex::plusOneGame(); // adding one to it

    Hex * newGames = new Hex[gameNumber+1]; // allocating new Hex class object array

    for(int i=0; i<gameNumber; i++)
        newGames[i] = games[i]; // assigning existing games to new object array

    newGames[gameNumber] = *game; // assigning last one

    return newGames;
}

void Hex::gamesStatus(Hex * games)
{
    const int gameNumber = Hex::getnumofGames();


    cout << endl;
    cout << " \t     Board \t Opponent    Move Count" << endl
         << " \t     ----- \t --------    ----------" << endl;
    for(int i=0; i<gameNumber; i++)
    {
        cout << i+1 << ") Game" << i+1 << ' ';
        games[i].printStats();
    }
}

void Hex::printStats()
{
    const int size = getBoardSize();
    const int move = countMoves();

    string op;
    if(opponent == computer)
        op.append("computer");
    else
        op.append("player  ");

    cout << "    " << size << 'x' << size << "\t " << op << "\t "; 
    if(move < 10)
        cout << ' '<< move << endl;
    else if(move > 10)
        cout << move << endl;
    
}


// [Hex Class Deconstructor]
Hex::~Hex()
{/* Empty */}