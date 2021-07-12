#ifndef HEXEXCEPTIONS_H
#define HEXEXCEPTIONS_H

namespace HexTugkan
{
    // [for main.cpp]
    class InvalidMenuSelection
    { };    

    // [for IsInputValid function]
    class InputIsInvalid 
    { };

    // [for IsInputValid function]
    class BoundIsInvalid
    { };

    // [for LastMove function]
    class NotEnoughMove
    { };

    // [for operator()]
    class OutOfBound
    { };

    // [for setSize function]
    class InvalidSize
    { };

} // HexTugkan

#endif