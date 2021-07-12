#ifndef HEXGAME_H
#define HEXGAME_H
#include <string>
#include <iostream>
#include <random>
#include <fstream>

using namespace std;

enum opponents  { user = 1, computer = 2 };
enum cellStates { empty = '.', letterX = 'x', letterO = 'o', bigLetterX = 'X', bigLetterO = 'O'};
enum turns      { user1 = 1, user2 = 2, ai = 2};

class Menu;

class Hex
{
public:
    class Cell
    {
    public:
        // constructors
        explicit Cell();
        explicit Cell(int row, int column, char state);

        void setState (const char state)    { cellState = state; }
        void setColumn(const int newColumn) { column = newColumn; }
        void setRow   (const int newRow)    { row = newRow; }
        // getters
        char getState () const { return cellState; }
        int  getColumn() const { return column; }      
        int  getRow()    const { return row; }

    private:

        int  column;    // holds column number of cell object
        int  row;       // holds row number of cell object                              
        char cellState; // there can be 3 state (which are empty, x, o)
    };

    // constructors
    Hex();             // no argument
    Hex(int size);     // allocates cells and route with given size
    Hex(string name);  // loads a game with the given name 

    // fundamental game functions      
    void playGame (bool isExistingGame);     // plays the game until someone wins or user goes to "MENU"
    Cell * play   ();                        // makes one move for the ai and returns played cell
    void   play   (int & row, int & column); // makes one move for the user
    void userInput(int & row, int & column);
    bool saveGame (string & name);
    bool loadGame (string & game);
    void moveBack ();

    // Menu functions
    static Hex * newGame      (); // creates a new Hex game and returns the address of it after user finishing or quiting the game
    static void  continueGame (Hex * games); // if user goes to the menu with or without finishing the game, game can be continued here 
    static void  compareGames (Hex * games); // compares number of moves of two selected games
    static void  moveCounts   (Hex * games); // prints out games statistics and total move count       
    static void  printBoard   (Hex * games); // only prints the selected board into the terminal
    static void  commandScreen(); // prints all possible commands to the screen

    // Menu helper functions
    static Hex * addGame     (Hex * games, Hex * game); // adds given game to the games object array and returns that new array back
    static void  gamesStatus (Hex * games);
    static void  plusOneGame () { numofGames += 1; }
    void         printStats  ();

    // overloaded operators
    Hex               operator -- (int); 
    Hex &             operator -- ();
    friend bool       operator >  (const Hex & game1, const Hex & game2); 
    friend bool       operator <  (const Hex & game1, const Hex & game2);
    friend bool       operator == (const Hex & game1, const Hex & game2);
    friend ifstream & operator >> (ifstream & stream, Hex & game); // copies all information to class object
    friend ofstream & operator << (ofstream & stream, Hex & game); // copies all necessary information about the game to the stream
    friend ostream  & operator << (ostream  & stream, Hex & game); // gives current board state to the given stream

    ~Hex(); // deletes dynamically allocated cells and route
private:    
    Cell ** hexCells;  // represent dot, x and o letters on the cell
    int  ** route;     // holds possible winning route. (used for win check)
    int  ** playedPath;// holds every play that has been done on the game. (used for roll back)

    // initializers
    void initializeGame  (); // asks size for board and opponent and allocates space for hexCells according to that size
    Cell ** allocateCells(); // creates dynamically allocated 2D cell objects with the size of boardSize class member
    int  ** allocateRoute(); // creates dynamically allocated 2D int array with the size of MAX_POSSIBLE_MOVE which is (size*size) / 2.

    // win check funtions
    bool isGameOver();
    void searchRoute(int index);

    // win check helper functions
    const bool isPathCompleted  (int index);
    const bool isNeighborFriend (int index, int neighbor);
    const bool isOnRoute        (int & x, int & y, int & index);
    const bool isEmpty          (int & y, int & x);
    void cleanRoute       ();
    void convertToUpper   ();
    void xWon             ();
    void oWon             ();

    // ai move
    void goRandom();

    // score functions
    const int getScoreX(); // returns score of x letter
    const int getScoreO(); // returns score of o letter

    // validty checker
    friend const bool isInputValid(const int input, const int lowerBound, const int upperBound);

    // setters && incrementers && decrementers
    void setBoardSize (const int & size)  { boardSize = size; } 
    void setOpponent  (const int & op)    { opponent = op;    }
    void setRow       (const int & y)     { row = y;          }
    void setColumn    (const int & x)     { column = x;       }
    void changeTurn   ()                  { turnOf = (turnOf == 1) ? 2:1 ; }
    void setFinish    (const bool isIt)   { isThisFinished = isIt; }
    void setName      (const string name) { gameName = name; }
    void setTurn      (const int & turn)  { turnOf = turn;   }
    void plusMove     (const int num)     { move += num; totalMove += num; }
    void minusMove    (const int num)     { move -= num; totalMove -= num; }
    void setGoTo      (const bool go)     { gotoMenu = go; }

    // getters
    const int  getBoardSize() const  { return boardSize; } 
    const int  getOpponent () const  { return opponent;  }
    const int  getTurn     () const  { return turnOf;    }
    const int  getRow      () const  { return row;       }
    const int  getColumn   () const  { return column;    }
    const bool getFinish   () const  { return isThisFinished; }
    const char getColumn   (char ignoreME) const { return ( (column < 26)? column+64:column+70 ); }
    const string getName   () const  { return gameName;  }
    const int getMove      () const  { return move;      }
    int countMoves   () const;
    const int getGoTo      () const  { return gotoMenu;   }
    const static int getTotalMove () { return totalMove; }
    const static int getnumofGames() { return numofGames; }


    int  row, column;    // they hold placed cell location
    int  boardSize;      // holds the board size of the Hex object
    int  opponent;       // tells who the user playing against
    int  turnOf = user2; // shows which user has the turn
    bool isThisFinished = false; // will be true if the game is finished  
    bool gotoMenu = false; // initially will be false but if user enters MENU while in the game, it will finish playing game. 
    string gameName;       // every hex class object should have a name
    
    int move = 0;
    static int totalMove;
    static int numofGames;
};

#endif