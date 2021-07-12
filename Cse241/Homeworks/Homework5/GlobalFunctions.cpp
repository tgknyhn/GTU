#include "HexExceptions.h"
#include "GlobalFunctions.h"

namespace HexTugkan
{
    typedef HexAdapter<Cell,std::vector> HexAdapterV;
    typedef HexAdapter<Cell,std::deque> HexAdapterD;

    // [Validity Checker]

    bool isInputValid(const int input, const int lowerBound, const int upperBound) 
    {
        try
        {
            if(cin.fail())
                throw InputIsInvalid();

            else if(input < lowerBound || input > upperBound)
                throw BoundIsInvalid();

            return true;
        }
        catch(InputIsInvalid)
        {
            cout << endl << "Error! Invalid input." << endl;
            cin.clear();
            cin.ignore(128, '\n'); // ignoring until '\n' character
            return false;
        }
        catch(BoundIsInvalid)
        {
            cout << endl << "Error! A valid input should be between " << lowerBound << '-' << upperBound << '.' << endl; 
            return false;
        }

    }

    // [Menu Functions]

    void newGame(HexVector * VectorGames, HexArray1D * ArrayGames, HexAdapterV * AdaptVGames, HexAdapterD * AdaptDGames)
    {    
        int op;
        cout << "Choose your Cell representation:" << endl;
        cout << "1) Vector" << endl;
        cout << "2) 1D Array" << endl;
        cout << "3) Templated Vector" << endl;
        cout << "4) Templated Deque" << endl;
        cout << "Enter: ";
        cin >> op;
        switch (op)
        {
        case 1:
        {
            // allocating new Hex class object
            shared_ptr<HexVector> VectorGame(new HexVector);
            // assigning VectorGame as a last added game
            VectorGames[numOfVGames] = *VectorGame;
            // finally, play the added game
            VectorGames[numOfVGames].playGame(false);    
            // adding one to the number of vector games
            numOfVGames += 1;
            break;
        }
        case 2:
        {
            // allocating new Hex class object
            shared_ptr<HexArray1D> ArrayGame(new HexArray1D);
            // assigning ArrayGame as a last added game
            ArrayGames[numOfAGames] = *ArrayGame;
            // finally, play the added game
            ArrayGames[numOfAGames].playGame(false);    
            // adding one to the number of array games
            numOfVGames += 1;
            break;
        }
        case 3:
        {
            // allocating new Hex class object
            shared_ptr<HexAdapterV> AdaptVGame(new HexAdapterV);
            // assigning templated vector game as a last added game
            AdaptVGames[numOfTVGames] = *AdaptVGame;
            // finally, play the added game
            AdaptVGames[numOfTVGames].playGame(false);    
            // adding one to the number of array games
            numOfTVGames += 1;
            break;
        }
        case 4:
        {
            // allocating new Hex class object
            shared_ptr<HexAdapterD> AdaptDGame(new HexAdapterD);
            // assigning templated deque game as a last added game
            AdaptDGames[numOfTDGames] = *AdaptDGame;
            // finally, play the added game
            AdaptDGames[numOfTDGames].playGame(false);    
            // adding one to the number of array games
            numOfTDGames += 1;
            break;
        }
        default:
            break;
        }
    }

    void continueGame(HexVector * VectorGames, HexArray1D * ArrayGames, HexAdapterV * AdaptVGames, HexAdapterD * AdaptDGames)
    {
        // for validty check
        const int lowerBound = 1;
        const int upperBound = numOfVGames + numOfAGames + numOfTVGames + numOfTDGames;
        int game; 

        gamesStatus(VectorGames, ArrayGames, AdaptVGames, AdaptDGames);
        
        cout << endl << "Select a game to continue(ex: 2) " << endl;
        cout << "Enter: ";
        cin  >> game;

        
        // for validty check
        if( !isInputValid(game, lowerBound, upperBound) )
            return;


        // game indexes
        const int indexV  = game-1;
        const int indexA  = indexV-numOfVGames;
        const int indexTV = indexA-numOfAGames; 
        const int indexTD = indexTV-numOfTVGames;

        // comparison indexes
        const int compV  = numOfVGames;
        const int compA  = numOfVGames+numOfAGames;
        const int compTV = numOfVGames+numOfAGames+numOfTVGames;

        if(game <= compV)
            VectorGames[indexV].playGame(true);
        else if(game > compV && game <= compA)
            ArrayGames[indexA].playGame(true);
        else if(game > compA && game <= compTV)
            AdaptVGames[indexTV].playGame(true);
        else if(game > compTV)
            AdaptDGames[indexTD].playGame(true);
    }

