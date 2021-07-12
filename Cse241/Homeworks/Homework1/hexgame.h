#ifndef HEXGAME_H
#define HEXGAME_H

const int MAX_SIZE = 13;
const int MAX_POSSIBLE_MOVE = 144; // Maximum possible dots in the field is 144.

//game utilities
void initialize_game (int * sizeOfBoard, char * player_or_computer);
void initialize_board(char board[MAX_SIZE][MAX_SIZE], int sizeOfBoard);
void printBoard      (char board[MAX_SIZE][MAX_SIZE], int sizeOfBoard);

//user1 or user2 moves
void askUser         (char board[MAX_SIZE][MAX_SIZE], int * row, char * column, bool user, int sizeOfBoard);
void insertTo        (int row, char column, char board[MAX_SIZE][MAX_SIZE], bool user, int sizeOfBoard);
void deleteRoute     (int * routeX, int * routeY);
void searchRouteU1   (char board[MAX_SIZE][MAX_SIZE], int * routeX, int * routeY, int currIndex, bool * flag, int sizeOfBoard);
void searchRouteU2   (char board[MAX_SIZE][MAX_SIZE], int * routeX, int * routeY, int currIndex, bool * flag, int sizeOfBoard, bool user);

//computer moves
void askComputer     (char board[MAX_SIZE][MAX_SIZE], int * row, char * column, int sizeOfBoard);
void findLastMove    (char board[MAX_SIZE][MAX_SIZE], char letter, int * routeX, int * routeY, int & currX, int & currY);
bool isNeighborsEmpty(char board[MAX_SIZE][MAX_SIZE], int x, int y, int neighbors[6]);
void goStraightO     (char board[MAX_SIZE][MAX_SIZE], int * row, char * column);
void placeToNeighbor (int x, int y, int * row, char * column, int neighbor);

//condition check or basic task functions
void convertToUpper  (char board[MAX_SIZE][MAX_SIZE], int * routeX, int * routeY, bool user);
int  countMoves      (char board[MAX_SIZE][MAX_SIZE], char letter);
int  countLetter     (char board[MAX_SIZE][MAX_SIZE], char letter);
bool checkNeighbors  (char board[MAX_SIZE][MAX_SIZE], int * routeX, int * routeY, int currIndex, int neighbor, char letter);
bool isGameOver      (char board[MAX_SIZE][MAX_SIZE], int sizeOfBoard, bool user);
bool isEmpty         (char board[MAX_SIZE][MAX_SIZE], int x, int y);
bool isOnRoute       (int x, int y, int * routeX, int * routeY, int currIndex);
bool isPathCompleted (int * routeX, int * routeY, int user, int lastMove, int lastXorY);
bool isInputValid    (char x, int y, int sizeOfBoard);

#endif
