#ifndef __BOARD_H__
#define __BOARD_H__

#include <vector>
#include "cell.h"
#include "graphicsdisplay.h"
#include "textdisplay.h"
#include "player.h"


class Board {
    std::vector<std::vector<Cell>> theBoard; // The board
    TextDisplay *td; // Text Display
    GraphicsDisplay *gd; // Graphics Display
    Player* a;
    Player* b;
    Player* curr;
    int wKingR, wKingC, bKingR, bKingC;
    int turn; // Current turn

    public:
    Board(Xwindow &window); // Default Constructor
    ~Board(); // Destructor

    void clearBoard(Xwindow &window);
    void init(Player *otherA, Player *otherB, Player *current); // Initialize the board with default board
    void setPiece(int r, int c, Piece p); // Set piece p at (x, y) of board. Assume there only will be one king per player on board.
    Piece getPiece(int r, int c);
    void setCurr(Player *current);
    void addTurn();
    Player *getCurr();
    void setAllEnPassant(int r = -1, int c = -1);
    void resetKing();
    void emptyBoard();
    bool onThePath(int r1, int c1, int r2, int c2);
    std::vector<bool> simulate(int r1, int c1, int r2, int c2, int r3 = -1, int c3 = -1, int r4 = -1, int c4 = -1);
    int simulateOpponent(int r1, int c1, int r2, int c2, PlayerColour c);
    bool makeMove(int r1, int c1, int r2, int c2, Piece p); // Make move from (x1, y1) to (x2, y2). Before making actual move, it checks:
                                                            // - Is (x1, y1) and (x2, y2) a valid coordinate in board?
                                                            // - Does (x1, y1) have a piece that is a current player's piece?
                                                            // - Does (x2, y2) have current player's piece?
                                                            // - Is move (x1, y1) to (x2, y2) a valid movement for piece in (x1, y1)?
                                                            // - Is there a threat to current player's king after the move?
                                                            // If the move pass all the conditions, it is a valid move.
                                                            // Special cases: Castling, En Passant, Pawn Promotion, Pawn's first move
                                                            // Castling: Check if King & Rook never moved. If they didn't, castling is allowed
                                                            // En Passant: Check the 'EnPassant' parameter of Piece. If true, En Passant is allowed
                                                            // Pawn Promotion: Check if the moved coordinate is at the other end. Check if the piece is pawn. If it is, pawn promotion.
                                                            // Pawn's first move: Check if Pawn never moved. If they didn't, can move two blocks.
    std::vector<std::vector<int>> returnPossibleMove(int r, int c);
    std::vector<std::vector<int>> returnPossibleMoveEmpty(int r, int c);
    bool isCheckMate(); // Is the board checkmate for current player?
    bool isStaleMate(); // Is the board stalemate for current player?
    bool isCheck(PlayerColour currCol); // Is the King of current player threatened?
    bool isCapturable(int targetRow, int targetCol);
    bool oneKing();
    std::vector<int> returnComputerMove(PlayerColour c, int level);
    friend std::ostream &operator<<(std::ostream &o, const Board &b); // Output board
};


#endif
