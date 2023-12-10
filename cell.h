#ifndef __CELL_H__
#define __CELL_H__
#include "observer.h"
#include "piece.h"

class Cell {
    int x, y; // Coordinate
    Piece p; // Piece
    Observer *td;
    Observer *gd;

    public:
    Cell();
    Cell(int x, int y, Piece p, Observer *td, Observer *gd); // Default Constructor
    ~Cell() = default; // Destructor

    // Modifying field
    void setCoords(int x, int y); // Set Coordinate
    void setPiece(Piece p); // Set Piece
    // void setGD(Observer *g); // Set GraphicsDisplay
    void setTD(Observer *t); // Set TextDisplay
    void setGD(Observer *g);
    
    // Returning field
    int getRow(); // Return row
    int getCol(); // Return column
    Piece getPiece(); // Return piece

    void changePiece(Piece p);
    
    // Notify
    void notifyObservers(); // Notify gd and td
};

#endif
