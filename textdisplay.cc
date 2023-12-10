#include "textdisplay.h"
#include "cell.h"
using namespace std;

char returnCharPiece(int r, int c) {
    if (r == 1) return 'p'; // Row 1 only has black pawns
    if (r == 6) return 'P'; // Row 6 only has white pawns

    if (r == 0) { // Row 0 has multiple black pieces
        switch (c) {
            case 0: return 'r'; 
            case 1: return 'n';
            case 2: return 'b';
            case 3: return 'q';
            case 4: return 'k';
            case 5: return 'b';
            case 6: return 'n';
            case 7: return 'r';
        }
    }

    if (r == 7) { // Row 7 has multiple white pieces
        switch (c) {
            case 0: return 'R';
            case 1: return 'N';
            case 2: return 'B';
            case 3: return 'Q';
            case 4: return 'K';
            case 5: return 'B';
            case 6: return 'N';
            case 7: return 'R';
        }
    }
    return 'X';
}

TextDisplay::TextDisplay(): theDisplay{} {
    for(int i = 0; i < 8; ++i) {
        vector<char> temp;
        for(int j = 0; j < 8; ++j) {
            if ((i == 0) || (i == 1) || (i == 6) || (i == 7)) {
                temp.emplace_back(returnCharPiece(i, j));
                continue;
            }
            if(i % 2) {
                if (j % 2) {
                    temp.emplace_back(' ');
                } else {
                    temp.emplace_back('_');
                }
            } else if (j % 2) {
                temp.emplace_back('_');
            } else {
                temp.emplace_back(' ');
            }
        }
        theDisplay.emplace_back(temp);
    }
}

char convertToChar(PieceType pt) {
    switch (pt) {
        case PieceType::King: return 'K';
        case PieceType::Queen: return 'Q';
        case PieceType::Rook: return 'R';
        case PieceType::Bishop: return 'B';
        case PieceType::Knight: return 'N';
        case PieceType::Pawn: return 'P';
        case PieceType::Blank: return 'X';
    }
    return 'X';
}

void TextDisplay::notify(Cell &c) {
    int row = c.getRow();
    int col = c.getCol();
    Piece p = c.getPiece();
    PieceType t = p.getPieceType();
    PlayerColour colour = p.getColour();
    char cWord = convertToChar(t);
    if (cWord == 'X') {
        if ((row%2) != (col%2)) theDisplay[row][col] = '_';
        else theDisplay[row][col] = ' ';
    } else if (colour == PlayerColour::White) {
        theDisplay[row][col] = cWord;
    } else {
        theDisplay[row][col] = cWord + 32;
    }
}

ostream &operator<<(ostream &o, const TextDisplay &t) {
    for(int i = 0; i < 8; ++i) {
        o << (8 - i) << " ";
        for(int j = 0; j < 8; ++j) {
            o << t.theDisplay[i][j];
        }
        o << endl;
    }
    o << "  abcdefgh" << endl;
    return o;
}