    void compareGames(HexVector * VectorGames, HexArray1D * ArrayGames, HexAdapterV * AdaptVGames, HexAdapterD * AdaptDGames)
    {
        // for validty check
        const int lowerBound = 1;
        const int upperBound = numOfVGames + numOfAGames + numOfTVGames + numOfTDGames;
        int game1, game2; 

        gamesStatus(VectorGames, ArrayGames, AdaptVGames, AdaptDGames);
        
        cout << endl << "Select two games to compare(ex: 1 3) " << endl;
        cout << "Enter: ";
        cin  >> game1 >> game2;

        
        // validty check
        if( !isInputValid(game1, lowerBound, upperBound) )
            return;
        
        if( !isInputValid(game2, lowerBound, upperBound) )
            return;

        // swapping values if game1 is bigger (for easy compare)
        int temp;
        if(game1 > game2)
        {
            temp = game1;
            game1 = game2;
            game2 = temp;
        }

        // game 1 indexes
        const int g1IndexV  = game1-1;                 // [Vector]
        const int g1IndexA  = g1IndexV-numOfVGames;    // [Array]
        const int g1IndexTV = g1IndexA-numOfAGames;    // [Templated Vector]
        const int g1IndexTD = g1IndexTV-numOfTVGames;  // [Templated Deque]

        // game 2 indexes
        const int g2IndexV  = game2-1;
        const int g2IndexA  = g2IndexV-numOfVGames;
        const int g2IndexTV = g2IndexA-numOfAGames;
        const int g2IndexTD = g2IndexTV-numOfTVGames;

        // comparison indexes
        const int compV  = numOfVGames;
        const int compA  = numOfVGames+numOfAGames;
        const int compTV = numOfVGames+numOfAGames+numOfTVGames;

        // if game1 is a Vector game
        if(game1 <= compV)
        {
            // if game2 is a Vector game
            if(game2 <= compV)
            {
                if(VectorGames[g1IndexV] == VectorGames[g2IndexV])
                    cout << endl << "[Game" << game1 << " has same board with Game" << game2 << "]" <<  endl;
                else 
                    cout << endl << "[Game" << game1 << " has a different board from Game" << game2 << "]" << endl;
            }
            // if game2 is a Array game
            else if(game2 > compV && game2 <= compA)
            {
                if(VectorGames[g1IndexV] == ArrayGames[g2IndexA])
                    cout << endl << "[Game" << game1 << " has same board with Game" << game2 << "]" <<  endl;
                else 
                    cout << endl << "[Game" << game1 << " has a different board from Game" << game2 << "]" << endl;
            }
            // if game2 is a templated vector game
            else if(game2 > compA && game2 <= compTV)
            {
                if(VectorGames[g1IndexV] == AdaptVGames[g2IndexTV])
                    cout << endl << "[Game" << game1 << " has same board with Game" << game2 << "]" <<  endl;
                else 
                    cout << endl << "[Game" << game1 << " has a different board from Game" << game2 << "]" << endl;
            }
            // if game2 is a templated deque game
            else if(game2 > compTV)
            {
                if(VectorGames[g1IndexV] == AdaptDGames[g2IndexTD])
                    cout << endl << "[Game" << game1 << " has same board with Game" << game2 << "]" <<  endl;
                else 
                    cout << endl << "[Game" << game1 << " has a different board from Game" << game2 << "]" << endl;
            }
        }
        // if game1 is a Array game
        else if(game1 > compV && game1 <= compA) 
        {
            // if game2 is a Array game
            if(game2 > compV && game2 <= compA)
            {
                if(ArrayGames[g1IndexA] == ArrayGames[g2IndexA])
                    cout << endl << "[Game" << game1 << " has same board with Game" << game2 << "]" <<  endl;
                else 
                    cout << endl << "[Game" << game1 << " has a different board from Game" << game2 << "]" << endl;
            }
            // if game2 is a templated vector game
            else if(game2 > compA && game2 <= compTV)
            {
                if(ArrayGames[g1IndexA] == AdaptVGames[g2IndexTV])
                    cout << endl << "[Game" << game1 << " has same board with Game" << game2 << "]" <<  endl;
                else 
                    cout << endl << "[Game" << game1 << " has a different board from Game" << game2 << "]" << endl;
            }
            // if game2 is a templated deque game
            else if(game2 > compTV)
            {
                if(ArrayGames[g1IndexA] == AdaptDGames[g2IndexTD])
                    cout << endl << "[Game" << game1 << " has same board with Game" << game2 << "]" <<  endl;
                else 
                    cout << endl << "[Game" << game1 << " has a different board from Game" << game2 << "]" << endl;
            }
        }
        // if game1 is a templated vector game
        else if(game1 > compA && game1 <= compTV)
        {
            // if game2 is a templated vector game
            if(game2 > compA && game2 <= compTV)
            {
                if(AdaptVGames[g1IndexTV] == AdaptVGames[g2IndexTV])
                    cout << endl << "[Game" << game1 << " has same board with Game" << game2 << "]" <<  endl;
                else 
                    cout << endl << "[Game" << game1 << " has a different board from Game" << game2 << "]" << endl;
            }
            // if game2 is a templated deque game
            else if(game2 > compTV)
            {
                if(AdaptVGames[g1IndexTV] == AdaptDGames[g2IndexTD])
                    cout << endl << "[Game" << game1 << " has same board with Game" << game2 << "]" <<  endl;
                else 
                    cout << endl << "[Game" << game1 << " has a different board from Game" << game2 << "]" << endl;
            }
        }
        // if game1 is a templated deque game
        else if (game1 > compTV)
        {
            // if game2 is a templated deque game
            if(game2 > compTV)
            {
                if(AdaptDGames[g1IndexTV] == AdaptDGames[g2IndexTD])
                    cout << endl << "[Game" << game1 << " has same board with Game" << game2 << "]" <<  endl;
                else 
                    cout << endl << "[Game" << game1 << " has a different board from Game" << game2 << "]" << endl;
            }
        }
    }

