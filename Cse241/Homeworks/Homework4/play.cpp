#include <iostream>
#include "hexgame.h"

int Hex::numofGames = 0;
int Hex::totalMove = 0;

int main()
{
    Hex * games;
    games = new Hex[5];

    // conversion constructor
    games[0] = (string)"game1.txt";
    games[1] = (string)"game2.txt";
    games[2] = (string)"game3.txt";
    games[3] = (string)"game4.txt";
    games[4] = (string)"game5.txt";

    // adding these 5 games to the game count
    for(int i=0; i<5; i++)
        Hex::plusOneGame();

    // menu loop
    int choice = 0;
    while(choice != 7)
    {
        cout << "\n              MENU            "   << endl
             << "--------------------------------"   << endl
             << "1) New Game"                        << endl
             << "2) Continue to an existing game"    << endl
             << "3) Comperision between two games"   << endl
             << "4) Current move count of all games" << endl
             << "5) Only print the board of a game " << endl
             << "6) In game commands               " << endl
             << "7) Exit                           " << endl << endl
             << "Enter: ";

        cin  >> choice;

        // clearing cin to prevent infinite loop in case of cin fails    
        if(cin.fail())
        {
            cin.clear();
            cin.ignore(128, '\n'); // ignoring until '\n' character
        }

        switch (choice)
        {
            case 1:
                Hex * game;
                game  = Hex::newGame();            // returning played game to the Hex object pointer 
                games = Hex::addGame(games, game); // so i can add it to the Hex object array
                break;
            case 2:
                Hex::continueGame(games);
                break;
            case 3:
                Hex::compareGames(games);
                break;
            case 4:
                Hex::moveCounts(games);
                break;
            case 5:
                Hex::printBoard(games);
                break;
            case 6:
                Hex::commandScreen();
                break;
            case 7:
                cout << "\nGood Bye!\n" << endl;
                break;
            default:
                cerr << endl << "Error: Invalid input!" << endl;
        }
        if(choice == 7)
            break;
    }

    delete []games;

    return 0;
}