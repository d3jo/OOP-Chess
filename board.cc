#include "board.h"
#include <cstdlib>
using namespace std;

int returnScore(PieceType p) {
    switch (p) {
        case PieceType::King: return 500;
        case PieceType::Queen: return 9;
        case PieceType::Rook: return 5;
        case PieceType::Bishop: return 3;
        case PieceType::Knight: return 3;
        case PieceType::Pawn: return 1;
        case PieceType::Blank: return 0;
    }
    return 0;
}


Piece returnPiece(int r, int c) {
    if (r == 1) return Piece{PlayerColour::Black, PieceType::Pawn}; // Row 1 only has black pawns
    if (r == 6) return Piece{PlayerColour::White, PieceType::Pawn}; // Row 6 only has white pawns

    if (r == 0) { // Row 0 has multiple black pieces
        switch (c) {
            case 0: return Piece{PlayerColour::Black, PieceType::Rook}; 
            case 1: return Piece{PlayerColour::Black, PieceType::Knight};
            case 2: return Piece{PlayerColour::Black, PieceType::Bishop};
            case 3: return Piece{PlayerColour::Black, PieceType::Queen};
            case 4: return Piece{PlayerColour::Black, PieceType::King};
            case 5: return Piece{PlayerColour::Black, PieceType::Bishop};
            case 6: return Piece{PlayerColour::Black, PieceType::Knight};
            case 7: return Piece{PlayerColour::Black, PieceType::Rook};
        }
    }

    if (r == 7) { // Row 7 has multiple white pieces
        switch (c) {
            case 0: return Piece{PlayerColour::White, PieceType::Rook};
            case 1: return Piece{PlayerColour::White, PieceType::Knight};
            case 2: return Piece{PlayerColour::White, PieceType::Bishop};
            case 3: return Piece{PlayerColour::White, PieceType::Queen};
            case 4: return Piece{PlayerColour::White, PieceType::King};
            case 5: return Piece{PlayerColour::White, PieceType::Bishop};
            case 6: return Piece{PlayerColour::White, PieceType::Knight};
            case 7: return Piece{PlayerColour::White, PieceType::Rook};
        }
    }
    return Piece{PlayerColour::NA, PieceType::Blank};
}


bool Board::onThePath(int r1, int c1, int r2, int c2) {
    if (r1 == r2) {
        if (c1 < c2) {
            for (int i = 1; i < 8; ++i) {
                if ((c1 + i) > 7) return false;
                if ((c1 + i) == c2) return false;
                if (theBoard[r1][c1 + i].getPiece().getPieceType() != PieceType::Blank) return true;
            }
        } else {
            for (int i = 1; i < 8; ++i) {
                if ((c1 - i) < 0) return false;
                if ((c1 - i) == c2) return false;
                if (theBoard[r1][c1 - i].getPiece().getPieceType() != PieceType::Blank) return true;
            }
        }
    } else if (c1 == c2) {
        if (r1 < r2) {
            for (int i = 1; i < 8; ++i) {
                if ((r1 + i) > 7) return false;
                if ((r1 + i) == r2) return false;
                if (theBoard[r1 + i][c1].getPiece().getPieceType() != PieceType::Blank) return true;
            }
        } else {
            for (int i = 1; i < 8; ++i) {
                if ((r1 - i) < 0) return false;
                if ((r1 - i) == r2) return false;
                if (theBoard[r1 - i][c1].getPiece().getPieceType() != PieceType::Blank) return true;
            }
        }
    } else if (r1 < r2) {
        if (c1 < c2) {
            for (int i = 1; i < 8; ++i) {
                if (((r1 + i) > 7) || ((c1 + i) > 7)) return false;
                if (((r1 + i) == r2) && ((c1 + i) == c2)) return false;
                if (theBoard[r1 + i][c1 + i].getPiece().getPieceType() != PieceType::Blank) return true;
            }
        } else {
            for (int i = 1; i < 8; ++i) {
                if (((r1 + i) > 7) || ((c1 - i) < 0)) return false;
                if (((r1 + i) == r2) && ((c1 - i) == c2)) return false;
                if (theBoard[r1 + i][c1 - i].getPiece().getPieceType() != PieceType::Blank) return true;
            }
        }
    } else {
        if (c1 < c2) {
            for (int i = 1; i < 8; ++i) {
                if (((r1 - i) < 0) || ((c1 + i) > 7)) return false;
                if (((r1 - i) == r2) && ((c1 + i) == c2)) return false;
                if (theBoard[r1 - i][c1 + i].getPiece().getPieceType() != PieceType::Blank) return true;
            }
        } else {
            for (int i = 1; i < 8; ++i) {
                if (((r1 - i) < 0) || ((c1 - i) < 0)) return false;
                if (((r1 - i) == r2) && ((c1 - i) == c2)) return false;
                if (theBoard[r1 - i][c1 - i].getPiece().getPieceType() != PieceType::Blank) return true;
            }
        }
    }
    return true;
}


Board::Board(Xwindow &window): theBoard{}, td{new TextDisplay{}}, gd{new GraphicsDisplay{window}}, a{nullptr}, 
                b{nullptr}, curr{nullptr}, wKingR{7}, wKingC{4}, 
                bKingR{0}, bKingC{4}, turn{1} {
    for (int i = 0; i < 8; ++i) { // Generate 64 cells
        vector<Cell> temp;
        for (int j = 0; j < 8; ++j) {
            Cell tempC = Cell();
            Observer *tempGD = gd;
            Observer *tempTD = td;
            tempC.setCoords(i, j);
            tempC.setGD(tempGD);
            tempC.setTD(tempTD);
            tempC.setPiece(returnPiece(i, j));
            temp.emplace_back(tempC);
        }
        theBoard.emplace_back(temp);
    }
}


Board::~Board() {
    // delete gd;
    delete td;
}