    void printBoard(HexVector * VectorGames, HexArray1D * ArrayGames, HexAdapterV * AdaptVGames, HexAdapterD * AdaptDGames) 
    {
        // for validty check
        const int lowerBound = 1;
        const int upperBound = numOfAGames + numOfVGames + numOfTVGames + numOfTDGames;

        int game;

        gamesStatus(VectorGames, ArrayGames, AdaptVGames, AdaptDGames);
        cout << endl << "Select the game you want to print(ex: 2)" << endl;
        cout << "Enter: ";
        cin  >> game;
        
        if(!isInputValid(game, lowerBound, upperBound))
            return;

        // game indexes
        const int indexV  = game-1;
        const int indexA  = indexV-numOfVGames;
        const int indexTV = indexA-numOfAGames; 
        const int indexTD = indexTV-numOfTVGames;

        // comparison indexes
        const int compV  = numOfVGames;
        const int compA  = numOfVGames+numOfAGames;
        const int compTV = numOfVGames+numOfAGames+numOfTVGames;

        if(game <= compV)
            VectorGames[indexV].print();
        else if(game > compV && game <= compA)
            ArrayGames[indexA].print();
        else if(game > compA && game <= compTV)
            AdaptVGames[indexTV].print();
        else if(game > compTV)
            AdaptDGames[indexTD].print();
    }

    // [Returns true if selected game has a valid sequence of moves]
    bool isGameValid(AbstractHex * Games[], const int & numOfGames)
    {
        int game;

        for(int i=0; i<numOfGames; i++)
            cout << i+1 << ") Game" << i+1 << ' ' << endl;

        cout << endl << "Select a game to check(ex: 3) " << endl;
        cout << "Enter: ";
        cin  >> game;


        cout << "There are " << Games[game-1]->numOfX() << " x letter and " << Games[game-1]->numOfO() << " o letter." << endl;

        try
        {
            if(Games[game-1]->isValid() == true)
                return true;
            else
                return false;
        }
        catch(OutOfBound)
        {
            cerr << "Invalid input for bound" << endl;
        }

        return true;
    }


    // [Menu Helper Function]

    void gamesStatus(HexVector * VectorGames, HexArray1D * ArrayGames, HexAdapterV * AdaptVGames, HexAdapterD * AdaptDGames)
    {
        // [game numbers]

        int V  = 1;                  // [Vector]
        int A  = V  + numOfVGames;   // [Array]
        int TV = A  + numOfAGames;   // [Templated Vector]
        int TD = TV + numOfTVGames;  // [Templated Deque]

        // [stats]

        // [Title]
        cout << endl;
        cout << " \t     Board \t Opponent    Move Count" << endl
            << " \t     ----- \t --------    ----------" << endl;

        // [Vector Games]
        cout << "Vector: " << endl << endl;
        for(int i=0; i<numOfVGames; i++, V++)
        {
            cout << V << ") Game" << V << ' ';
            VectorGames[i].printStats();
        }

        // [Array Games]
        cout << endl << "Array: " << endl << endl;
        for(int i=0; i<numOfAGames; i++, A++)
        {
            cout << A << ") Game" << A << ' ';
            ArrayGames[i].printStats();
        }

        // [Templated Vector Games]
        cout << endl << "Templated Vector:" << endl << endl;
        for(int i=0; i<numOfTVGames; i++, TV++)
        {
            cout << TV << ") Game" << TV << ' ';
            AdaptVGames[i].printStats();
        }

        // [Templated Deque Games]
        cout << endl << "Templated Deque:" << endl << endl;
        for(int i=0; i<numOfTDGames; i++, TD++)
        {
            cout << TD << ") Game" << TD << ' ';
            AdaptDGames[i].printStats();
        }

    }

    void commandScreen()
    {
        cout << endl;
        cout << "\tCommands" << endl;
        cout << "\t--------" << endl;
        cout << "MENU           : goes to the menu screen  " << endl;
        cout << "SAVE ____.txt  : saves the current game. can be loaded anytime with LOAD command" << endl;
        cout << "LOAD ____.txt  : loads the game with given name." << endl;
        cout << "RESET          : Resets the current board to it's initial state  " << endl;
        cout << "R_SIZE x       : Resets the board like RESET, 'x' takes a number and board will change to that size (sizes:5-52)" << endl;
        cout << "LASTMOVE       : Shows 2 last move that has been played on the board. Throws exception if there are less than 2 moves" << endl;
    }

} // HexTugkan



