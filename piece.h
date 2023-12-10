#ifndef __PIECE_H__
#define __PIECE_H__
using namespace std;

enum PlayerType { Human, Computer1, Computer2, Computer3, Computer4, ND };
enum PlayerColour { Black, White, NA };
enum PieceType { King, Queen, Rook, Bishop, Knight, Pawn, Blank };

class Piece {

    PlayerColour c;
    PieceType p;
    bool HasMoved;
    bool EnPassant;

public:
    Piece();
    Piece(PlayerColour c, PieceType p);
    ~Piece() {}

    void setColour(PlayerColour col);
    void setPieceType(PieceType p);
    void setHasMoved(bool b);
    void setEnPassant(bool b);
    PlayerColour getColour();
    PieceType getPieceType();
    bool getHasMoved();
    bool getEnPassant();
};

#endif