void Board::clearBoard(Xwindow &window) {
    delete gd; // Delete existing GraphicsDisplay
    delete td; // Delete existing TextDisplay
    gd = new GraphicsDisplay{window}; // Create new GraphicsDisplay
    td = new TextDisplay{}; // Create new TextDisplay
    a = nullptr; // Remove player
    b = nullptr; // Remove player
    curr = nullptr; // Remove player
    turn = 1; // Turn = 0 (Default value)
    wKingR = 7; // White King's row = 7 (Default value)
    wKingC = 4; // White King's column = 4 (Default value)
    bKingR = 0; // Black King's row = 0 (Default value)
    bKingC = 4; // Black King's column = 4 (Default value)
    vector<vector<Cell>> tempBoard; // Create new board
    for (int i = 0; i < 8; ++i) { // Recreate initial board
        vector<Cell> temp;
        for (int j = 0; j < 8; ++j) {
            Cell tempC = Cell();
            Observer *tempGD = gd;
            Observer *tempTD = td;
            tempC.setCoords(i, j);
            tempC.setGD(tempGD);
            tempC.setTD(tempTD);
            tempC.setPiece(returnPiece(i, j));
            temp.emplace_back(tempC);
        }
        tempBoard.emplace_back(temp);
    }
    theBoard = tempBoard; // Replace theBoard with new Board
}

void Board::init(Player *otherA, Player *otherB, Player *current) {
    a = otherA; // Allocate player A
    b = otherB; // Allocate player B
    curr = current; // Starting player
}

void Board::setPiece(int r, int c, Piece p) {
    theBoard[r][c].changePiece(p); // Change piece at row = r, column = c to Piece p. changepiece(p) notifies observers
    if (p.getPieceType() == PieceType::King) {
        if (p.getColour() == PlayerColour::Black) {
            bKingR = r;
            bKingC = c;
        } else {
            wKingR = r;
            wKingC = c;
        }
    }
}

Piece Board::getPiece(int r, int c) {
    return theBoard[r][c].getPiece();
}

void Board::setCurr(Player *current) {curr = current;}

Player* Board::getCurr() {return curr;}

void Board::addTurn() {++turn;}

void Board::setAllEnPassant(int row, int col) {
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            if ((r == row) && (c == col)) {
                Piece tempP = theBoard[r][c].getPiece();
                tempP.setEnPassant(true);
                theBoard[r][c].setPiece(tempP);
            } else {
                Piece tempP = theBoard[r][c].getPiece();
                tempP.setEnPassant(false);
                theBoard[r][c].setPiece(tempP);
            }
        }
    }
} 

void Board::resetKing() {
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            if (theBoard[r][c].getPiece().getPieceType() == PieceType::King) {
                if (theBoard[r][c].getPiece().getColour() == PlayerColour::Black) {
                    bKingR = r;
                    bKingC = c;
                } else {
                    wKingR = r;
                    wKingC = c;
                }
            }
        }
    }
}

void Board::emptyBoard() {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            theBoard[i][j].changePiece(Piece{});
        }
    }
    wKingC = -1;
    wKingR = -1;
    bKingC = -1;
    bKingR = -1;
}

vector<bool> Board::simulate(int r1, int c1, int r2, int c2, int r3, int c3, int r4, int c4) {
    vector<bool> returnVec; // Vector to return
    Piece pTemp = Piece{PlayerColour::NA, PieceType::Blank}; // Temporary piece to replace
    Piece pInit1 = theBoard[r1][c1].getPiece(); // Original piece at (r1, c1)
    Piece pFinal1 = theBoard[r2][c2].getPiece(); // Original piece at (r2, c2)
    Piece pInit2 = (r3 == -1)?pTemp:theBoard[r3][c3].getPiece(); // Original piece at (r3, c3), only when coordinate is given
    Piece pFinal2 = (r3 == -1)?pTemp:theBoard[r4][c4].getPiece(); // Original piece at (r4, c4), only when coordinate is given
    Piece pEnPass = pTemp;
    PlayerColour currCol = pInit1.getColour(); // Colour of the piece moving
    PlayerColour otherCol = (currCol == PlayerColour::Black)?PlayerColour::White:PlayerColour::Black; // Opposite colour

    // Change piece at (r1, c1) and (r2, c2)
    theBoard[r1][c1].setPiece(pTemp); 
    theBoard[r2][c2].setPiece(pInit1);

    //update king coordinate if necessary
    if (theBoard[r2][c2].getPiece().getPieceType() == PieceType::King) {
        if (currCol == PlayerColour::Black) {
            bKingC = c2;
            bKingR = r2;
        } else {
            wKingC = c2;
            wKingR = r2;
        }
    }
    // Change piece at (r3, c3) and (r4, c4), only when coordinate is given
    if (r3 != -1) {
        theBoard[r3][r3].setPiece(pTemp);
        theBoard[r4][c4].setPiece(pInit2);
    }

    if ((pInit1.getPieceType() == PieceType::Pawn) && (c1 != c2) && (pFinal1.getPieceType() == PieceType::Blank)) {
        pEnPass = theBoard[r1][c2].getPiece();
        theBoard[r1][c2].setPiece(pTemp);
    }

    // If it leads to current player's check
    if (isCheck(currCol)) {
        returnVec.emplace_back(true);
    } else {
        returnVec.emplace_back(false);
    }

    //If it leads to opponent player's check
    if (isCheck(otherCol)) {
        returnVec.emplace_back(true);
    } else {
        returnVec.emplace_back(false);
    }

    // If the move captures a piece
    if (pFinal1.getPieceType() != PieceType::Blank) {
        returnVec.emplace_back(true);
    } else {
        returnVec.emplace_back(false);
    }

    // If the piece is capturable after the move
    if (isCapturable(r2, c2)) {
        returnVec.emplace_back(true);
    } else {
        returnVec.emplace_back(false);
    }

    // If Castling results in capturable
    if ((r3 != -1) && isCapturable(r4, c4)) {
        returnVec[3] = true;
    }

    // Change (r1, c1) and (r2, c2) back to its original piece
    theBoard[r1][c1].setPiece(pInit1);
    theBoard[r2][c2].setPiece(pFinal1);
    
    //change back the king coordinates if necessary
    if (theBoard[r1][c1].getPiece().getPieceType() == PieceType::King) {
        if (currCol == PlayerColour::Black) {
            bKingC = c1;
            bKingR = r1;
        } else {
            wKingC = c1;
            wKingR = r1;
        }
    }

    // Change (r3, c3) and (r4, c4) back to its orignal piece, only if it changed
    if (r3 != -1) {
        theBoard[r3][c3].setPiece(pInit2);
        theBoard[r4][c4].setPiece(pFinal2);
    }

    if ((pInit1.getPieceType() == PieceType::Pawn) && (c1 != c2) && (pFinal1.getPieceType() == PieceType::Blank)) {
        theBoard[r1][c2].setPiece(pEnPass);
    }

    return returnVec;
}

