#include <iostream>
#include <string>
#include <vector>
#include "hexgame.h"

using namespace std;

int Hex::movecount = 0;

Hex game;
vector<Hex> games;
static int gameNum = 5;

int main()
{
    games.push_back(Hex("game1.txt"));
    games.push_back(Hex("game2.txt"));
    games.push_back(Hex("game3.txt"));
    games.push_back(Hex("game4.txt"));
    games.push_back(Hex("game5.txt"));

    cout << endl << "******* Welcome to HEX GAME *******\n" << endl;

    menu();

    return 0;
}

void menu()
{
    string choice;
    string gameName;

    do
    {
        cout << "\t     MENU" << endl
             << "--------------------------------" << endl
             << "1) New Game" << endl
             << "2) Continue to an existing game" << endl
             << "3) Comperision between two games" << endl
             << "4) Current move count of all games" << endl
             << "5) Only print the board of a game" << endl
             << "6) Exit" << endl;

        do
        {
            cout << "\nEnter: ";
            cin >> choice;
        } 
        while (isChoiceValid(choice) == false);

        switch (choice[0])
        {
            case '1':
                gameName = newName(gameNum);
                newGame(games, gameName);
                break;
            case '2':
                continueGame(games);
                break;
            case '3':
                compareGames(games);
                break;
            case '4':
                printTotalMoveCount(games);
                break;
            case '5':
                printSelectedBoard(games);
                break;
            case '6':
                cout << "\nGood Bye!\n" << endl;
        }
    } 
    while (choice[0] != '6');
}
