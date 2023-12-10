#include "cell.h"
using namespace std;

Cell::Cell(): x{0}, y{0}, p{Piece{PlayerColour::NA, PieceType::Blank}}, td{nullptr} {}

Cell::Cell(int x, int y, Piece p, Observer *td, Observer *gd): x{x}, y{y}, p{p}, td{td}, gd{gd} {}

void Cell::setCoords(int x, int y) {
    this->x = x;
    this->y = y;
}

void Cell::setPiece(Piece p) {
    this->p = p;
}

void Cell::setTD(Observer *t) {
    td = t;
}

void Cell::setGD(Observer *g) {
    gd = g;
}

int Cell::getRow() {
    return x;
}

int Cell::getCol() {
    return y;
}

Piece Cell::getPiece() {
    return p;
}

void Cell::changePiece(Piece p) {
    this->p = p;
    notifyObservers();
}

void Cell::notifyObservers() {
    td->notify(*this);
    gd->notify(*this);
}