int Board::simulateOpponent(int r1, int c1, int r2, int c2, PlayerColour c) {
    Piece pTemp = Piece{PlayerColour::NA, PieceType::Blank}; // Temporary piece to replace
    Piece pInit = theBoard[r1][c1].getPiece(); // Original piece at (r1, c1)
    Piece pFinal = theBoard[r2][c2].getPiece(); // Original piece at (r2, c2)
    Piece pCastle = pTemp;
    Piece pEnPass = pTemp;
    PlayerColour currCol = pInit.getColour(); // Colour of the piece moving
    PlayerColour otherCol = (currCol == PlayerColour::Black)?PlayerColour::White:PlayerColour::Black; // Opposite colour

    // Change piece at (r1, c1) and (r2, c2)
    theBoard[r1][c1].setPiece(pTemp); 
    theBoard[r2][c2].setPiece(pInit);

    //update king coordinate if necessary
    if (pInit.getPieceType() == PieceType::King) {
        if (currCol == PlayerColour::Black) {
            bKingC = c2;
            bKingR = r2;
        } else {
            wKingC = c2;
            wKingR = r2;
        }
        if ((r1 == r2) && (c1 == 4)) {
            if (c2 == 6) {
                pCastle = theBoard[r1][7].getPiece();
                theBoard[r1][5].setPiece(pCastle);
                theBoard[r1][7].setPiece(pTemp);
            }
            if (c2 == 2) {
                pCastle = theBoard[r1][0].getPiece();
                theBoard[r1][3].setPiece(pCastle);
                theBoard[r1][0].setPiece(pTemp);
            }
        }
    }

    if ((pInit.getPieceType() == PieceType::Pawn) && (c1 != c2) && (pFinal.getPieceType() == PieceType::Blank)) {
        pEnPass = theBoard[r1][c2].getPiece();
        theBoard[r1][c2].setPiece(pTemp);
    }

    int grade = 0;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (theBoard[i][j].getPiece().getColour() == otherCol) {
                vector<vector<int>> pMoves = returnPossibleMove(i, j);
                for (auto it = pMoves.begin(); it != pMoves.end(); ++it) {
                    int currGrade = 1;
                    vector<bool> information = simulate(i, j, (*it)[0], (*it)[1]);
                    if (information[1]) currGrade += 2;
                    if (information[2]) currGrade += returnScore(theBoard[(*it)[0]][(*it)[1]].getPiece().getPieceType());
                    if (information[3]) currGrade -= returnScore(theBoard[i][j].getPiece().getPieceType());
                    Player *currPlayer = curr;
                    if (c == PlayerColour::White) curr = b;
                    else curr = a;
                    if (isCheckMate()) currGrade += 50;
                    curr = currPlayer;
                    if (currGrade > grade) grade = currGrade;
                }
            }
        }
    }

    // Change (r1, c1) and (r2, c2) back to its original piece
    theBoard[r1][c1].setPiece(pInit);
    theBoard[r2][c2].setPiece(pFinal);
    
    //change back the king coordinates if necessary
    if (theBoard[r1][c1].getPiece().getPieceType() == PieceType::King) {
        if (currCol == PlayerColour::Black) {
            bKingC = c1;
            bKingR = r1;
        } else {
            wKingC = c1;
            wKingR = r1;
        }
        if ((r1 == r2) && (c1 == 4)) {
            if (c2 == 6) {
                theBoard[r1][5].setPiece(pTemp);
                theBoard[r1][7].setPiece(pCastle);
            }
            if (c2 == 2) {
                theBoard[r1][3].setPiece(pTemp);
                theBoard[r1][0].setPiece(pCastle);
            }
        }
    }

    if ((pInit.getPieceType() == PieceType::Pawn) && (c1 != c2) && (pFinal.getPieceType() == PieceType::Blank)) {
        theBoard[r1][c2].setPiece(pEnPass);
    }

    return grade;
}

