#ifndef HEXADAPTER_H
#define HEXADAPTER_H
#include <vector>
#include <deque>
#include <vector>
#include "AbstractHex.h"

using namespace std;

namespace HexTugkan
{
    template<typename E, template<typename, typename=allocator<E>> class T>
    class HexAdapter : public AbstractHex
    {
    public:
        HexAdapter();
        HexAdapter(int size);
        HexAdapter(string name);
        
        virtual Cell operator () (const int & row, const int & column);
        
        // fundamental game functions      
        virtual void playGame      (bool isExistingGame);     // plays the game until someone wins or user goes to "MENU"
        virtual Cell * play        ();                        // makes one move for the ai and returns played cell
        virtual void   play        (int & row, int & column); // makes one move for the user
        virtual bool   writeToFile (string & name);
        virtual bool   readFromFile(string & game);
        virtual void   print       (); // prints out current board
        virtual void   reset       (); // resets board cells into empty state

    private:
        T<E> hexCells;
        int  getIndex(const int & row, const int & column) { return ((row-1)*boardSize)+column; }; 

        // win check functions
        virtual void searchRoute(int index);

        // win check helper functions
        virtual bool isNeighborFriend (int index, int neighbor);
        virtual bool isEmpty          (int & y, int & x);
        virtual void convertToUpper   ();


        // score functions
        virtual int getScoreX(); // returns score of x letter
        virtual int getScoreO(); // returns score of o letter

        // getters
        virtual int countMoves();

        // initializers
        virtual Cell * allocateCells(); // creates dynamically allocated 2D cell objects with the size of boardSize class member


    };

    #endif

    // constructors
    template<typename E, template<typename, typename=allocator<E>> class T>
    HexAdapter<E,T>::HexAdapter() : AbstractHex()
    {
        *this = HexAdapter(52);
    }


    template<typename E, template<typename, typename=allocator<E>> class T>
    HexAdapter<E,T>::HexAdapter(int size) : AbstractHex(size)
    { 
        allocateCells();
        route      = allocateRoute();
        playedPath = allocateRoute();

    }

    template<typename E, template<typename, typename=allocator<E>> class T>
    HexAdapter<E,T>::HexAdapter(string name) : AbstractHex(name)
    { 
        *this = HexAdapter(52);
        if(readFromFile(name) == false)
            cerr << "Error: Initially added game couldn't added. Please check if your giving a valid .txt file." << endl;
    }







    // [Overloaded Operators]

    template<typename E, template<typename, typename=allocator<E>> class T>
    Cell HexAdapter<E,T>::operator () (const int & row, const int & column)
    {
        if(row < 1 || row > getBoardSize())
            throw OutOfBound();

        if(column < 1 || column > getBoardSize())
            throw OutOfBound();

        return hexCells[getIndex(row,column)];
    }











    // Initializers

    template<typename E, template<typename, typename=allocator<E>> class T>
    Cell * HexAdapter<E,T>::allocateCells()
    {
        const int size = getBoardSize();
        // allocating +3 Cell then normally to excape seg. fault 
        for(int i=0; i<((size+3)*(size+2)); i++)
            hexCells.push_back(Cell(empty));

        Cell * cells; // just for function declaration not used
        return cells; // same.. not used
    }









    // [Fundamental play functions]

    template<typename E, template<typename, typename=allocator<E>> class T>
    void HexAdapter<E,T>::playGame(bool isExistingGame) 
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

    template<typename E, template<typename, typename=allocator<E>> class T>
    Cell * HexAdapter<E,T>::play()
    {
        goRandom(); //finds valid row and column and assigns it

        int y = getRow();    
        int x = getColumn(); 

        cout << "Computers turn..." << endl << endl; 
        if(x < 26)
            cout << "[ Computer placed to (" << static_cast<char>(x+64) << y << ") ]" << endl;
        else
            cout << "[ Computer placed to (" << static_cast<char>(x+70) << y << ") ]" << endl;

        return &hexCells[getIndex(y,x)];
    }

    template<typename E, template<typename, typename=allocator<E>> class T>
    void HexAdapter<E,T>::play(int & row, int & column)
    {
        setColumn(column); 
        setRow(row);     

        // if user gives a valid location it will be added to the playedPath
        const int currIndex = numberOfMoves() - 1;
        playedPath[0][currIndex] = row;
        playedPath[1][currIndex] = column;
        
        char letter = (getTurn() == 1)? letterX:letterO;   // getting right letter before assigning to a cell
        hexCells[getIndex(row,column)] = Cell(letter); // initializing hexCell with right letter

        cout << endl << "[ User" << getTurn() << " placed to (" << getColumn(' ') << getRow() << ") ]" << endl;
    }

