#ifndef __BOARD_H__
#define __BOARD_H__

#include <vector>
#include <string>

#define DEBUG 0

typedef unsigned long U64;
typedef unsigned char U8;

const U8 WHITE_PAWN   =  2;
const U8 WHITE_ROOK   =  4;
const U8 WHITE_KNIGHT =  6;
const U8 WHITE_BISHOP =  8;
const U8 WHITE_QUEEN  = 10;
const U8 WHITE_KING   = 12;

const U8 BLACK_PAWN   =  3;
const U8 BLACK_ROOK   =  5;
const U8 BLACK_KNIGHT =  7;
const U8 BLACK_BISHOP =  9;
const U8 BLACK_QUEEN  = 11;
const U8 BLACK_KING   = 13;

const U8 EMPTY = 0;

class Board {
    public:
        // Tabla de sah - reprezentare interna
        U8 chessBoard[8][8] = {
            BLACK_ROOK,BLACK_KNIGHT,BLACK_BISHOP,BLACK_QUEEN,BLACK_KING,BLACK_BISHOP,BLACK_KNIGHT,BLACK_ROOK,
            BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN, BLACK_PAWN,
            EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
            EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
            EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
            EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
            WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN, WHITE_PAWN,
            WHITE_ROOK, WHITE_KNIGHT, WHITE_BISHOP, WHITE_QUEEN, WHITE_KING, WHITE_BISHOP, WHITE_KNIGHT, WHITE_ROOK 
        };

        // Variabile necesare pentru rocade
        bool whiteKingMoved = false;
        bool whiteLeftRookMoved = false;
        bool whiteRightRookMoved = false;
        bool blackKingMoved = false;
        bool blackLeftRookMoved = false;
        bool blackRightRookMoved = false;

        // Variabile necesare pentru enPassant
        bool enPassantPossible;
        int xEnPassant, yEnPassant;

        Board() {}

        Board(const Board &copyBoard) {
            for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 8; j++) {
                    this->chessBoard[i][j] = copyBoard.chessBoard[i][j];
                }
            }
            this->whiteKingMoved = copyBoard.whiteKingMoved;
            this->whiteLeftRookMoved = copyBoard.whiteLeftRookMoved;
            this->whiteRightRookMoved = copyBoard.whiteRightRookMoved;
            this->blackKingMoved = copyBoard.blackKingMoved;
            this->blackLeftRookMoved = copyBoard.blackLeftRookMoved;
            this->blackRightRookMoved = copyBoard.blackRightRookMoved;

            this->enPassantPossible = copyBoard.enPassantPossible;
            this->xEnPassant = copyBoard.xEnPassant;
            this->yEnPassant = copyBoard.yEnPassant;
        }

    void move(char* movement);
    void printBoard();
    char *getWhiteLegalMoves();
    char *getBlackLegalMoves();
    std::vector<std::string> getPawnMoves(int x, int y);
    std::vector<std::string> getAllPawnMoves(int engineColor);
    std::vector<std::string> getKnightMoves(int x, int y);
    std::vector<std::string> getAllKnightMoves(int engineColor);
    std::vector<std::string> getRookMoves(int x, int y);
    std::vector<std::string> getAllRookMoves(int engineColor);
    std::vector<std::string> getBishopMoves(int x, int y);
    std::vector<std::string> getAllBishopMoves(int engineColor);
    std::vector<std::string> getAllKingMoves(int engineColor);
    std::vector<std::string> getAllQueenMoves(int engineColor);
    bool isCheck(int engineColor);
    bool checkIfEmptyBetween(int line, int x, int y);
};

#endif