bool Board::makeMove(int x1, int y1, int x2, int y2, Piece p) {

    if (!(x1<8 && x1>=0 && y1<8 && y1>=0 && x2<8 && x2>=0 && y2<8 && y2>=0)) return false; // check if coordinates are valid.

    Piece currPiece = theBoard[x1][y1].getPiece();

    if ((currPiece.getColour() != curr->getPlayerColour()) || (currPiece.getPieceType() == PieceType::Blank)) return false;
    vector<vector<int>> validMoves = returnPossibleMove(x1,y1);

    for (size_t i = 0 ; i < validMoves.size() ; i++) {

        if (validMoves[i][0] == x2 && validMoves[i][1] == y2) { // if the destination coordinate is one of our possible moves.

            // Assess what kind of move it is : For White player and Black player
            if (curr->getPlayerColour() == PlayerColour::Black) { // Black Player

                // case 1. Pawn First move
                if (theBoard[x1][y1].getPiece().getPieceType() == PieceType::Pawn && x1 == 1 && x2 == 3) {
                    Piece tempPiece = theBoard[x1][y1].getPiece();
                    tempPiece.setHasMoved(true);
                    tempPiece.setEnPassant(true);
                    theBoard[x2][y2].changePiece(tempPiece);
                    theBoard[x1][y1].changePiece(Piece{PlayerColour::NA, PieceType::Blank});
                    setAllEnPassant(x2, y2);
                    return true;
                } 
                // case 2. EnPassant
                else if (theBoard[x2-1][y2].getPiece().getEnPassant() && (x1 == 4) && (x2 == 5) && (y1 != y2)) {
                    Piece tempPiece = theBoard[x1][y1].getPiece();
                    tempPiece.setHasMoved(true);
                    theBoard[x2][y2].changePiece(tempPiece); // go to the diagonal destination
                    theBoard[x1][y1].changePiece(Piece{PlayerColour::NA, PieceType::Blank});  // empty the original cell.
                    theBoard[x2-1][y2].changePiece(Piece{PlayerColour::NA, PieceType::Blank});  // empty the enpassant cell
                    setAllEnPassant();
                    return true;
                }
                // case 3. Pawn Promotion
                else if (theBoard[x1][y1].getPiece().getPieceType() == PieceType::Pawn && (x2 == 7)) {
                    Piece tempPiece = theBoard[x1][y1].getPiece();
                    tempPiece.setPieceType(p.getPieceType());
                    tempPiece.setHasMoved(true);
                    theBoard[x2][y2].changePiece(tempPiece);  // promote the pawn to the desired piece.
                    theBoard[x1][y1].changePiece(Piece{PlayerColour::NA, PieceType::Blank}); // empty the original cell.
                    setAllEnPassant();
                    return true;
                }
                // case 4. Castling
                else if (theBoard[x1][y1].getPiece().getPieceType()==PieceType::King && x2==x1 && (y2==y1+2 || y2==y1-2)) { //if it is a castling move
                    Piece top_right = theBoard[0][7].getPiece();
                    Piece top_left = theBoard[0][0].getPiece();
                    if (y2 == 6) { //castling to the right
                        Piece tempPiece = theBoard[x1][y1].getPiece();
                        tempPiece.setHasMoved(true);
                        theBoard[0][6].changePiece(tempPiece);
                        theBoard[0][4].changePiece(Piece{PlayerColour::NA, PieceType::Blank});
                        bKingR = 0;
                        bKingC = 6;
                        top_right.setHasMoved(true);
                        theBoard[0][5].changePiece(top_right);
                        theBoard[0][7].changePiece(Piece{PlayerColour::NA, PieceType::Blank});
                        setAllEnPassant();
                        return true;
                    } else { //castling to the left
                        Piece tempPiece = theBoard[x1][y1].getPiece();
                        tempPiece.setHasMoved(true);
                        theBoard[0][2].changePiece(tempPiece);
                        theBoard[0][4].changePiece(Piece{PlayerColour::NA, PieceType::Blank});
                        bKingR = 0;
                        bKingC = 2;
                        top_left.setHasMoved(true);
                        theBoard[0][3].changePiece(top_left);
                        theBoard[0][0].changePiece(Piece{PlayerColour::NA, PieceType::Blank});
                        setAllEnPassant();
                        return true;
                    }
                }
                else {
                    if (theBoard[x1][y1].getPiece().getPieceType() == PieceType::King) {
                        bKingR = x2;
                        bKingC = y2; 
                    }
                    Piece tempPiece = theBoard[x1][y1].getPiece();
                    tempPiece.setHasMoved(true);
                    theBoard[x2][y2].changePiece(tempPiece);  // change the destination cell to our piece.
                    theBoard[x1][y1].changePiece(Piece{PlayerColour::NA, PieceType::Blank});  // empty the original cell.
                    setAllEnPassant();
                    return true;
                }
            } else if (curr->getPlayerColour() == PlayerColour::White) {

                // White Player

                // case 1.  Pawn First move
                if (theBoard[x1][y1].getPiece().getPieceType() == PieceType::Pawn && x1 == 6 && x2 == 4) {
                    Piece tempPiece = theBoard[x1][y1].getPiece();
                    tempPiece.setHasMoved(true);
                    tempPiece.setEnPassant(true);
                    theBoard[x2][y2].changePiece(tempPiece);
                    theBoard[x1][y1].changePiece(Piece{PlayerColour::NA, PieceType::Blank});
                    setAllEnPassant(x2, y2);
                    return true;
                }

                // case 2. EnPassant
                else if (theBoard[x1][y1].getPiece().getEnPassant() && (x1 == 3) && (x2 == 2) && (y1 != y2)) {
                    Piece tempPiece = theBoard[x1][y1].getPiece();
                    tempPiece.setHasMoved(true);
                    theBoard[x2][y2].changePiece(tempPiece);
                    theBoard[x1][y1].changePiece(Piece{PlayerColour::NA, PieceType::Blank});
                    theBoard[x2+1][y2].changePiece(Piece{PlayerColour::NA, PieceType::Blank});
                    setAllEnPassant();
                    return true;
                }

                // case 3. Pawn Promotion
                else if (theBoard[x1][y1].getPiece().getPieceType() == PieceType::Pawn && x2 == 0) {
                    Piece tempPiece = theBoard[x1][y1].getPiece();
                    tempPiece.setPieceType(p.getPieceType());
                    tempPiece.setHasMoved(true);
                    theBoard[x2][y2].changePiece(tempPiece);
                    theBoard[x1][y1].changePiece(Piece{PlayerColour::NA, PieceType::Blank});
                    setAllEnPassant();
                    return true;
                }

                // case 4. Castling
                else if (currPiece.getPieceType()==PieceType::King && x2==x1 && (y2==y1+2 || y2==y1-2)) { // if it is a castling move
                    Piece bot_right = theBoard[7][7].getPiece();
                    Piece bot_left = theBoard[7][0].getPiece();
                    if (y2==y1+2) { //castling to the right
                        Piece tempPiece = theBoard[x1][y1].getPiece();
                        tempPiece.setHasMoved(true);
                        theBoard[7][6].changePiece(tempPiece);
                        theBoard[7][4].changePiece(Piece{PlayerColour::NA, PieceType::Blank});
                        wKingR = 7;
                        wKingC = 6;
                        bot_right.setHasMoved(true);
                        theBoard[7][5].changePiece(bot_right);
                        theBoard[7][7].changePiece(Piece{PlayerColour::NA, PieceType::Blank});
                        setAllEnPassant();
                        return true;
                    } else { //castling to the left
                        Piece tempPiece = theBoard[x1][y1].getPiece();
                        tempPiece.setHasMoved(true);
                        theBoard[7][2].changePiece(tempPiece);
                        theBoard[7][4].changePiece(Piece{PlayerColour::NA, PieceType::Blank});
                        wKingR = 7;
                        wKingC = 2;
                        bot_left.setHasMoved(true);
                        theBoard[7][3].changePiece(bot_left);
                        theBoard[7][0].changePiece(Piece{PlayerColour::NA, PieceType::Blank});
                        setAllEnPassant();
                        return true;
                    }
                }
                // else case
                else {
                    if (theBoard[x1][y1].getPiece().getPieceType() == PieceType::King) {
                        wKingR = x2;
                        wKingC = y2;
                    }
                    Piece tempPiece = theBoard[x1][y1].getPiece();
                    tempPiece.setHasMoved(true);
                    theBoard[x2][y2].changePiece(tempPiece);  // change the destination cell to our piece.
                    theBoard[x1][y1].changePiece(Piece{PlayerColour::NA, PieceType::Blank});  // empty the original cell.
                    setAllEnPassant();
                    return true;
                } 
            } // If Colour == black or white
            return false;
        } // If it's in valid move vector
    } // For loop
    return false;
}


