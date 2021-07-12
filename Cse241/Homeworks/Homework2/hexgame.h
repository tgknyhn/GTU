#ifndef HEXGAME_H
#define HEXGAME_H

const int MAX_SIZE = 13;
const int MAX_POSSIBLE_MOVE = 144; // Maximum possible dots in the field is 144.

//game utilities
void initialize_game (int & size, char & player_or_computer);
void initialize_board(char board[MAX_SIZE][MAX_SIZE], int & sizeOfBoard);
void printBoard      (char board[MAX_SIZE][MAX_SIZE], int & sizeOfBoard);

//user1 or user2 moves
bool askUser         (char board[MAX_SIZE][MAX_SIZE], int & row, char & column, bool & user, int & sizeOfBoard, bool & pveORpvp);
void insertTo        (int row, char column, char board[MAX_SIZE][MAX_SIZE], bool user, int & sizeOfBoard);
void deleteRoute     (int * routeX, int * routeY);
void searchRouteU1   (char board[MAX_SIZE][MAX_SIZE], int * routeX, int * routeY, int currIndex, bool * flag, int & sizeOfBoard);
void searchRouteU2   (char board[MAX_SIZE][MAX_SIZE], int * routeX, int * routeY, int currIndex, bool * flag, int & sizeOfBoard, bool user);
void saveGame        (char board[MAX_SIZE][MAX_SIZE], const std::string move, const std::string name, bool & user, int & sizeOfBoard, bool & pveORpvp);
void loadGame	     (char board[MAX_SIZE][MAX_SIZE], const std::string move, const std::string name, bool & user, int & sizeOfBoard, bool & pveORpvp);

//computer moves
void askComputer     (char board[MAX_SIZE][MAX_SIZE], int & row, char & column, int & sizeOfBoard);
void findLastMove    (char board[MAX_SIZE][MAX_SIZE], char letter, int * routeX, int * routeY, int & currX, int & currY, int & sizeOfBoard);
bool isNeighborsEmpty(char board[MAX_SIZE][MAX_SIZE], int x, int y, int neighbors[6]);
void goStraightO     (char board[MAX_SIZE][MAX_SIZE], int & row, char & column, int & sizeOfBoard);
void placeToNeighbor (const int & x, const int & y, int & row, char & column, int neighbor);

//condition check or basic task functions
void changeUser      (bool & user); 
void convertToUpper  (char board[MAX_SIZE][MAX_SIZE], int * routeX, int * routeY, bool user = 0);
int  countMoves      (char board[MAX_SIZE][MAX_SIZE], char letter = 'x'); 
bool checkNeighbors  (char board[MAX_SIZE][MAX_SIZE], int * routeX, int * routeY, int currIndex, int neighbor, char letter = 'x');
bool isGameOver      (char board[MAX_SIZE][MAX_SIZE], int & sizeOfBoard, bool user);
bool isEmpty         (char board[MAX_SIZE][MAX_SIZE], int x, int y);
bool isOnRoute       (int x, int y, int * routeX, int * routeY, int currIndex);
bool isPathCompleted (int * routeX, int * routeY, int user, int lastMove, int lastXorY);
bool isInputValid    (char x, int y, int & sizeOfBoard);

#endif
