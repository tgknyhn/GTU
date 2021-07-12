#include <iostream>
#include <array>
#include <deque>
#include "GlobalFunctions.h"
#include "AbstractHex.h"
#include "HexVector.h"
#include "HexArray1D.h"
#include "HexExceptions.h"

using namespace HexTugkan;

typedef HexAdapter<Cell,std::vector> HexAdapterV;  // [Adapt Vector]
typedef HexAdapter<Cell,std::deque>  HexAdapterD;  // [Adapt Deque ]

int numOfVGames  = 0;  // [number of Vector games]
int numOfAGames  = 0;  // [number of Array  games]
int numOfTVGames = 0;  // [number of templated Vector games]
int numOfTDGames = 0;  // [number of templated Deque games]

int main()
{
    // Creating 4 size AbstractHex array for validty check
    AbstractHex * Games[4];
    HexVector   * v  = new HexVector   ("invalid1.txt");
    HexArray1D  * a  = new HexArray1D  ("valid1.txt");
    HexAdapterV * tv = new HexAdapterV ("valid2.txt");
    HexAdapterD * td = new HexAdapterD ("invalid2.txt");

    Games[0] = v; 
    Games[1] = a;
    Games[2] = tv;
    Games[3] = td;

    // Adding 2 games into VectorGames
    HexVector * VectorGames = new HexVector[10];
    VectorGames[0] = HexVector("game1.txt");
    VectorGames[1] = HexVector("game2.txt");
    // adding these 2 games to the game count
    for(int i=0; i<2; i++)
        numOfVGames += 1;
    
    // Adding 2 games into ArrayGames
    HexArray1D * ArrayGames = new HexArray1D[10];
    ArrayGames[0] = HexArray1D("game1.txt");
    ArrayGames[1] = HexArray1D("game3.txt");
    // adding these 2 games to the game count
    for(int i=0; i<2; i++)
        numOfAGames += 1;

    // Adding 2 games into Templated Vector Games
    HexAdapterV * AdaptVGames = new HexAdapterV[10];
    AdaptVGames[0] = HexAdapterV("game1.txt");
    AdaptVGames[1] = HexAdapterV("game4.txt");
    // adding these 2 games to the game count
    for(int i=0; i<2; i++)
        numOfTVGames += 1;

    // Adding 2 games into Templated Deque Games
    HexAdapterD * AdaptDGames = new HexAdapterD[10];
    AdaptDGames[0] = HexAdapterD("game1.txt");
    AdaptDGames[1] = HexAdapterD("game5.txt");
    // adding these 2 games to the game count
    for(int i=0; i<2; i++)
        numOfTDGames += 1;


    // menu loop
    int choice = 0;
    while(choice != 7)
    {
        cout << "\n              MENU            "   << endl
             << "--------------------------------"   << endl
             << "1) New Game"                        << endl
             << "2) Continue to an existing game"    << endl
             << "3) Comperision between two games"   << endl
             << "4) Only print the board of a game " << endl
             << "5) Is game valid                  " << endl
             << "6) In game commands               " << endl
             << "7) Exit                           " << endl << endl
             << "Enter: ";

        try
        {
            cin  >> choice;

            if(cin.fail() || choice < 1 || choice > 7)
                throw InvalidMenuSelection();

            if(choice == 1)
                newGame(VectorGames, ArrayGames, AdaptVGames, AdaptDGames);            
            
            else if(choice == 2)
                continueGame(VectorGames, ArrayGames, AdaptVGames, AdaptDGames);
            
            else if(choice == 3)
                compareGames(VectorGames, ArrayGames, AdaptVGames, AdaptDGames);
            
            else if(choice == 4)
                printBoard(VectorGames, ArrayGames, AdaptVGames, AdaptDGames);
            
            else if(choice == 5)
            {
                if(isGameValid(Games, 4) == true) // returns true if selected game's X and O numbers are equal
                    cout << "Selected game contains valid game sequence of moves" << endl;
                else
                    cout << "This game is not a valid game" << endl;
            }
            
            else if(choice == 6)
                commandScreen();
            
            else if(choice == 7)
            {
                cout << "\nGood Bye!\n" << endl;
                break;
            }
        }
        catch(InvalidMenuSelection)
        {
            cerr << "Error: Input is invalid." << endl;
            cin.clear();
            cin.ignore(128, '\n'); // ignoring until '\n' character
        }
    }

    return 0;
}