vector<vector<int>> Board::returnPossibleMove(int r, int c) {
    if (theBoard[r][c].getPiece().getPieceType() == PieceType::Blank) return vector<vector<int>>{};
    vector<vector<int>> possibleMoves = returnPossibleMoveEmpty(r, c);
    PlayerColour currCol = theBoard[r][c].getPiece().getColour();
    PlayerColour otherCol = ((theBoard[r][c].getPiece().getColour() == PlayerColour::Black)?PlayerColour::White:PlayerColour::Black);

    for(auto it = possibleMoves.begin(); it != possibleMoves.end();) {
        int currR = (*it)[0];
        int currC = (*it)[1];
        if (theBoard[currR][currC].getPiece().getColour() == currCol) {
            possibleMoves.erase(it);
        } else if ((theBoard[r][c].getPiece().getPieceType() == PieceType::Pawn) && (theBoard[currR][currC].getPiece().getPieceType() != PieceType::Blank)) {
            possibleMoves.erase(it);
        } else if ((theBoard[r][c].getPiece().getPieceType() != PieceType::Knight) && (onThePath(r, c, currR, currC))) {
            possibleMoves.erase(it);      
        } else if (simulate(r, c, currR, currC)[0]) {
            possibleMoves.erase(it);
        } else {
            ++it;
        }
    }
    
    if (theBoard[r][c].getPiece().getPieceType() == PieceType::Pawn) {
        if (theBoard[r][c].getPiece().getColour() == PlayerColour::Black) {
            if (((r + 1) <= 7) && ((c + 1) <= 7)) {
                Piece pTemp1 = theBoard[r + 1][c + 1].getPiece();
                Piece pTemp2 = theBoard[r][c + 1].getPiece();
                if ((pTemp1.getPieceType() != PieceType::Blank) && (pTemp1.getColour() == otherCol)) possibleMoves.emplace_back(vector<int>{r + 1, c + 1});
                else if ((pTemp1.getPieceType() == PieceType::Blank) && pTemp2.getEnPassant() && (pTemp2.getColour() == otherCol) && !simulate(r, c, r + 1, c + 1)[0]) possibleMoves.emplace_back(vector<int>{r + 1, c + 1});
            }
            if (((r + 1) <= 7) && ((c - 1) >= 0)) {
                Piece pTemp1 = theBoard[r + 1][c - 1].getPiece();
                Piece pTemp2 = theBoard[r][c - 1].getPiece();
                if ((pTemp1.getPieceType() != PieceType::Blank) && (pTemp1.getColour() == otherCol)) possibleMoves.emplace_back(vector<int>{r + 1, c - 1});
                else if ((pTemp1.getPieceType() == PieceType::Blank) && pTemp2.getEnPassant() && (pTemp2.getColour() == otherCol) && !simulate(r, c, r + 1, c - 1)[0]) possibleMoves.emplace_back(vector<int>{r + 1, c - 1});
            }
        } else {
            if (((r - 1) >= 0) && ((c + 1) <= 7)) {
                Piece pTemp1 = theBoard[r - 1][c + 1].getPiece();
                Piece pTemp2 = theBoard[r][c + 1].getPiece();
                if ((pTemp1.getPieceType() != PieceType::Blank) && (pTemp1.getColour() == otherCol)) possibleMoves.emplace_back(vector<int>{r - 1, c + 1});
                else if ((pTemp1.getPieceType() == PieceType::Blank) && pTemp2.getEnPassant() && (pTemp2.getColour() == otherCol) && !simulate(r, c, r - 1, c + 1)[0]) possibleMoves.emplace_back(vector<int>{r - 1, c + 1});
            }
            if (((r - 1) >= 0) && ((c - 1) >= 0)) {
                Piece pTemp1 = theBoard[r - 1][c - 1].getPiece();
                Piece pTemp2 = theBoard[r][c - 1].getPiece();
                if ((pTemp1.getPieceType() != PieceType::Blank) && (pTemp1.getColour() == otherCol)) possibleMoves.emplace_back(vector<int>{r - 1, c - 1});
                else if ((pTemp1.getPieceType() == PieceType::Blank) && pTemp2.getEnPassant() && (pTemp2.getColour() == otherCol) && !simulate(r, c, r - 1, c - 1)[0]) possibleMoves.emplace_back(vector<int>{r - 1, c - 1});
            }
        }
    }

    if (!isCheck(currCol)) {
        if (c == 4) {
            if (((r == 0) || (r == 7)) && (theBoard[r][4].getPiece().getPieceType() == PieceType::King) && (!theBoard[r][4].getPiece().getHasMoved())) {
                Piece leftRook = theBoard[r][0].getPiece();
                Piece rightRook = theBoard[r][7].getPiece();
                if ((leftRook.getPieceType() == PieceType::Rook) && (leftRook.getColour() == currCol) && (!leftRook.getHasMoved())) {
                    if ((theBoard[r][1].getPiece().getPieceType() == PieceType::Blank) &&
                        (theBoard[r][2].getPiece().getPieceType() == PieceType::Blank) &&
                        (theBoard[r][3].getPiece().getPieceType() == PieceType::Blank)) {
                        if (!simulate(r, 4, r, 2, r, 0, r, 3)[0]) possibleMoves.emplace_back(vector<int>{r, 2});
                    }
                }
                if ((rightRook.getPieceType() == PieceType::Rook) && (rightRook.getColour() == currCol) && (!rightRook.getHasMoved())) {
                    if ((theBoard[r][5].getPiece().getPieceType() == PieceType::Blank) &&
                        (theBoard[r][6].getPiece().getPieceType() == PieceType::Blank)) {
                        if (!simulate(r, 4, r, 6, r, 7, r, 5)[0]) possibleMoves.emplace_back(vector<int>{r, 6});
                    }
                }
            }
        }
    }
    return possibleMoves;
}


