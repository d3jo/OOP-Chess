#include "piece.h"
using namespace std;

Piece::Piece(): c{PlayerColour::NA}, p{PieceType::Blank}, HasMoved{false}, EnPassant{false} {}

Piece::Piece(PlayerColour c, PieceType p): c{c}, p{p}, HasMoved{false}, EnPassant{false} {}

void Piece::setColour(PlayerColour col) {
    c = col;
}

void Piece::setPieceType(PieceType piece) {
    p = piece;
}

void Piece::setHasMoved(bool b) {
    HasMoved = b;
}

void Piece::setEnPassant(bool b) {
    EnPassant = b;
}

PlayerColour Piece::getColour() {
    return c;
}
 PieceType Piece::getPieceType() {
    return p;
 }

bool Piece::getHasMoved() {
    return HasMoved;
}
bool Piece::getEnPassant() {
    return EnPassant;
}

