#include "GlobalFunctions.h"
#include "AbstractHex.h"

namespace HexTugkan
{
    // [AbstractHex constructors]

    AbstractHex::AbstractHex()
    { /* empty */ }

    AbstractHex::AbstractHex(int size) : boardSize(size)
    { /* empty */ }

    AbstractHex::AbstractHex(string name)
    { /* empty */ }



    // [Cell constructors]

    Cell::Cell() : cellState(empty)
    { /* cell will be initialized to empty state and row - column numbers will be 0 */ }

    Cell::Cell(char state) : cellState(state)
    { /* unless state is given, cell will be initialized to given state. also row and column has to be explicitly given. */ }



    // [Overloaded Operators]
    Cell AbstractHex::operator () (const int & row, const int & column)
    { return *temp; }


    bool AbstractHex::operator == (AbstractHex & rHex)
    { 
        const int size1 = this->getBoardSize(); 
        const int size2 = rHex.getBoardSize();

        int allCells = size1*size2;
        int count = 0;

        if(size1 != size2) 
            return false;
        else
            for(int i=1; i<=size1; i++)
                for(int j=1; j<=size1; j++)
                {
                    //cout << "i:" << i << " j:" << j << " state1:" << (*this)(i,j).getState() << " state2:" << (rHex)(i,j).getState() << endl;
                    if((*this)(i,j).getState() == (rHex)(i,j).getState())
                        count++;
                }
        //cout << "size1: " << size1 << " size2: " << size2 << " count: " << count << endl;

        if(count != allCells)
            return false;
        else
            return true;
    }












    //  [Initializiers] 

    Cell * AbstractHex::allocateCells()
    { return temp; }

    int ** AbstractHex::allocateRoute()
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

    void AbstractHex::initializeGame()
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

    void AbstractHex::playGame(bool isExistingGame) 
    { /* virtual */ }

    Cell * AbstractHex::play()
    { return temp; }

    void AbstractHex::play(int & row, int & column)
    { /* virtual */ }

    void AbstractHex::userInput(int & row, int & column)
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

                if(column_s == "SAVE" && writeToFile(fileName) == false)
                    cerr << "Error: Game \"" << fileName << "\" couldn't saved. Name is invalid." << endl;
                else if(column_s == "LOAD")
                {
                    if(readFromFile(fileName) == true)
                        cout << endl << "Game Loaded!" << endl;
                    else
                        cerr << "Error: Game \"" << fileName << "\" is not a valid game." << endl; 
                }

