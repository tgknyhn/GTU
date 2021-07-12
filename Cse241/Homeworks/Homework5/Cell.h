#ifndef CELL_H
#define CELL_H

namespace HexTugkan
{
    class Cell
    {
    public:
        // constructors
        Cell();
        Cell(char state);

        void setState (const char& state) { cellState = state; }
        char getState () const            { return cellState; }
    private:
        char cellState; // there can be 3 state (which are empty, x, o)
    };
    
} // HexTugkan


#endif