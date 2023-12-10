#include "board.h"
#include "player.h"
#include "piece.h"
#include <iostream>
#include <string>

int main()
{
    Xwindow window{800, 800};
    Board myBoard{window};
    Player whitePlayer{PlayerColour::White, PlayerType::ND, 0};
    Player blackPlayer{PlayerColour::Black, PlayerType::ND, 0};
    Player *startingPlayer = &whitePlayer;
    std::string l;
    std::string playerTypeSL = "PQKNRB";
    vector<PieceType> playerTypeList{PieceType::Pawn, PieceType::Queen, PieceType::King, PieceType::Knight, PieceType::Rook, PieceType::Bishop};

    bool gameStarted = false;
    std::cout << myBoard;
    while (true)
    {
        std::cin >> l;
        if (std::cin.eof())
            break;
        if (l == "game")
        {
            if (gameStarted) continue;
            std::string output = "Game Started\n";
            output += "Starting Player: ";
            if (startingPlayer->getPlayerColour() == PlayerColour::Black) output += "Black";
            else output += "White";
            output += "\n";
            std::cin >> l;
            if (l == "human")
            {
                whitePlayer.setPlayerType(PlayerType::Human);
                output += "White Player: Human\n";
            }
            else if (l == "computer1")
            {
                whitePlayer.setPlayerType(PlayerType::Computer1);
                output += "White Player: Computer1\n";
            }
            else if (l == "computer2")
            {
                whitePlayer.setPlayerType(PlayerType::Computer2);
                output += "White Player: Computer2\n";
            }
            else if (l == "computer3")
            {
                whitePlayer.setPlayerType(PlayerType::Computer3);
                output += "White Player: Computer3\n";
            }
            else if (l == "computer4")
            {
                whitePlayer.setPlayerType(PlayerType::Computer4);
                output += "White Player: Computer4\n";
            }
            std::cin >> l;
            if (l == "human")
            {
                blackPlayer.setPlayerType(PlayerType::Human);
                output += "Black Player: Human\n";
            }
            else if (l == "computer1")
            {
                blackPlayer.setPlayerType(PlayerType::Computer1);
                output += "Black Player: Computer1\n";
            }
            else if (l == "computer2")
            {
                blackPlayer.setPlayerType(PlayerType::Computer2);
                output += "Black Player: Computer2\n";
            }
            else if (l == "computer3")
            {
                blackPlayer.setPlayerType(PlayerType::Computer3);
                output += "Black Player: Computer3\n";
            }
            else if (l == "computer4")
            {
                blackPlayer.setPlayerType(PlayerType::Computer4);
                output += "Black Player: Computer4\n";
            }
            myBoard.init(&whitePlayer, &blackPlayer, startingPlayer);
            startingPlayer = &whitePlayer;
            gameStarted = true;
            std::cout << output << endl;
            std::cout << myBoard;
        }
        else if (l == "resign")
        {
            if (!gameStarted)
                continue;
            Player *curr = myBoard.getCurr();
            if (curr == &blackPlayer)
            {
                std::cout << "Black player resigned!" << std::endl;
                whitePlayer.addScore(1);
            }
            else
            {
                std::cout << "White player resigned!" << std::endl;
                blackPlayer.addScore(1);
            }
            myBoard.clearBoard(window);
            gameStarted = false;
        }
        else if (l == "move")
        {
            if (!gameStarted)
                continue;
            Player *currPlayer = myBoard.getCurr();
            bool successfulMove = false;
            if (currPlayer->getPlayerType() == PlayerType::Human)
            {
                std::string s1, s2, s3;
                int r1, c1, r2, c2;
                std::cin >> s1 >> s2;
                c1 = s1[0] - 'a'; // convert alphabetical char to int
                c2 = s2[0] - 'a';
                r1 = '8' - s1[1];
                r2 = '8' - s2[1];
                PieceType pt = PieceType::Blank;
                char temp = toupper(cin.peek()); // peek to see if addtional arg is provided for pawn promotion
                for (int i = 0; i < 6; ++i)
                {
                    if (playerTypeSL[i] == temp)
                    {
                        std::cin >> s3;              // if it is provided, read it in
                        pt = playerTypeList[i]; // get the player type for praw promotion
                    }
                }
                Piece promote_pawn = Piece{currPlayer->getPlayerColour(), pt};
                successfulMove = myBoard.makeMove(r1, c1, r2, c2, promote_pawn);
            }
            else
            {
                int level;
                if (myBoard.getCurr()->getPlayerType() == PlayerType::Computer1) level = 1;
                else if (myBoard.getCurr()->getPlayerType() == PlayerType::Computer2) level = 2; 
                else if (myBoard.getCurr()->getPlayerType() == PlayerType::Computer3) level = 3;
                else if (myBoard.getCurr()->getPlayerType() == PlayerType::Computer4) level = 4;
                std::vector<int> move = myBoard.returnComputerMove(myBoard.getCurr()->getPlayerColour(), level);
                successfulMove = myBoard.makeMove(move[0], move[1], move[2], move[3], Piece{myBoard.getCurr()->getPlayerColour(), PieceType::Queen});
                
                // computer auto make a move
            }
            if (successfulMove)
            {
                myBoard.addTurn();
                if (myBoard.getCurr() == &whitePlayer)
                {
                    myBoard.setCurr(&blackPlayer);
                    if (myBoard.isCheckMate())
                    {
                        std::cout << "Checkmate! White wins!" << std::endl;
                        whitePlayer.addScore(1);
                        myBoard.clearBoard(window);
                        gameStarted = false;
                    }
                    else if (myBoard.isStaleMate())
                    {
                        std::cout << "Stalemate!" << std::endl;
                        whitePlayer.addScore(0.5);
                        blackPlayer.addScore(0.5);
                        myBoard.clearBoard(window);
                        gameStarted = false;
                    }
                    else if (myBoard.isCheck(blackPlayer.getPlayerColour()))
                    {
                        std::cout << "Black is in check." << std::endl;
                    }
                }
                else
                {
                    myBoard.setCurr(&whitePlayer);
                    if (myBoard.isCheckMate())
                    {
                        std::cout << "Checkmate! Black wins!" << std::endl;
                        blackPlayer.addScore(1);
                        myBoard.clearBoard(window);
                        gameStarted = false;
                    }
                    else if (myBoard.isStaleMate())
                    {
                        std::cout << "Stalemate!" << std::endl;
                        whitePlayer.addScore(0.5);
                        blackPlayer.addScore(0.5);
                        myBoard.clearBoard(window);
                        gameStarted = false;
                    }
                    else if (myBoard.isCheck(whitePlayer.getPlayerColour()))
                    {
                        std::cout << "White is in check." << std::endl;
                    }
                }
            }
            else
            {
                std::cout << "Invalid Move" << std::endl;
            }
            std::cout << myBoard;
        }
        else if (l == "setup")
        {
            if (gameStarted)
                continue;
            std::cout << "Entering setup step" << std::endl;
            myBoard.emptyBoard();
            while (true)
            {
                std::cin >> l;
                if (std::cin.fail()) break;
                if (l == "+")
                {
                    std::string coord;
                    std::cin >> l >> coord;
                    int row = '8' - coord[1];
                    int col = coord[0] - 'a';
                    if (l[0] == 'K')
                    {
                        myBoard.setPiece(row, col, Piece{PlayerColour::White, PieceType::King});
                    }
                    else if (l[0] == 'Q')
                    {
                        myBoard.setPiece(row, col, Piece{PlayerColour::White, PieceType::Queen});
                    }
                    else if (l[0] == 'R')
                    {
                        myBoard.setPiece(row, col, Piece{PlayerColour::White, PieceType::Rook});
                    }
                    else if (l[0] == 'B')
                    {
                        myBoard.setPiece(row, col, Piece{PlayerColour::White, PieceType::Bishop});
                    }
                    else if (l[0] == 'N')
                    {
                        myBoard.setPiece(row, col, Piece{PlayerColour::White, PieceType::Knight});
                    }
                    else if (l[0] == 'P')
                    {
                        myBoard.setPiece(row, col, Piece{PlayerColour::White, PieceType::Pawn});
                    }
                    else if (l[0] == 'k')
                    {
                        myBoard.setPiece(row, col, Piece{PlayerColour::Black, PieceType::King});
                    }
                    else if (l[0] == 'q')
                    {
                        myBoard.setPiece(row, col, Piece{PlayerColour::Black, PieceType::Queen});
                    }
                    else if (l[0] == 'r')
                    {
                        myBoard.setPiece(row, col, Piece{PlayerColour::Black, PieceType::Rook});
                    }
                    else if (l[0] == 'b')
                    {
                        myBoard.setPiece(row, col, Piece{PlayerColour::Black, PieceType::Bishop});
                    }
                    else if (l[0] == 'n')
                    {
                        myBoard.setPiece(row, col, Piece{PlayerColour::Black, PieceType::Knight});
                    }
                    else if (l[0] == 'p')
                    {
                        myBoard.setPiece(row, col, Piece{PlayerColour::Black, PieceType::Pawn});
                    }
                }
                else if (l == "-")
                {
                    std::cin >> l;
                    int row = '8' - l[1];
                    int col = l[0] - 'a';
                    myBoard.setPiece(row, col, Piece{PlayerColour::NA, PieceType::Blank});
                }
                else if (l == "=")
                {
                    std::cin >> l;
                    if (l == "white")
                    {
                        startingPlayer = &whitePlayer;
                        std::cout << "Starting colour setted to White" << std::endl;
                    }
                    else
                    {
                        startingPlayer = &blackPlayer;
                        std::cout << "Starting colour setted to Black" << std::endl;
                    }
                }
                else if (l == "done")
                {
                    bool isError = false;
                    if (!myBoard.oneKing())
                    {
                        std::cout << "There are inadequate number of King of same colour. Please revise your board" << std::endl;
                        isError = true;
                    }
                    PlayerColour otherCol = (startingPlayer->getPlayerColour() == PlayerColour::Black)?PlayerColour::White:PlayerColour::Black;
                    if (myBoard.isCheck(otherCol)) {
                        std::cout << "Second player is in check. Please revise your board" << std::endl;
                        isError = true;
                    }
                    if (!isError) {
                        myBoard.resetKing();
                        std::cout << "Exiting setup mode" << std::endl;
                        break;
                    }
                }
                std::cout << myBoard;
            }
        }
    }
    std::cout << "Final Score:" << endl;
    std::cout << "White: " << whitePlayer.getScore() << std::endl;
    std::cout << "Black: " << blackPlayer.getScore() << std::endl;
}