vector<vector<int>> Board::returnPossibleMoveEmpty(int r, int c) {
    if ((r < 0) || (r > 7) || (c < 0) || (c > 7)) return vector<vector<int>>{};
    Piece p = theBoard[r][c].getPiece();
    vector<vector<int>> possibleMoves;
    switch (p.getPieceType()) {
        case PieceType::King:
            if ((r - 1) >= 0) possibleMoves.emplace_back(vector<int>{r - 1, c});
            if ((r + 1) <= 7) possibleMoves.emplace_back(vector<int>{r + 1, c});
            if ((c - 1) >= 0) possibleMoves.emplace_back(vector<int>{r, c - 1});
            if ((c + 1) <= 7) possibleMoves.emplace_back(vector<int>{r, c + 1});
            if (((r - 1) >= 0) && ((c - 1) >= 0)) possibleMoves.emplace_back(vector<int>{r - 1, c - 1});
            if (((r - 1) >= 0) && ((c + 1) <= 7)) possibleMoves.emplace_back(vector<int>{r - 1, c + 1});
            if (((r + 1) <= 7) && ((c - 1) >= 0)) possibleMoves.emplace_back(vector<int>{r + 1, c - 1});
            if (((r + 1) <= 7) && ((c + 1) <= 7)) possibleMoves.emplace_back(vector<int>{r + 1, c + 1});
            break;
        case PieceType::Queen:
            for(int i = 1; i < 8; ++i) {
                if ((r - i) >= 0) possibleMoves.emplace_back(vector<int>{r - i, c});
                if ((r + i) <= 7) possibleMoves.emplace_back(vector<int>{r + i, c});
                if ((c - i) >= 0) possibleMoves.emplace_back(vector<int>{r, c - i});
                if ((c + i) <= 7) possibleMoves.emplace_back(vector<int>{r, c + i});
                if (((r - i) >= 0) && ((c - i) >= 0)) possibleMoves.emplace_back(vector<int>{r - i, c - i});
                if (((r - i) >= 0) && ((c + i) <= 7)) possibleMoves.emplace_back(vector<int>{r - i, c + i});
                if (((r + i) <= 7) && ((c - i) >= 0)) possibleMoves.emplace_back(vector<int>{r + i, c - i});
                if (((r + i) <= 7) && ((c + i) <= 7)) possibleMoves.emplace_back(vector<int>{r + i, c + i});
            }
            break;
        case PieceType::Rook:
            for(int i = 1; i < 8; ++i) {
                if ((r - i) >= 0) possibleMoves.emplace_back(vector<int>{r - i, c});
                if ((r + i) <= 7) possibleMoves.emplace_back(vector<int>{r + i, c});
                if ((c - i) >= 0) possibleMoves.emplace_back(vector<int>{r, c - i});
                if ((c + i) <= 7) possibleMoves.emplace_back(vector<int>{r, c + i});
            }
            break;
        case PieceType::Bishop:
            for(int i = 1; i < 8; ++i) {
                if (((r - i) >= 0) && ((c - i) >= 0)) possibleMoves.emplace_back(vector<int>{r - i, c - i});
                if (((r - i) >= 0) && ((c + i) <= 7)) possibleMoves.emplace_back(vector<int>{r - i, c + i});
                if (((r + i) <= 7) && ((c - i) >= 0)) possibleMoves.emplace_back(vector<int>{r + i, c - i});
                if (((r + i) <= 7) && ((c + i) <= 7)) possibleMoves.emplace_back(vector<int>{r + i, c + i});
            }
            break;
        case PieceType::Knight:
            if (((r - 2) >= 0) && ((c - 1) >= 0)) possibleMoves.emplace_back(vector<int>{r - 2, c - 1});
            if (((r - 2) >= 0) && ((c + 1) <= 7)) possibleMoves.emplace_back(vector<int>{r - 2, c + 1});
            if (((r - 1) >= 0) && ((c + 2) <= 7)) possibleMoves.emplace_back(vector<int>{r - 1, c + 2});
            if (((r + 1) <= 7) && ((c + 2) <= 7)) possibleMoves.emplace_back(vector<int>{r + 1, c + 2});
            if (((r + 2) <= 7) && ((c + 1) <= 7)) possibleMoves.emplace_back(vector<int>{r + 2, c + 1});
            if (((r + 2) <= 7) && ((c - 1) >= 0)) possibleMoves.emplace_back(vector<int>{r + 2, c - 1});
            if (((r + 1) <= 7) && ((c - 2) >= 0)) possibleMoves.emplace_back(vector<int>{r + 1, c - 2});
            if (((r - 1) >= 0) && ((c - 2) >= 0)) possibleMoves.emplace_back(vector<int>{r - 1, c - 2});
            break;
        case PieceType::Pawn:
            if (p.getColour() == PlayerColour::Black) {
                if ((r + 1) <= 7) possibleMoves.emplace_back(vector<int>{r + 1, c});
                if (((r + 2) <= 7) && (!p.getHasMoved())) possibleMoves.emplace_back(vector<int>{r + 2, c});
            } else {
                if ((r - 1) >= 0) possibleMoves.emplace_back(vector<int>{r - 1, c});
                if (((r - 2) >= 0) && (!p.getHasMoved())) possibleMoves.emplace_back(vector<int>{r - 2, c});
            }
            break;
        default:
            break;
    }
    return possibleMoves;
}


bool Board::isCheckMate() {
    if (!isCheck(curr->getPlayerColour())) return false;

    PlayerColour colour = curr->getPlayerColour();

    for (int i = 0 ; i < 8 ; ++i) {
        for (int j = 0 ; j < 8 ; ++j) { // if a piece at one of 64 cells is the same colour as current player

            if (theBoard[i][j].getPiece().getColour() == colour) {  // if that piece in i, j coordinate is current player's piece

                if (returnPossibleMove(i, j).size() != 0) { // if there is a possible move for a piece.
                    return false; // then it is not a checkmate
                }
            }
        }
    }
    return true; /* when you check that all of the current player's pieces have no
                     move to avoid check, it is a checkmate.    */ 
}


bool Board::isStaleMate() {

    if (isCheck(curr->getPlayerColour())) return false;

    PlayerColour colour = curr->getPlayerColour();

    for (int i = 0 ; i < 8 ; ++i) {
        for (int j = 0 ; j < 8 ; ++j) { // if a piece at one of 64 cells is the same colour as current player

            if (theBoard[i][j].getPiece().getColour() == colour) {  // if that piece in i, j coordinate is current player's piece

                if (returnPossibleMove(i, j).size() != 0) { // if there is a possible move for a piece.
                    return false; // then it is not a stalemate
                }
            }
        }
    }
    return true; // when you check that all of the current player's pieces have nowhere to go, it is a stalemate.
}


bool Board::isCheck(PlayerColour currCol) {
    if(currCol == PlayerColour::White) {
        return isCapturable(wKingR, wKingC);
    } else {
        return isCapturable(bKingR, bKingC);
    }
}


