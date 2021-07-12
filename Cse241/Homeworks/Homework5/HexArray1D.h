#ifndef HEXARRAY1D_H
#define HEXARRAY1D_H
#include "AbstractHex.h"

using namespace std;

namespace HexTugkan
{
    class HexArray1D : public AbstractHex
    {
    public:
        HexArray1D();
        HexArray1D(int size);
        HexArray1D(string name);
        
        virtual Cell operator () (const int & row, const int & column);

        // fundamental game functions      
        virtual void playGame      (bool isExistingGame);     // plays the game until someone wins or user goes to "MENU"
        virtual Cell * play        ();                        // makes one move for the ai and returns played cell
        virtual void   play        (int & row, int & column); // makes one move for the user
        virtual bool   writeToFile (string & name);
        virtual bool   readFromFile(string & game);
        virtual void   print       (); // prints out current board
        virtual void   reset       (); // resets board cells into empty state

        virtual ~HexArray1D();
    private:
        // ex:(for boardsize = 5) hexCells[3][2], this  2D array is equivalent in 1D hexCells[12] and this function returns that 12  
        int  getIndex(const int & row, const int & column) { return ((row-1)*boardSize)+column; }; 
        Cell * hexCells;  // represent dot, x and o letters on the cell


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

} // HexTugkan

#endif