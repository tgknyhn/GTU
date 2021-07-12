#ifndef ABSTRACTHEX_H
#define ABSTRACTHEX_H
#include <string>
#include <iostream>
#include <random>
#include <fstream>
#include <memory>
#include "HexExceptions.h"
#include "Cell.h"
using namespace std;

enum opponents  { user = 1, computer = 2 };
enum cellStates { empty = '.', letterX = 'x', letterO = 'o', bigLetterX = 'X', bigLetterO = 'O'};
enum turns      { user1 = 1, user2 = 2, ai = 2};

namespace HexTugkan
{
    class AbstractHex
    {
    public:
        // constructors
        AbstractHex();             // no argument
        AbstractHex(int size);     // allocates cells and route with given size
        AbstractHex(string name);  // loads a game with the given name 

        virtual Cell operator () (const int & row, const int & column);
        bool operator == (AbstractHex & rHex);

        // fundamental game functions      
        virtual void playGame     (bool isExistingGame);     // plays the game until someone wins or user goes to "MENU"
        virtual Cell * play       ();                        // makes one move for the ai and returns played cell
        virtual void   play       (int & row, int & column); // makes one move for the user
        virtual bool writeToFile  (string & name);
        virtual bool readFromFile (string & game);
        virtual void print        (); // prints out current board
        virtual void reset        (); // resets board cells into empty state
        void userInput    (int & row, int & column);
        void setSize      (const int & size); // sets board size and resets the current cell states
        void printStats   ();

        int getBoardSize () const  { return boardSize; } 
        int numOfX       (); // returns number of X on the board
        int numOfO       (); // returns number of O on the board
        bool isValid     ();
        
        virtual ~AbstractHex(); // deletes dynamically allocated cells and route
    protected:
        
        int  ** route;     // holds possible winning route. (used for win check)
        int  ** playedPath;// holds every play that has been done on the game. (used for roll back)

        // initializers
        void initializeGame  (); // asks size for board and opponent and allocates space for hexCells according to that size
        int  ** allocateRoute(); // creates dynamically allocated 2D int array with the size of MAX_POSSIBLE_MOVE which is (size*size) / 2.
        virtual Cell * allocateCells(); // creates dynamically allocated 2D cell objects with the size of boardSize class member

        // win check funtions
        bool isEnd();
        virtual void searchRoute(int index);

        // win check helper functions
        virtual bool isNeighborFriend (int index, int neighbor);
        virtual bool isEmpty          (int & y, int & x);
        virtual void convertToUpper   ();
        bool isPathCompleted  (int index);
        bool isOnRoute        (int & x, int & y, int & index);
        void cleanRoute       ();
        void xWon             ();
        void oWon             ();

        // ai move
        void goRandom();

        // score functions
        virtual int getScoreX(); // returns score of x letter
        virtual int getScoreO(); // returns score of o letter

        // validty checker
        friend bool isInputValid(const int input, const int lowerBound, const int upperBound);

        // setters && incrementers && decrementers
        void setBoardSize (const int & size)  { boardSize = size; } 
        void setOpponent  (const int & op)    { opponent = op;    }
        void setRow       (const int & y)     { row = y;          }
        void setColumn    (const int & x)     { column = x;       }
        void changeTurn   ()                  { turnOf = (turnOf == 1) ? 2:1 ; }
        void setFinish    (const bool isIt)   { isThisFinished = isIt; }
        void setName      (const string name) { gameName = name; }
        void setTurn      (const int & turn)  { turnOf = turn;   }
        void plusMove     (const int num)     { move += num; }
        void minusMove    (const int num)     { move -= num; }
        void setGoTo      (const bool go)     { gotoMenu = go; }

        // getters
        int  getOpponent () const  { return opponent;  }
        int  getTurn     () const  { return turnOf;    }
        int  getRow      () const  { return row;       }
        int  getColumn   () const  { return column;    }
        bool getFinish   () const  { return isThisFinished; }
        char getColumn   (char ignoreME) const { return ( (column < 26)? column+64:column+70 ); }
        string getName   () const  { return gameName;  }
        int numberOfMoves() const  { return move;      }
        int getGoTo      () const  { return gotoMenu;   }
        char * LastMove  ();
        virtual int countMoves();

        char * last;
        int  row, column;    // they hold placed cell location
        int  boardSize;      // holds the board size of the AbstractHex object
        int  opponent;       // tells who the user playing against
        int  turnOf = user2; // shows which user has the turn
        bool isThisFinished = false; // will be true if the game is finished  
        bool gotoMenu = false; // initially will be false but if user enters MENU while in the game, it will finish playing game. 
        string gameName;       // every AbstractHex class object should have a name

        Cell * temp;

        int move = 0;
    };

} // HexTugkan

#endif