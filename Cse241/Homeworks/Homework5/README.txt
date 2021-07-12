Author: Ahmet Tuğkan Ayhan

Welcome to Homework 5
---------------------

To compile the game write "make".

If it says: "file is up to date", then first enter "make clean" 
after that enter "make" to execute the game.

IMPORTANT!
----------
To check if the global function that takes AbstractHex pointer works fine I let the game include "invalid1.txt" and "invalid2.txt". 
Normally you can't make any invalid moves, game doesn't allow you to do that but if you change a .txt file and load it then you can create an invalid game.
If you try to load these kind of games, game doesn't allow that too (code only checks if move count of X and O are equal(or +1 -1 of that) 
and if there is any unwanted letters except x,X,o,O and dot). Game only allows loading invalid games that named "invalid1.txt" and "invalid2.txt". and this is only for making sure that global function works correctly. and it does :)

also I added invalid3.txt to the directory which is also an invalid game. You can try to load this in game.


1) New Game
-----------

- When a new game starts choose class represantation first

- Then, choose board size and an opponent.

- To make a move first enter a column then a row (ex: A 11) !space is important

- Your game will be saved if you go to menu with or without finishing it. 

- Your games is listed by their class represantation even if you leave your game with or without finishing it   


2) Continue to an existing game
-------------------------------

- All games will be listed here. 

- There are initially added 8 games when you execute the game(game1, game3, game5 are the same game. to see how comperision works). 

- All information you'll need before game selection, it will be shown to you (boardsize, opponent, moves played, class represantation)

- To continue a game write the number of the game you want to continue

3) Comperision
--------------

- All games will be listed here as well

- To compare enter two numbers which represents their game number. 

- If two selected boards are exactly same (x and o locations and numbers included),
then 2 boards are equal. 

- Example input: "3 5" (without commas)

5) Print Board
-------------

- You can see a game's board without playing it. 

- Ideal to see finished games board 

6) In game commands
-------------------

MENU           : goes to the menu screen 
SAVE ____.txt  : saves the current game. can be loaded anytime with LOAD command
LOAD ____.txt  : loads the game with given name. make sure that your giving a valid .txt file. 
RESET          : Resets the current board to it's initial state 
R_SIZE x       : Resets the board like RESET, 'x' takes a number and board will change to that size (sizes:5-52)
LASTMOVE       : Shows 2 last move that has been played on the board. Throws exception if there are less than 2 moves

** If you load any game without saving current one, loaded game's progress will be written onto current one. So all progress will be lost.
** You can't continue to any finished game. But you can still see the board in 5th section of the menu