    template<typename E, template<typename, typename=allocator<E>> class T>
    bool HexAdapter<E,T>::writeToFile(string & name)
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
                stream << hexCells[getIndex(i,j)].getState();
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

    template<typename E, template<typename, typename=allocator<E>> class T>
    bool HexAdapter<E,T>::readFromFile(string & name)
    {
        ifstream stream;
        stream.open(name);

        if(stream.fail())
            return false;

        // substracting all move count before loading a game 
        int moves = countMoves();
        minusMove(moves);
        
        HexAdapter * newGame = new HexAdapter(52);

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
                newGame->hexCells[newGame->getIndex(i,j)].setState(c); 
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

    template<typename E, template<typename, typename=allocator<E>> class T>
    int HexAdapter<E,T>::countMoves() // returns number of moves
    {
        if(gameName.size() == 0)
            readFromFile(gameName);

        int count = 0;
        const int size = getBoardSize(); 
        
        for(int i=1; i<size+1; i++)
            for(int j=1; j<size+1; j++)
                if(hexCells[getIndex(i,j)].getState() != empty)
                    count++;
        
        return count;
    }

    template<typename E, template<typename, typename=allocator<E>> class T>
    void HexAdapter<E,T>::print()
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
                cout << hexCells[getIndex(i,j)].getState() << ' ';

            
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

    template<typename E, template<typename, typename=allocator<E>> class T>
    void HexAdapter<E,T>::reset()
    {
        const int size = getBoardSize();

        for(int i=1; i<=size; i++)
            for(int j=1; j<=size; j++)
                hexCells[getIndex(i,j)].setState(empty);

    }









    // [Score Functions]

    template<typename E, template<typename, typename=allocator<E>> class T>
    int HexAdapter<E,T>::getScoreX()
    {
        int score = 0;
        bool isFinished;

        const int size = getBoardSize();

        //if there is a big letter returns 100 else assigns isFinished to false
        for(int i=1; i<size+1; i++)
        {
            if(hexCells[getIndex(i,1)].getState() == bigLetterX)
                return 100;
            else
                isFinished = false;        
        }

        // if inner loop finds any letterX in the same column then increases score and breaks the loop for that column 
        for(int i=1; i<size+1; i++)
            for(int j=1; j<size+1; j++)
                if(hexCells[getIndex(j,i)].getState() == letterX)
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

    template<typename E, template<typename, typename=allocator<E>> class T>
    int HexAdapter<E,T>::getScoreO()
    {
        int score = 0;
        bool isFinished;

        const int size = getBoardSize();

        //if there is a big letter returns 100%
        for(int i=1; i<size+1; i++)
        {
            if(hexCells[getIndex(1,i)].getState() == bigLetterO)
                return 100;
            else
                isFinished = false;        
        }
        
        // if inner loop finds any letterO in the same row then increases score and breaks the loop for that row 
        for(int i=1; i<size+1; i++)
            for(int j=1; j<size+1; j++)
                if(hexCells[getIndex(i,j)].getState() == letterO)
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

    template<typename E, template<typename, typename=allocator<E>> class T>
    void HexAdapter<E,T>::searchRoute(int index)
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
                if(hexCells[getIndex(i,index)].getState() == letterX && turn == user1)
                {
                    route[1][index] = 1;
                    route[0][index] = i;
                    searchRoute(index+1);
                }
                else if(hexCells[getIndex(index,i)].getState() == letterO && turn == user2)
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

    template<typename E, template<typename, typename=allocator<E>> class T>
    bool HexAdapter<E,T>::isNeighborFriend(int index, int neighbor)
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
        int tmp = getIndex(currY, currX);

        if(tmp < 0)
            tmp = 0;
        
        int currState = hexCells[tmp].getState();

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

    template<typename E, template<typename, typename=allocator<E>> class T>
    bool HexAdapter<E,T>::isEmpty(int & y, int & x) // return true if given cell is empty '.'
    {
        int size = getBoardSize();

        if( (x <= size && y <= size) && (x >= 1 && y >= 1) )
            if( hexCells[getIndex(y,x)].getState() == empty)
                return true;

        return false;
    }

    template<typename E, template<typename, typename=allocator<E>> class T>
    void HexAdapter<E,T>::convertToUpper() // converts letters to uppercase for the winning route
    {
        const int currTurn  = getTurn();
        int x, y, i=1; 

        while(route[0][i] != 0)
        {
            x = route[1][i]; // getting route's column values
            y = route[0][i]; // getting route's row values

            if(currTurn == user1)
                hexCells[getIndex(y,x)] = Cell(bigLetterX); // assignin 'O' letter to gotten cell 
            else
                hexCells[getIndex(y,x)] = Cell(bigLetterO); // assignin 'X' letter to gotten cell
            
            i++;
        }
    }

} // HexTugkan