bool Board::isCapturable(int targetRow, int targetCol) {
    PlayerColour currCol = theBoard[targetRow][targetCol].getPiece().getColour();
    PlayerColour otherCol;

    // otherCol == Opposite colour of currCol
    if(currCol == PlayerColour::White) {
        otherCol = PlayerColour::Black;
    } else {
        otherCol = PlayerColour::White;
    }

    // Check 8 directions that is possible to capture
    for(int i = 1; i < 8; i++) { // North
        if ((targetRow - i) < 0) break;
        Piece closestPiece = theBoard[targetRow - i][targetCol].getPiece();
        if (closestPiece.getColour() == currCol) break;
        PieceType pt = closestPiece.getPieceType();
        if ((pt == PieceType::King) && (i == 1)) return true;
        if ((pt == PieceType::Rook) || (pt == PieceType::Queen)) return true;
        if ((pt != PieceType::Blank)) break;
    }
    for(int i = 1; i < 8; i++) { // Northwest
        if (((targetRow - i) < 0) || ((targetCol + i) > 7)) break;
        Piece closestPiece = theBoard[targetRow - i][targetCol + i].getPiece();
        if (closestPiece.getColour() == currCol) break;
        PieceType pt = closestPiece.getPieceType();
        if ((currCol == PlayerColour::White) && (pt == PieceType::Pawn) && (i == 1)) return true;
        if ((pt == PieceType::King) && (i == 1)) return true;
        if ((pt == PieceType::Bishop) || (pt == PieceType::Queen)) return true;
        if ((pt != PieceType::Blank)) break;
    }
    for(int i = 1; i < 8; ++i) { // West 
        if ((targetCol + i) > 7) break;
        Piece closestPiece = theBoard[targetRow][targetCol + i].getPiece();
        if (closestPiece.getColour() == currCol) break;
        PieceType pt = closestPiece.getPieceType();
        if ((pt == PieceType::King) && (i == 1)) return true;
        if ((pt == PieceType::Rook) || (pt == PieceType::Queen)) return true;
        if ((pt != PieceType::Blank)) break;
    }
    for(int i = 1; i < 8; ++i) { // Southwest 
        if (((targetRow + i) > 7) || ((targetCol + i) > 7)) break;
        Piece closestPiece = theBoard[targetRow + i][targetCol + i].getPiece();
        if (closestPiece.getColour() == currCol) break;
        PieceType pt = closestPiece.getPieceType();
        if ((currCol == PlayerColour::Black) && (pt == PieceType::Pawn) && (i == 1)) return true;
        if ((pt == PieceType::King) && (i == 1)) return true;
        if ((pt == PieceType::Bishop) || (pt == PieceType::Queen)) return true;
        if ((pt != PieceType::Blank)) break;
    }
    for(int i = 1; i < 8; ++i) { // South 
        if ((targetRow + i) > 7) break;
        Piece closestPiece = theBoard[targetRow + i][targetCol].getPiece();
        if (closestPiece.getColour() == currCol) break;
        PieceType pt = closestPiece.getPieceType();
        if ((pt == PieceType::King) && (i == 1)) return true;
        if ((pt == PieceType::Rook) || (pt == PieceType::Queen)) return true;
        if ((pt != PieceType::Blank)) break;
    }
    for(int i = 1; i < 8; ++i) { // Southeast 
        if (((targetRow + i) > 7) || ((targetCol - i) < 0)) break;
        Piece closestPiece = theBoard[targetRow + i][targetCol - i].getPiece();
        if (closestPiece.getColour() == currCol) break;
        PieceType pt = closestPiece.getPieceType();
        if ((currCol == PlayerColour::Black) && (pt == PieceType::Pawn) && (i == 1)) return true;
        if ((pt == PieceType::King) && (i == 1)) return true;
        if ((pt == PieceType::Bishop) || (pt == PieceType::Queen)) return true;
        if ((pt != PieceType::Blank)) break;
    }
    for(int i = 1; i < 8; ++i) { // East 
        if ((targetCol - i) < 0) break;
        Piece closestPiece = theBoard[targetRow][targetCol - i].getPiece();
        if (closestPiece.getColour() == currCol) break;
        PieceType pt = closestPiece.getPieceType();
        if ((pt == PieceType::King) && (i == 1)) return true;
        if ((pt == PieceType::Rook) || (pt == PieceType::Queen)) return true;
        if ((pt != PieceType::Blank)) break;
    }
    for(int i = 1; i < 8; ++i) { // Northeast 
        if (((targetRow - i) < 0) || ((targetCol - i) < 0)) break;
        Piece closestPiece = theBoard[targetRow - i][targetCol - i].getPiece();
        if (closestPiece.getColour() == currCol) break;
        PieceType pt = closestPiece.getPieceType();
        if ((currCol == PlayerColour::White) && (pt == PieceType::Pawn) && (i == 1)) return true;
        if ((pt == PieceType::King) && (i == 1)) return true;
        if ((pt == PieceType::Bishop) || (pt == PieceType::Queen)) return true;
        if ((pt != PieceType::Blank)) break;
    }

    if (((targetRow - 2) >= 0) && ((targetCol - 1) >= 0)) { // Two up, One left
        if ((theBoard[targetRow - 2][targetCol - 1].getPiece().getPieceType() == PieceType::Knight) && 
            (theBoard[targetRow - 2][targetCol - 1].getPiece().getColour() == otherCol)) return true;
    } 
    if (((targetRow - 2) >= 0) && ((targetCol + 1) <= 7)) { // Two up, One right
        if ((theBoard[targetRow - 2][targetCol + 1].getPiece().getPieceType() == PieceType::Knight) && 
            (theBoard[targetRow - 2][targetCol + 1].getPiece().getColour() == otherCol)) return true;
    } 
    if (((targetRow - 1) >= 0) && ((targetCol + 2) <= 7)) { // One up, Two right
        if ((theBoard[targetRow - 1][targetCol + 2].getPiece().getPieceType() == PieceType::Knight) && 
            (theBoard[targetRow - 1][targetCol + 2].getPiece().getColour() == otherCol)) return true;
    } 
    if (((targetRow + 1) <= 7) && ((targetCol + 2) <= 7)) { // One down, Two right
        if ((theBoard[targetRow + 1][targetCol + 2].getPiece().getPieceType() == PieceType::Knight) && 
            (theBoard[targetRow + 1][targetCol + 2].getPiece().getColour() == otherCol)) return true;
    } 
    if (((targetRow + 2) <= 7) && ((targetCol + 1) <= 7)) { // Two down, One right
        if ((theBoard[targetRow + 2][targetCol + 1].getPiece().getPieceType() == PieceType::Knight) && 
            (theBoard[targetRow + 2][targetCol + 1].getPiece().getColour() == otherCol)) return true;
    } 
    if (((targetRow + 2) <= 7) && ((targetCol - 1) >= 0)) { // Two down, One left
        if ((theBoard[targetRow + 2][targetCol - 1].getPiece().getPieceType() == PieceType::Knight) && 
            (theBoard[targetRow + 2][targetCol - 1].getPiece().getColour() == otherCol)) return true;
    } 
    if (((targetRow + 1) <= 7) && ((targetCol - 2) >= 0)) { // One down, Two left
        if ((theBoard[targetRow + 1][targetCol - 2].getPiece().getPieceType() == PieceType::Knight) && 
            (theBoard[targetRow + 1][targetCol - 2].getPiece().getColour() == otherCol)) return true;
    } 
    if (((targetRow - 1) >= 0) && ((targetCol - 2) >= 0)) { // One up, Two left
        if ((theBoard[targetRow - 1][targetCol - 2].getPiece().getPieceType() == PieceType::Knight) && 
            (theBoard[targetRow - 1][targetCol - 2].getPiece().getColour() == otherCol)) return true;
    }

    return false;
}