                minusMove(1);
                changeTurn();
                return;
            }
            else if(column_s == "MENU")
            {
                setGoTo(true); // to break isEnd() loop in the playGame()
                minusMove(1);  // so it doesn't count this as a move

                cout << endl << "Returning to the menu..." << endl;
                
                return;
            }
            else if(column_s == "RESET")
            {

                minusMove(1);  // so it doesn't count this as a move
                const int moves = numberOfMoves(); 
                minusMove(moves); // resetting a game means substracting all moves
                reset();
                print();
                setTurn(user1);
                continue;

            }
            else if(column_s == "R_SIZE")
            {
                cin >> row_s;
                row_i = stoi(row_s);
                minusMove(1); // doesn't count as a move
                const int moves = numberOfMoves(); 
                minusMove(moves); // resetting a game means substracting all moves
                try
                {
                    setSize(row_i);
                    setTurn(user1);
                }
                catch(InvalidSize)
                {
                    cerr << "Error: Invalid size. Size must be between 5 and 52" << endl;
                }
                continue;
            }
            else if(column_s == "LASTMOVE")
            {
                minusMove(1); // doesn't count as a move
                try
                {
                    char * last = LastMove();
                    cout << "Last 2 move was (" << last[0] << (int)last[1] << ") and (" << last[2] << (int)last[3] << ")" << endl; 
                }
                catch(NotEnoughMove)
                {
                    cerr << "There aren't enough moves on the board" << endl;
                }
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

    bool AbstractHex::writeToFile(string & name)
    { return false; }

    bool AbstractHex::readFromFile(string & name)
    { return false; }

    int AbstractHex::countMoves() // returns number of moves
    { return 0; }

    int AbstractHex::numOfX()
    { 
        int count = 0;
        const int size = getBoardSize(); 
        
        for(int i=1; i<size+1; i++)
            for(int j=1; j<size+1; j++)
                if((*this)(i,j).getState() == letterX || (*this)(i,j).getState() == bigLetterX)
                    count++;
        
        return count;
     }

    int AbstractHex::numOfO()
    { 
        int count = 0;
        const int size = getBoardSize(); 
        
        for(int i=1; i<size+1; i++)
            for(int j=1; j<size+1; j++)
                if((*this)(i,j).getState() == letterO || (*this)(i,j).getState() == bigLetterO)
                    count++;
        
        return count; 
    }

    void AbstractHex::print()
    { /* virtual */ }

    void AbstractHex::reset()
    { /* virtual */ }


    void AbstractHex::setSize(const int & size)
    {
        if(size < 5 || size > 52)
            throw InvalidSize(); 

        setBoardSize(size);
        allocateCells();
        allocateRoute();
        reset();
        print();
    }












    // [AI Moves] 

    void AbstractHex::goRandom() 
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
        const int currIndex = numberOfMoves() - 1;
        playedPath[0][currIndex] = y;
        playedPath[1][currIndex] = x;
    }












    // [Score Functions]

    int AbstractHex::getScoreX()
    { return false; }


    int AbstractHex::getScoreO()
    { return false; }











    // [Last Move]

    char * AbstractHex::LastMove() 
    {
        last = new char[4];

        if(countMoves() < 2)
        {
            throw NotEnoughMove();            
        }
        else
        {
            setColumn(playedPath[1][move-1]);
            last[0] = getColumn('a');
            setColumn(playedPath[1][move-2]);
            last[2] = getColumn('a');

            last[1] = playedPath[0][move-1];
            last[3] = playedPath[0][move-2];
        }

        return last;
    }

    // isValid

    bool AbstractHex::isValid()
    {
        const int size = getBoardSize();

        for(int i=1; i<=size; i++)
            for(int j=1; j<=size; j++)
            {
                Cell temp   = (*this)(i,j);
                char c      = temp.getState();
                if(c != letterX && c != bigLetterX && c != letterO && c != bigLetterO && c != empty)
                    return false;
            }

        if(numOfO() != numOfX() && numOfO() != numOfX()+1 && numOfO() != numOfX()-1)
            return false;

        return true;
    }
    










    // [Win Check Functions]

    bool AbstractHex::isEnd()
    {
        int index = 1;
        searchRoute(index);
        
        return getFinish();
    }

    void AbstractHex::searchRoute(int index)
    { /* virtual */ }













    // [Win Check Helper Functions]

    bool AbstractHex::isPathCompleted(int index)
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

    bool AbstractHex::isNeighborFriend(int index, int neighbor)
    { return false; }

    bool AbstractHex::isOnRoute(int & x, int & y, int & index) // if given coords is on the route returns true. if not returns false
    {
        for(int i=1; i<=index; i++)
            if(route[0][i] == y && route[1][i] == x)
                return true;

        return false;
    }

    bool AbstractHex::isEmpty(int & y, int & x) // return true if given cell is empty '.'
    { return false; }

    void AbstractHex::cleanRoute()
    {
        delete []route[0]; // deleting route[0][MAX_POSSIBLE_MOVE]
        delete []route[1]; // deleting route[1][MAX_POSSIBLE_MOVE]
        delete []route ; // deleting route itself

        route = allocateRoute(); // reallocating
    }

    void AbstractHex::convertToUpper() // converts letters to uppercase for the winning route
    { /* virtual */ }

    void AbstractHex::xWon()
    {
        convertToUpper();
        cout << "\n-------------------------------------------\n" << endl;
        print();
        cout << endl;
        cout << "User1 won the game!" << endl << endl;
    }

    void AbstractHex::oWon()
    {
        convertToUpper();
        cout << "\n-------------------------------------------\n" << endl;
        print();
        cout << endl;
        if(opponent == user)
            cout << "User2 won the game!" << endl << endl;
        else 
            cout << "Computer won the game!" << endl << endl;
    }












    void AbstractHex::printStats()
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
        else if(move >= 10)
            cout << move << endl;
        
    }


    // [AbstractHex Class Deconstructor]
    AbstractHex::~AbstractHex()
    {/* Empty */}

} // HexTugkan
