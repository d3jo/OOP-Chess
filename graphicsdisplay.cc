#include "graphicsdisplay.h"
#include "cell.h"
#include <string>

string returnStringPiece(int r, int c) {
    if (r == 1) return "pawn"; // Row 1 only has black pawns
    if (r == 6) return "PAWN"; // Row 6 only has white pawns

    if (r == 0) { // Row 0 has multiple black pieces
        switch (c) {
            case 0: return "rook"; 
            case 1: return "knight";
            case 2: return "bishop";
            case 3: return "queen";
            case 4: return "king";
            case 5: return "bishop";
            case 6: return "knight";
            case 7: return "rook";
        }
    }

    if (r == 7) { // Row 7 has multiple white pieces
        switch (c) {
            case 0: return "ROOK";
            case 1: return "KNIGHT";
            case 2: return "BISHOP";
            case 3: return "QUEEN";
            case 4: return "KING";
            case 5: return "BISHOP";
            case 6: return "KNIGHT";
            case 7: return "ROOK";
        }
    }
    return "";
}

string convertToString(Piece p) {
    if (p.getColour() == PlayerColour::White) {
        switch (p.getPieceType()) {
            case PieceType::King: return "KING";
            case PieceType::Queen: return "QUEEN";
            case PieceType::Rook: return "ROOK";
            case PieceType::Bishop: return "BISHOP";
            case PieceType::Knight: return "KNIGHT";
            case PieceType::Pawn: return "PAWN";
            case PieceType::Blank: return "";
        }
    } else if (p.getColour() == PlayerColour::Black) {
        switch (p.getPieceType()) {
            case PieceType::King: return "king";
            case PieceType::Queen: return "queen";
            case PieceType::Rook: return "rook";
            case PieceType::Bishop: return "bishop";
            case PieceType::Knight: return "knight";
            case PieceType::Pawn: return "pawn";
            case PieceType::Blank: return "";
        }
    }
    return "";
}

GraphicsDisplay::GraphicsDisplay(Xwindow &window): window{window} {
    int gap = 800 / 8;
    for(int i = 0; i < 8; ++i) {
        for(int j = 0; j < 8; ++j) {
            if ((i%2)!=(j%2)) window.fillRectangle(j*gap, i*gap, gap, gap, Xwindow::Green);
            else window.fillRectangle(j*gap, i*gap, gap, gap, Xwindow::White);
            if ((i == 0) || (i == 1) || (i == 6) || (i == 7)) window.drawString(j*gap + 40, (i + 1)*gap - 45, returnStringPiece(i, j));
        }
    }
}


void GraphicsDisplay::notify(Cell &c) {
    int gap = 900 / 9;
    int row = c.getRow();
    int col = c.getCol();
    Piece p = c.getPiece();
    if ((row%2) != (col%2)) window.fillRectangle(col*gap, row*gap, gap, gap, Xwindow::Green);
    else window.fillRectangle(col*gap, row*gap, gap, gap, Xwindow::White);
    window.drawString(col*gap + 40, (row + 1)*gap - 45, convertToString(p));
}