vector<int> Board::returnComputerMove(PlayerColour c, int level) {
    vector<vector<int>> bestMove{{-1, -1, -1, -1}};
    int grade = -10000;
    switch (level) {
        case 1:
            for(int i = 0; i < 8; ++i) {
                for (int j = 0; j < 8; ++j) {
                    if (theBoard[i][j].getPiece().getColour() == c) {
                        vector<vector<int>> pMoves = returnPossibleMove(i, j);
                        if (pMoves.size() != 0) return vector<int>{i, j, pMoves[0][0], pMoves[0][1]};
                    }
                }
            }
            break;
        case 2:
            for (int i = 0; i < 8; ++i) {
                for (int j = 0; j < 8; ++j) {
                    if (theBoard[i][j].getPiece().getColour() == c) {
                        vector<vector<int>> pMoves = returnPossibleMove(i, j);
                        for (auto it = pMoves.begin(); it != pMoves.end(); ++it) {
                            int currGrade = 1;
                            vector<bool> information = simulate(i, j, (*it)[0], (*it)[1]);
                            if (information[1]) currGrade += 2;
                            if (information[2]) currGrade += 1;
                            if (currGrade == grade) {
                                bestMove.emplace_back(vector<int>{i, j, (*it)[0], (*it)[1]});
                            } else if (currGrade > grade) {
                                bestMove.clear();
                                bestMove.emplace_back(vector<int>{i, j, (*it)[0], (*it)[1]});
                                grade = currGrade;
                            }
                        }
                    }
                }
            }
            return bestMove[rand() % bestMove.size()];
        case 3:
            for (int i = 0; i < 8; ++i) {
                for (int j = 0; j < 8; ++j) {
                    if (theBoard[i][j].getPiece().getColour() == c) {
                        vector<vector<int>> pMoves = returnPossibleMove(i, j);
                        for (auto it = pMoves.begin(); it != pMoves.end(); ++it) {
                            int currGrade = 1;
                            vector<bool> information = simulate(i, j, (*it)[0], (*it)[1]);
                            if (information[1]) currGrade += 2;
                            if (information[2]) currGrade += 1;
                            if (information[3]) currGrade -= 2;
                            if (c == PlayerColour::White) curr = b;
                            else curr = a;
                            if (isCheckMate()) currGrade += 50;
                            if (curr == b) curr = a;
                            else curr = b;
                            if (currGrade == grade) {
                                bestMove.emplace_back(vector<int>{i, j, (*it)[0], (*it)[1]});
                            } else if (currGrade > grade) {
                                bestMove.clear();
                                bestMove.emplace_back(vector<int>{i, j, (*it)[0], (*it)[1]});
                                grade = currGrade;
                            }
                        }
                    }
                }
            }
            return bestMove[rand() % bestMove.size()];
        case 4:
            for (int i = 0; i < 8; ++i) {
                for (int j = 0; j < 8; ++j) {
                    if (theBoard[i][j].getPiece().getColour() == c) {
                        vector<vector<int>>pMoves = returnPossibleMove(i, j);
                        for (auto it = pMoves.begin(); it != pMoves.end(); ++it) {
                            int currGrade = 1;
                            vector<bool>information = simulate(i, j, (*it)[0], (*it)[1]);
                            if (information[1]) currGrade += 2;
                            if (information[2]) currGrade += returnScore(theBoard[(*it)[0]][(*it)[1]].getPiece().getPieceType());
                            if (information[3]) currGrade -= returnScore(theBoard[i][j].getPiece().getPieceType());
                            Player *currPlayer = curr;
                            if (c == PlayerColour::White) curr = b;
                            else curr = a;
                            if (isCheckMate()) currGrade += 50;
                            curr = currPlayer;
                            int oppScore = simulateOpponent(i, j, (*it)[0], (*it)[1], c);
                            currGrade -= oppScore;
                            if (currGrade == grade) {
                                bestMove.emplace_back(vector<int>{i, j, (*it)[0], (*it)[1]});
                            } else if (currGrade > grade) {
                                bestMove.clear();
                                bestMove.emplace_back(vector<int>{i, j, (*it)[0], (*it)[1]});
                                grade = currGrade;
                            }
                        }
                    }
                }
            }
            return bestMove[rand() % bestMove.size()];
            break;
        default:
            break;
    }
    return bestMove[rand() % bestMove.size()];
}

bool Board::oneKing() {
    int wKing = 0;
    int bKing = 0;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if(theBoard[i][j].getPiece().getPieceType() == PieceType::King) {
                if (theBoard[i][j].getPiece().getColour() == PlayerColour::White) ++wKing;
                else ++bKing;
            }
        }
    }
    return ((wKing == 1) && (bKing == 1));
}

std::ostream &operator<<(std::ostream &o, const Board &b) {
    return o << *(b.td) << endl;
}


