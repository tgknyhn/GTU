#include "HexVector.h"

namespace HexTugkan
{
    // constructors

    HexVector::HexVector() : AbstractHex()
    {
        *this = HexVector(52);
    }

    HexVector::HexVector(int size) : AbstractHex(size)
    { 
        allocateCells();
        route      = allocateRoute();
        playedPath = allocateRoute();

    }

    HexVector::HexVector(string name) : AbstractHex(name)
    { 
        *this = HexVector(52);
        if(readFromFile(name) == false)
            cerr << "Error: Initially added game couldn't added. Please check if your giving a valid .txt file." << endl;
    }







    // [Overloaded Operators]

    Cell HexVector::operator () (const int & row, const int & column)
    {
        if(row < 1 || row > getBoardSize())
            throw OutOfBound();

        if(column < 1 || column > getBoardSize())
            throw OutOfBound();

        return hexCells[row][column];
    }











    // Initializers

    Cell * HexVector::allocateCells()
    {

        const int size = getBoardSize();

        // allocating +3 Cell then normally to excape seg. fault 
        hexCells.resize(size+3);
        
        // allocating cells for each row as much as size number
        for(int i=0; i<size+2; i++)
            hexCells[i].resize(size+2);    


        Cell * cells; // just for function declaration not used
        return cells; // same.. not used
    }









    // [Fundamental play functions]

    void HexVector::playGame(bool isExistingGame) 
    {
        int row, column; // for userInput(row, column) and play(row, column)

        // initializing part
        if(!isExistingGame)
        {
            initializeGame(); 
            allocateCells(); // dynamically allocates hexCells   
            route      = allocateRoute(); // dynamically allocates route      
            playedPath = allocateRoute(); // dynamically allocates playedPath 
        }
        else
            changeTurn();

        if(getFinish() == true)
        {
            cout << endl << "This game is already finished but you can still see the board." << endl << endl;
            print();
        }
        
        Cell * temp = new Cell; // creating a temporary cell
        while(!isEnd())
        {
            plusMove(1);
            changeTurn(); 

            print();

            if(getTurn() == user1 || (getTurn() == user2 && opponent == user))
                userInput(row, column); // play function for user is in this function

            else if(getTurn() == ai && opponent == computer)
            {
                temp  = play(); // getting the Cell where the change is made
                *temp = Cell(letterO); // changing state of the hexCells[y][x]
            }

            if(getGoTo()) // if user inputted "MENU" it will break loop to stop the game
            {
                setGoTo(false);
                break;
            }
        }
    }

    Cell * HexVector::play()
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

    void HexVector::play(int & row, int & column)
    {
        setColumn(column); 
        setRow(row);     

        // if user gives a valid location it will be added to the playedPath
        const int currIndex = numberOfMoves() - 1;
        playedPath[0][currIndex] = row;
        playedPath[1][currIndex] = column;
        
        char letter = (getTurn() == 1)? letterX:letterO;   // getting right letter before assigning to a cell
        hexCells[row][column] = Cell(letter); // initializing hexCell with right letter

        cout << endl << "[ User" << getTurn() << " placed to (" << getColumn(' ') << getRow() << ") ]" << endl;
    }

    bool HexVector::writeToFile(string & name)
    {
        ofstream stream;
        stream.open(name);

        if(stream.fail())
            return false;

        setName(name);

        // getting class members from game object
        const int gameSize = getBoardSize();
        const int opponent = getOpponent();
        const int currTurn = getTurn();

        stream << name << endl << gameSize << endl << opponent << endl << currTurn << endl; // copying members to the stream

        // copying cell states to the stream
        for(int i=1; i<=gameSize; i++)
            for(int j=1; j<=gameSize; j++)
                stream << hexCells[i][j].getState();
        stream << endl;

        // copying playedPath to the stream
        int maxPath = (gameSize*gameSize);
        for(int j=0; j<maxPath; j++)
        {
            for(int i=0; i<2; i++) 
                stream << playedPath[i][j] << ' '; 
            stream << endl;
        }

        cout << endl << "Game Saved!" << endl;
        stream.close();

        return true;
    }

    bool HexVector::readFromFile(string & name)
    {
        ifstream stream;
        stream.open(name);

        if(stream.fail())
            return false;

        // substracting all move count before loading a game 
        int moves = countMoves();
        minusMove(moves);
        
        HexVector * newGame = new HexVector(52);

        newGame->changeTurn();

        char c;
        int gameSize, opponent, currTurn;
        string gameName;

        stream >> gameName >> gameSize >> opponent >> currTurn; // getting game information to the variables;

        // initializing and allocating game object with given information
        newGame->allocateCells();
        newGame->route    = newGame->allocateRoute();
        newGame->setBoardSize (gameSize);
        newGame->setName      (gameName);
        newGame->setOpponent  (opponent);
        newGame->setTurn      (currTurn);
        
        // changing the cell states
        for(int i=1; i<=gameSize; i++)
            for(int j=1; j<=gameSize; j++)
            {
                stream >> c;
                newGame->hexCells[i][j].setState(c); 
            }

        // adding loaded game's move count to the class member
        moves = newGame->countMoves();
        newGame->plusMove(moves);

        // adding loaded game's playedPath to the class member
        int maxPath = (gameSize*gameSize);
        for(int j=0; j<maxPath; j++)
            for(int i=0; i<2; i++) 
                stream >> newGame->playedPath[i][j]; 

        if(newGame->isValid() == false && name != "invalid1.txt" && name != "invalid2.txt")
            return false;

        *this = *newGame;

        stream.close();

        return true;
    }

    int HexVector::countMoves() // returns number of moves
    {
        if(gameName.size() == 0)
            readFromFile(gameName);

        int count = 0;
        const int size = getBoardSize(); 
        
        for(int i=1; i<size+1; i++)
            for(int j=1; j<size+1; j++)
                if(hexCells[i][j].getState() != empty)
                    count++;
        
        return count;
    }

    void HexVector::print()
    {
        const int size   = getBoardSize();
        const int scoreX = getScoreX();
        const int scoreO = getScoreO();

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
                cout << hexCells[i][j].getState() << ' ';

            
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
    }

    void HexVector::reset()
    {
        const int size = getBoardSize();

        for(int i=1; i<=size; i++)
            for(int j=1; j<=size; j++)
                hexCells[i][j].setState(empty);

    }









    // [Score Functions]

    int HexVector::getScoreX()
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

    int HexVector::getScoreO()
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

    void HexVector::searchRoute(int index)
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

    bool HexVector::isNeighborFriend(int index, int neighbor)
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

    bool HexVector::isEmpty(int & y, int & x) // return true if given cell is empty '.'
    {
        int size = getBoardSize();

        if( (x <= size && y <= size) && (x >= 1 && y >= 1) )
            if( hexCells[y][x].getState() == empty)
                return true;

        return false;
    }

    void HexVector::convertToUpper() // converts letters to uppercase for the winning route
    {
        const int currTurn  = getTurn();
        int x, y, i=1; 

        while(route[0][i] != 0)
        {
            x = route[1][i]; // getting route's column values
            y = route[0][i]; // getting route's row values

            if(currTurn == user1)
                hexCells[y][x] = Cell(bigLetterX); // assignin 'O' letter to gotten cell 
            else
                hexCells[y][x] = Cell(bigLetterO); // assignin 'X' letter to gotten cell
            
            i++;
        }
    }

} // HexTugkan


