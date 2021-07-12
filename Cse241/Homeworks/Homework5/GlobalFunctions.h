#ifndef GLOBALFUNCTIONS_H
#define GLOBALFUNCTIONS_H
#include "AbstractHex.h"
#include "HexVector.h"
#include "HexAdapter.h"
#include "HexArray1D.h"

extern int numOfVGames;
extern int numOfAGames;
extern int numOfTVGames;
extern int numOfTDGames;

namespace HexTugkan
{
    typedef HexAdapter<Cell,std::vector> HexAdapterV;
    typedef HexAdapter<Cell,std::deque> HexAdapterD;

    // [validty check]
    bool  isInputValid(const int input, const int lowerBound, const int upperBound);
    
    // [creates a new AbstractHex game and returns the address of it after user finishing or quiting the game]
    void  newGame       (HexVector * VectorGames, HexArray1D * ArrayGames, HexAdapterV * AdaptVGames, HexAdapterD * AdaptDGames);

    // [if user goes to the menu with or without finishing the game, game can be continued here]
    void  continueGame  (HexVector * VectorGames, HexArray1D * ArrayGames, HexAdapterV * AdaptVGames, HexAdapterD * AdaptDGames);  

    // [compares number of moves of two selected games]
    void  compareGames  (HexVector * VectorGames, HexArray1D * ArrayGames, HexAdapterV * AdaptVGames, HexAdapterD * AdaptDGames);

    // [only prints the selected board into the terminal]
    void  printBoard    (HexVector * VectorGames, HexArray1D * ArrayGames, HexAdapterV * AdaptVGames, HexAdapterD * AdaptDGames); 

    // [Returns true if selected game has a valid sequence of moves]
    bool isGameValid    (AbstractHex * Games[], const int & numOfGames);

    // [Menu helper function]
    void  gamesStatus   (HexVector * VectorGames, HexArray1D * ArrayGames, HexAdapterV * AdaptVGames, HexAdapterD * AdaptDGames);

    // [prints all possible commands to the screen]
    void  commandScreen (); 

} // HexTugkan

#endif