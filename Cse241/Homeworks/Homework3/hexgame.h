#ifndef HEXGAME_H
#define HEXGAME_H

using namespace std;

const int MAX_SIZE = 52;
const int MAX_POSSIBLE_MOVE = 1352; // a player can make maximum of 1352 valid moves. how to get? => ( (52*52) / 2 ) = 1352

enum cState { empty = '.', x = 'x', o = 'o', bigX = 'X', bigO = 'O'};
enum uTurn  { u1 = 1, u2 = 2};

class Hex
{
private:
    class Cell
    {
    public:
        Cell();

        // Getters
        char get_cellState() const { return cellState; }
        char get_column   () const { return column;    }
        int  get_row      () const { return row;    }
    private:
        // Setters
        void set_column (char & newCol) { column = newCol;}
        void set_row    (int  & newRow) { row = newRow;   }

        //Cell variables
        char cellState;
        char column;
        int  row;
    public:    
        void set_cellState(char state) { cellState = state; }
    };
    
    // vector of vector of cell objects 
    vector<vector<Cell>> hexCells;
    
    // used in printBoard()
    vector<char> columnNames; // holds column letters. ex: 'A', 'B', 'C'... using with printBoard() func.
    
    // used in isGameOver()
    vector<int> routeX;       // holds possible win route for 'x' letter.  
    vector<int> routeO;       // holds possible win route for 'o' letter

public:

    // Constructors
    Hex();
    Hex(int size, char pvx_c);
    Hex(string filename);

    // Gameplay and helper functions
    void  playGame        (string & name, bool isExistingGame);
    void  playGame        (string & name);
    void  printBoard      () const;
    void  showGameStatus  ();
    bool  compareGameWith (Hex & game);

    // Global hex class move count getter 
    inline static int  getCount () { return movecount; }

    // Individual hex class move count getter
    int classMoves();


private:
    // Global hex class move count setters 
    inline static void addCount(int count) { movecount += count; }
    inline static void remCount(int count) { movecount -= count; }

    // Game Mechanics
    void play      ();
    void play      (int & row, int & column);
    void userInput ();
    void nextTurn();
    
    // Game Initializers
    void initialize_game();
    void initialize_board();
    void loadGame(string & filename);
    void loadGame(string filename, bool isExistingGame);
    void saveGame(string & filename);

    // Setters
    void set_gameName (string & name) { gameName = name; }
    void set_boardSize(int  & size)   { boardSize = size; } // works once when the user decide size of board 
    void set_pvx      (char & newPVX) { pvx = newPVX; }

    // Win Checkers
    void  isGameOver      ();              // if game is over it will assign gameFlag as true. else it will be false.
    void  searchX         (int cellIndex); // searches all possible routes for 'x' letters by checking placed cells to find a winning one.
    void  searchO         (int cellIndex); // searches all possible routes for 'y' letters by checking placed cells to find a winning one.
    
    // Win Check Helper Functions
    bool  isPathCompleted (int cellIndex);    
    bool  isEmpty         (int & row_i, int & column_s);
    bool  isOnRoute       (int x, int y, int cellIndex);
    bool  checkNeighbors  (int cellIndex, int neighbor, char letter);
    void  convertToUpper  ();                                         // converts wining path to bigger letters
    void  deleteRoute     ();                                         // deletes row and column values in routeX and routeO.

    // Computer Moves
    void goRandom    ();  // places 'o' letter to a random empty space   
    
    // Input Validty Checkers
    bool isNumber      (char & num);
    bool isSizeValid   (string & size_s);
    bool isPVXValid    (string & pvx_s);
    bool isRowValid    (string & row_s);
    bool isColumnValid (string & column_s);

    // Individual game class variables
    string gameName;
    int  row, column; //holds play() functions row and column location
    int  turn;        //shows which player is playing
    int  boardSize;   //min: 5 && max: 52
    char state;       //shows which letters is going to placed to specified cell
    char pvx;         //hols who the user1 playing agaings(player vs player(pvp) or player vs computer(pvc)) 
    bool gameFlag ;   // gameFlag becomes true when a winning route has been found and games end.

    // Global game class variables
    static int movecount;
};

// Global Functions

    // adds a new game and a name for this game
    string newName          (int & gameNum);
    void   printGamesStatus (vector<Hex> & games); 

    // Input Validty Checkers
    bool isChoiceValid       (string & choice);

    //  Menu
    void menu();
    void newGame             (vector<Hex> & games, string & name); // Case 1 (new game)
    void continueGame        (vector<Hex> & games); // Case 2 (continue to an existing game)
    void compareGames        (vector<Hex> & games); // Case 3 (comperision between two games)
    void printTotalMoveCount (vector<Hex> & games); // Case 4 (current move count of all games)
    void printSelectedBoard  (vector<Hex> & games); // Case 5 (Only print the board of a game)

#endif

