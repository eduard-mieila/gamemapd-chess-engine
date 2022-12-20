#include <iostream>
#include <string.h>
#include "board.h"
#include "engine_io.h"

#define WHITE 0
#define BLACK 1

using namespace std;

/**
 * Transforma din coordonatele de reprezentare interna, in notatii de forma
 * "pure coordinate notation". Promotion poate fi 0, caz in care piesa nu
 * se transforma, sau una dintre valorile pentru Regina, Cal, Nebun sau
 * Turn. Nu se fac verificari, este responsabilitatea utilizatorului
 * in folosirea ei pentru a nu face mutari invalide.
 */
char *coords_to_string(int si, int sj, int di, int dj, int promotion) {
  char *movement = (char *)malloc(7 * sizeof(char));

    switch (si) {
        case 7:
            movement[1] = '1';
            break;

        case 6:
            movement[1] = '2';
            break;

        case 5:
            movement[1] = '3';
            break;
        
        case 4:
            movement[1] = '4';
            break;

        case 3:
            movement[1] = '5';
            break;

        case 2:
            movement[1] = '6';
            break;

        case 1:
            movement[1] = '7';
            break;
        
        case 0:
            movement[1] = '8';
            break;
    }


    switch (sj) {
        case 0:
            movement[0] = 'a';
            break;

        case 1:
            movement[0] = 'b';
            break;

        case 2:
            movement[0] = 'c';
            break;

        case 3:
            movement[0] = 'd';
            break;

        case 4:
            movement[0] = 'e';
            break;

        case 5:
            movement[0] = 'f';
            break;

        case 6:
            movement[0] = 'g';
            break;

        case 7:
            movement[0] = 'h';
            break;

    }

    switch (di) {
        case 7:
            movement[3] = '1';
            break;

        case 6:
            movement[3] = '2';
            break;

        case 5:
            movement[3] = '3';
            break;
        
        case 4:
            movement[3] = '4';
            break;

        case 3:
            movement[3] = '5';
            break;

        case 2:
            movement[3] = '6';
            break;

        case 1:
            movement[3] = '7';
            break;
        
        case 0:
            movement[3] = '8';
            break;
    }

    switch (dj) {
        case 0:
            movement[2] = 'a';
            break;

        case 1:
            movement[2] = 'b';
            break;

        case 2:
            movement[2] = 'c';
            break;

        case 3:
            movement[2] = 'd';
            break;

        case 4:
            movement[2] = 'e';
            break;

        case 5:
            movement[2] = 'f';
            break;

        case 6:
            movement[2] = 'g';
            break;

        case 7:
            movement[2] = 'h';
            break;

    }

    if (promotion) {
        if (promotion == WHITE_QUEEN || promotion == BLACK_QUEEN) {
            movement[4] = 'q';
        }

        if (promotion == WHITE_ROOK || promotion == BLACK_ROOK) {
            movement[4] = 'r';
        }

        if (promotion == WHITE_KNIGHT || promotion == BLACK_KNIGHT) {
            movement[4] = 'n';
        }

        if (promotion == WHITE_BISHOP || promotion == BLACK_BISHOP) {
            movement[4] = 'b';
        }

      movement[5] = '\n';
      movement[6] = '\0';
      return movement;
    }

    movement[4] = '\n';
    movement[5] = '\0';
    return movement;
}

/*
 * Primeste ca parametru indicatia de unde trebuie mutata o piesa si unde
 * trebuie mutata.
 * Daca ca rezultat al mutarii, o piesa a adversarului este "batuta", 
 * adica se suprapune cu mutarea piesei ce am mutat-o, atunci se sterge
 * din baza de data piesa batuta a adversarului.
 * Transforma pionul in alta piesa daca este cazul.
 * De exemplu: movement -> b2b4
 *             movement -> b7b8q (caz in care pionul devine queen)
 *                               (similar pentru r,n,b)
 */
void Board::move(char* movement) {
    int si, sj, di, dj;

    switch (movement[1]) {
        case '1':
            si = 7;
            break;

        case '2':
            si = 6;
            break;

        case '3':
            si = 5;
            break;
        
        case '4':
            si = 4;
            break;

        case '5':
            si = 3;
            break;

        case '6':
            si = 2;
            break;

        case '7':
            si = 1;
            break;
        
        case '8':
            si = 0;
            break;
    }


    switch (movement[0]) {
        case 'a':
            sj = 0;
            break;

        case 'b':
            sj = 1;
            break;

        case 'c':
            sj = 2;
            break;

        case 'd':
            sj = 3;
            break;

        case 'e':
            sj = 4;
            break;

        case 'f':
            sj = 5;
            break;

        case 'g':
            sj = 6;
            break;

        case 'h':
            sj = 7;
            break;

    }

    switch (movement[3]) {
        case '1':
            di = 7;
            break;

        case '2':
            di = 6;
            break;

        case '3':
            di = 5;
            break;
        
        case '4':
            di = 4;
            break;

        case '5':
            di = 3;
            break;

        case '6':
            di = 2;
            break;

        case '7':
            di = 1;
            break;
        
        case '8':
            di = 0;
            break;
    }

    switch (movement[2]) {
        case 'a':
            dj = 0;
            break;

        case 'b':
            dj = 1;
            break;

        case 'c':
            dj = 2;
            break;

        case 'd':
            dj = 3;
            break;

        case 'e':
            dj = 4;
            break;

        case 'f':
            dj = 5;
            break;

        case 'g':
            dj = 6;
            break;

        case 'h':
            dj = 7;
            break;

    }


    // Actualizare variabile necesare pentru rocade
    if (si == 0 && sj == 0) {
        blackLeftRookMoved = true;
    }

    if (si == 0 && sj == 7) {
        blackRightRookMoved = true;
    }

    if (si == 0 && sj == 4) {
        blackKingMoved = true;
    }

    if (si == 7 && sj == 0) {
        whiteLeftRookMoved = true;
    }

    if (si == 7 && sj == 7) {
        whiteRightRookMoved = true;
    }

    if (si == 7 && sj == 4) {
        whiteKingMoved = true;
    }


    // Actualizare variabile pentru en passant
    if (chessBoard[si][sj] == WHITE_PAWN && si == 6 && di == 4) {
        enPassantPossible = true;
        xEnPassant = di;
        yEnPassant = dj;
    }

    if (chessBoard[si][sj] == BLACK_PAWN && si == 1 && di == 3) {
        enPassantPossible = true;
        xEnPassant = di;
        yEnPassant = dj;
    }


    if (strlen(movement) == 5) {
        // Actualizare speciala in reprezentarea interna pentru rocade
        if (strncmp(movement, "e1g1", 4) == 0 && chessBoard[si][sj] == WHITE_KING) {
            chessBoard[7][5] = WHITE_ROOK;
            chessBoard[7][7] = EMPTY;
            whiteKingMoved = true;
            whiteRightRookMoved = true;
        }

        if (strncmp(movement, "e1c1", 4) == 0 && chessBoard[si][sj] == WHITE_KING) {
            chessBoard[7][3] = WHITE_ROOK;
            chessBoard[7][0] = EMPTY;
            whiteKingMoved = true;
            whiteLeftRookMoved = true;
        }

        if (strncmp(movement, "e8g8", 4) == 0 && chessBoard[si][sj] == BLACK_KING) {
            chessBoard[0][5] = BLACK_ROOK;
            chessBoard[0][7] = EMPTY;
            blackKingMoved = true;
            blackRightRookMoved = true;
        }

        if (strncmp(movement, "e8c8", 4) == 0 && chessBoard[si][sj] == BLACK_KING) {
            chessBoard[0][3] = BLACK_ROOK;
            chessBoard[0][0] = EMPTY;
            blackKingMoved = true;
            blackLeftRookMoved = true;
        }


        // Actualizare speciala pentru en passant
        if (enPassantPossible) {
            if (si == xEnPassant && (sj == yEnPassant - 1 || sj == yEnPassant + 1)) {
                if (chessBoard[si][sj] == WHITE_PAWN) {
                    if (di == 2 && dj == yEnPassant) {
                        chessBoard[xEnPassant][yEnPassant] = EMPTY;
                    }
                }

                if (chessBoard[si][sj] == BLACK_PAWN) {
                    if (di == 5 && dj == yEnPassant) {
                        chessBoard[xEnPassant][yEnPassant] = EMPTY;
                    }
                }
            }
        }

        // Efectuare mutare
        chessBoard[di][dj] = chessBoard[si][sj];
        chessBoard[si][sj] = EMPTY;
    } else if (strlen(movement) == 6) {
        // Efecuteaza promovari
        if (movement[4] == 'q') {
            chessBoard[di][dj] = WHITE_QUEEN + (chessBoard[si][sj] & 1);
        }

        if (movement[4] == 'r') {
            chessBoard[di][dj] = WHITE_ROOK + (chessBoard[si][sj] & 1);
        }

        if (movement[4] == 'b') {
            chessBoard[di][dj] = WHITE_BISHOP + (chessBoard[si][sj] & 1);
        }

        if (movement[4] == 'n') {
            chessBoard[di][dj] = WHITE_KNIGHT + (chessBoard[si][sj] & 1);
        }

        chessBoard[si][sj] = EMPTY;
    }
}


void Board::printBoard() { 
    char endline[2] = "\n";
    char ruler[25] = "    A B C D E F G H\n";
    char delim[25] = "    ----------------\n";
    print_cmd(ruler);
    print_cmd(delim);
    for (int i = 0; i < 8; i++) {
        char tmp[10];
        sprintf(tmp, "%d | ", 8-i);
        print_cmd(tmp);
        for (int j = 0; j < 8; j++) {
            switch (chessBoard[i][j]) {
                case 2:
                    sprintf(tmp, "P ");
                    print_cmd(tmp);
                    break;

                case 3:
                    sprintf(tmp, "p ");
                    print_cmd(tmp);
                    break;

                case 4:
                    sprintf(tmp, "R ");
                    print_cmd(tmp);
                    break;

                case 5:
                    sprintf(tmp, "r ");
                    print_cmd(tmp);
                    break;

                case 6:
                    sprintf(tmp, "N ");
                    print_cmd(tmp);
                    break;

                case 7:
                    sprintf(tmp, "n ");
                    print_cmd(tmp);
                    break;

                case 8:
                    sprintf(tmp, "B ");
                    print_cmd(tmp);
                    break;

                case 9:
                    sprintf(tmp, "b ");
                    print_cmd(tmp);
                    break;

                case 10:
                    sprintf(tmp, "Q ");
                    print_cmd(tmp);
                    break;

                case 11:
                    sprintf(tmp, "q ");
                    print_cmd(tmp);
                    break;

                case 12:
                    sprintf(tmp, "K ");
                    print_cmd(tmp);
                    break;

                case 13:
                    sprintf(tmp, "k ");
                    print_cmd(tmp);
                    break;

                default:
                    sprintf(tmp, "* ");
                    print_cmd(tmp);
                    break;
            }
        }

        print_cmd(endline);
    }
}

/**
 * Genereaza mutarile pionului de pe pozitia (x, y), cu x, y coordonate in 
 * reprezentarea interna.
 */
std::vector<string> Board::getPawnMoves(int x, int y) {
    std::vector<string> moves;
    
    // Mutari pion alb
    if (chessBoard[x][y] == WHITE_PAWN) {
        // Mutare 2 pozitii la inceput
        if (x == 6) {
            if (chessBoard[x - 2][y] == 0 && chessBoard[x - 1][y]) {
                moves.push_back(coords_to_string(x, y, x - 2 , y, 0));
            }
        }

        // Mutare o pozitie in fata
        if (chessBoard[x - 1][y] == 0) {
            // Verifica daca s-a ajuns la capatul tablei
            if (x == 1) {
                moves.push_back(coords_to_string(x, y, x - 1 , y, WHITE_QUEEN));
                moves.push_back(coords_to_string(x, y, x - 1 , y, WHITE_ROOK));
                moves.push_back(coords_to_string(x, y, x - 1 , y, WHITE_KNIGHT));
                moves.push_back(coords_to_string(x, y, x - 1 , y, WHITE_BISHOP));
            } else {
                moves.push_back(coords_to_string(x, y, x - 1 , y, 0));
            }
        }

        // Mutare o pozitie in fata-stanga
        if (y != 0) {
            if (chessBoard[x - 1][y - 1] && chessBoard[x - 1][y - 1] % 2 == 1) {
                if (x - 1 == 0) {
                    moves.push_back(coords_to_string(x, y, x - 1 , y - 1, WHITE_QUEEN));
                    moves.push_back(coords_to_string(x, y, x - 1 , y - 1, WHITE_ROOK));
                    moves.push_back(coords_to_string(x, y, x - 1 , y - 1, WHITE_KNIGHT));
                    moves.push_back(coords_to_string(x, y, x - 1 , y - 1, WHITE_BISHOP));
                } else {
                    moves.push_back(coords_to_string(x, y, x - 1 , y - 1, 0));
                }
            }
        }

        // Mutare o pozitie in fata-dreapta
        if (y != 7) {
            if (chessBoard[x - 1][y + 1] && chessBoard[x - 1][y + 1] % 2 == 1) {
                if (x - 1 == 0) {
                    moves.push_back(coords_to_string(x, y, x - 1 , y + 1, WHITE_QUEEN));
                    moves.push_back(coords_to_string(x, y, x - 1 , y + 1, WHITE_ROOK));
                    moves.push_back(coords_to_string(x, y, x - 1 , y + 1, WHITE_KNIGHT));
                    moves.push_back(coords_to_string(x, y, x - 1 , y + 1, WHITE_BISHOP));
                } else {
                    moves.push_back(coords_to_string(x, y, x - 1 , y + 1, 0));
                }
            }
        }

        // Mutari enPassant - alb
        if (x == 3) {
            // In stanga
            if (enPassantPossible && xEnPassant == 3 && y > 0 && y - 1 == yEnPassant) {
                moves.push_back(coords_to_string(x, y, x - 1, y - 1, 0));
            }

            // In dreapta
            if (enPassantPossible && xEnPassant == 3 && y < 7 && y + 1 == yEnPassant) {
                moves.push_back(coords_to_string(x, y, x - 1, y + 1, 0));
            }
        }
    }

    // Mutari pion negru
    if (chessBoard[x][y] == BLACK_PAWN) {
        // Mutare 2 pozitii la inceput
        if (x == 1) {
            if (chessBoard[x + 2][y] == 0 && chessBoard[x + 1][y] == 0) {
                moves.push_back(coords_to_string(x, y, x + 2 , y, 0));
            }
        }

        // Mutare o pozitie in fata
        if (chessBoard[x + 1][y] == 0) {
            // Verifica daca s-a ajuns la capatul tablei
            if (x == 6) {
                moves.push_back(coords_to_string(x, y, x + 1 , y, BLACK_QUEEN));
                moves.push_back(coords_to_string(x, y, x + 1 , y, BLACK_ROOK));
                moves.push_back(coords_to_string(x, y, x + 1 , y, BLACK_KNIGHT));
                moves.push_back(coords_to_string(x, y, x + 1 , y, BLACK_BISHOP));
            } else {
                moves.push_back(coords_to_string(x, y, x + 1 , y, 0));
            }
        }

        // Mutare o pozitie in fata-dreapta
        if (y != 0) {
            if (chessBoard[x + 1][y - 1] && chessBoard[x + 1][y - 1] % 2 == 0) {
                if (x + 1 == 7) {
                    moves.push_back(coords_to_string(x, y, x + 1 , y - 1, BLACK_QUEEN));
                    moves.push_back(coords_to_string(x, y, x + 1 , y - 1, BLACK_ROOK));
                    moves.push_back(coords_to_string(x, y, x + 1 , y - 1, BLACK_KNIGHT));
                    moves.push_back(coords_to_string(x, y, x + 1 , y - 1, BLACK_BISHOP));
                } else {
                    moves.push_back(coords_to_string(x, y, x + 1 , y - 1, 0));
                }
            }
        }

        // Mutare o pozitie in fata-stanga
        if (y != 7) {
            if (chessBoard[x + 1][y + 1] && chessBoard[x + 1][y + 1] % 2 == 0) {
                if (x + 1 == 7) {
                    moves.push_back(coords_to_string(x, y, x + 1 , y + 1, BLACK_QUEEN));
                    moves.push_back(coords_to_string(x, y, x + 1 , y + 1, BLACK_ROOK));
                    moves.push_back(coords_to_string(x, y, x + 1 , y + 1, BLACK_KNIGHT));
                    moves.push_back(coords_to_string(x, y, x + 1 , y + 1, BLACK_BISHOP));
                } else {
                    moves.push_back(coords_to_string(x, y, x + 1 , y + 1, 0));
                }
            }
        }

        // Mutari enPassant - negru
        if (x == 4) {
            // In stanga
            if (enPassantPossible && xEnPassant == 4 && y > 0 && y - 1 == yEnPassant) {
                moves.push_back(coords_to_string(x, y, x + 1, y - 1, 0));
            }

            // In dreapta
            if (enPassantPossible && xEnPassant == 4 && y < 7 && y + 1 == yEnPassant) {
                moves.push_back(coords_to_string(x, y, x + 1, y + 1, 0));
            }
        }
    }

    return moves;
}

/**
 * Genereaza toate mutarile posibile pentru toti pionii de engineColor
 * de pe tabla curenta.
 */
std::vector<string> Board::getAllPawnMoves(int engineColor) {
    std::vector<string> moves;

    for (int i = 1; i < 7; i++) {
        for (int j = 0; j < 8; j++) {
            if (chessBoard[i][j] == WHITE_PAWN + engineColor) {
                std::vector<string> tmpMoves = getPawnMoves(i, j);
                moves.insert(moves.end(), tmpMoves.begin(), tmpMoves.end());
            }
        }
    }

    return moves;
}

/**
 * Genereaza mutarile calului de pe pozitia (x, y), cu x, y coordonate in 
 * reprezentarea interna.
 */
std::vector<string> Board::getKnightMoves(int x, int y) {
    std::vector<string> moves;

    // Mutari cal alb
    if (chessBoard[x][y] == WHITE_KNIGHT) {
        if (x >= 2 && y >= 1 && (chessBoard[x-2][y-1] == 0 || chessBoard[x-2][y-1] % 2 == 1)) {
            moves.push_back(coords_to_string(x, y, x - 2, y - 1, 0));
        }

        if (x >= 2 && y < 7 && (chessBoard[x-2][y+1] == 0 || chessBoard[x-2][y+1] % 2 == 1)) {
            moves.push_back(coords_to_string(x, y, x - 2, y + 1, 0));
        }

        if (x >= 1 && y >= 2 && (chessBoard[x-1][y-2] == 0 || chessBoard[x-1][y-2] % 2 == 1)) {
            moves.push_back(coords_to_string(x, y, x - 1, y - 2, 0));
        }

        if (x >= 1 && y < 6 && (chessBoard[x-1][y+2] == 0 || chessBoard[x-1][y+2] % 2 == 1)) {
            moves.push_back(coords_to_string(x, y, x - 1, y + 2, 0));
        }


        if (x < 7 && y >= 2 && (chessBoard[x+1][y-2] == 0 || chessBoard[x+1][y-2] % 2 == 1)) {
            moves.push_back(coords_to_string(x, y, x + 1, y - 2, 0));
        }

        if (x < 7 && y < 6 && (chessBoard[x+1][y+2] == 0 || chessBoard[x+1][y+2] % 2 == 1)) {
            moves.push_back(coords_to_string(x, y, x + 1, y + 2, 0));
        }

        if (x < 6 && y >= 1 && (chessBoard[x+2][y-1] == 0 || chessBoard[x+2][y-1] % 2 == 1)) {
            moves.push_back(coords_to_string(x, y, x + 2, y - 1, 0));
        }

        if (x < 6 && y < 7 && (chessBoard[x+2][y+1] == 0 || chessBoard[x+2][y+1] % 2 == 1)) {
            moves.push_back(coords_to_string(x, y, x + 2, y + 1, 0));
        }
    }

    // Mutari cal negru
    if (chessBoard[x][y] == BLACK_KNIGHT) {
        if (x >= 2 && y >= 1 && (chessBoard[x-2][y-1] == 0 || ((chessBoard[x-2][y-1] % 2 == 0) && (chessBoard[x-2][y-1] != 0)))) {
            moves.push_back(coords_to_string(x, y, x - 2, y - 1, 0));
        }

        if (x >= 2 && y < 7 && (chessBoard[x-2][y+1] == 0 || ((chessBoard[x-2][y+1] % 2 == 0) && (chessBoard[x-2][y+1] != 0)))) {
            moves.push_back(coords_to_string(x, y, x - 2, y + 1, 0));
        }

        if (x >= 1 && y >= 2 && (chessBoard[x-1][y-2] == 0 || ((chessBoard[x-1][y-2] % 2 == 0) && (chessBoard[x-1][y-2] != 0)))) {
            moves.push_back(coords_to_string(x, y, x - 1, y - 2, 0));
        }

        if (x >= 1 && y < 6 && (chessBoard[x-1][y+2] == 0 || ((chessBoard[x-1][y+2] % 2 == 0) && (chessBoard[x-1][y+2] != 0)))) {
            moves.push_back(coords_to_string(x, y, x - 1, y + 2, 0));
        }


        if (x < 7 && y >= 2 && (chessBoard[x+1][y-2] == 0 || ((chessBoard[x+1][y-2] % 2 == 0) && (chessBoard[x+1][y-2] != 0)))) {
            moves.push_back(coords_to_string(x, y, x + 1, y - 2, 0));
        }

        if (x < 7 && y < 6 && (chessBoard[x+1][y+2] == 0 || ((chessBoard[x+1][y+2] % 2 == 0) && (chessBoard[x+1][y+2] != 0)))) {
            moves.push_back(coords_to_string(x, y, x + 1, y + 2, 0));
        }

        if (x < 6 && y >= 1 && (chessBoard[x+2][y-1] == 0 || ((chessBoard[x+2][y-1] % 2 == 0) && (chessBoard[x+2][y-1] != 0)))) {
            moves.push_back(coords_to_string(x, y, x + 2, y - 1, 0));
        }

        if (x < 6 && y < 7 && (chessBoard[x+2][y+1] == 0 || ((chessBoard[x+2][y+1] % 2 == 0) && (chessBoard[x+2][y+1] != 0)))) {
            moves.push_back(coords_to_string(x, y, x + 2, y + 1, 0));
        }
    }

    return moves;

}

/**
 * Genereaza toate mutarile posibile pentru toti caii de engineColor
 * de pe tabla curenta.
 */
std::vector<string> Board::getAllKnightMoves(int engineColor) {
    std::vector<string> moves;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (chessBoard[i][j] == WHITE_KNIGHT + engineColor) {
                std::vector<string> tmpMoves = getKnightMoves(i, j);
                moves.insert(moves.end(), tmpMoves.begin(), tmpMoves.end());
            }
        }
    }

    return moves;
}

/**
 * Genereaza mutarile pe care le poate face o tura de pe pozitia
 * (x, y), cu x, y coordonate in reprezentarea interna.
 * Este de datoria utilizatorului de a verifica daca aceasta functie
 * este apelata pe un turn sau o alta piesa(explicatii in
 * getAllQueenMoves).
 */
std::vector<std::string> Board::getRookMoves(int x, int y) {
    std::vector<std::string> moves;

    if (chessBoard[x][y] % 2 == 0) {
        // Avem de mutat tura alba

        // Caut mutari pe coloana in jos
        if (x < 7) {
            for (int i = x + 1; i < 8; ++i) {
                if (chessBoard[i][y] != 0) {
                    if (chessBoard[i][y] % 2 == 1) {
                        moves.push_back(coords_to_string(x, y, i, y, 0));
                        break;
                    } else {
                        break;
                    }
                } else {
                    moves.push_back(coords_to_string(x, y, i, y, 0));
                }
            }
        }
        

        // Caut mutari pe coloana in sus
        if (x > 0) {
            for (int i = x - 1; i >= 0; --i) {
                if (chessBoard[i][y] != 0) {
                    if (chessBoard[i][y] % 2 == 1) {
                        moves.push_back(coords_to_string(x, y, i, y, 0));
                        break;
                    } else {
                        break;
                    }
                } else {
                    moves.push_back(coords_to_string(x, y, i, y, 0));
                }
            }
        }

        // Caut mutari pe linie in dreapta
        if (y < 7) {
            for (int i = y + 1; i < 8; ++i) {
                if (chessBoard[x][i] != 0) {
                    if (chessBoard[x][i] % 2 == 1) {
                        moves.push_back(coords_to_string(x, y, x, i, 0));
                        break;
                    } else {
                        break;
                    }
                } else {
                    moves.push_back(coords_to_string(x, y, x, i, 0));
                }
            }
        }

        // Caut mutari pe linie in stanga
        if (y > 0) {
            for (int i = y - 1; i >= 0; --i) {
                if (chessBoard[x][i] != 0) {
                    if (chessBoard[x][i] % 2 == 1) {
                        moves.push_back(coords_to_string(x, y, x, i, 0));
                        break;
                    } else {
                        break;
                    }
                } else {
                    moves.push_back(coords_to_string(x, y, x, i, 0));
                }
            }
        }
    } else {
        // Avem de mutat tura neagra

        // Caut mutari pe coloana in jos
        if (x < 7) {
            for (int i = x + 1; i < 8; ++i) {
                if (chessBoard[i][y] != 0) {
                    if (chessBoard[i][y] % 2 == 0) {
                        moves.push_back(coords_to_string(x, y, i, y, 0));
                        break;
                    } else {
                        break;
                    }
                } else {
                    moves.push_back(coords_to_string(x, y, i, y, 0));
                }
            }
        }
        

        // Caut mutari pe coloana in sus
        if (x > 0) {
            for (int i = x - 1; i >= 0; --i) {
                if (chessBoard[i][y] != 0) {
                    if (chessBoard[i][y] % 2 == 0) {
                        moves.push_back(coords_to_string(x, y, i, y, 0));
                        break;
                    } else {
                        break;
                    }
                } else {
                    moves.push_back(coords_to_string(x, y, i, y, 0));
                }
            }
        }

        // Caut mutari pe linie in dreapta
        if (y < 7) {
            for (int i = y + 1; i < 8; ++i) {
                if (chessBoard[x][i] != 0) {
                    if (chessBoard[x][i] % 2 == 0) {
                        moves.push_back(coords_to_string(x, y, x, i, 0));
                        break;
                    } else {
                        break;
                    }
                } else {
                    moves.push_back(coords_to_string(x, y, x, i, 0));
                }
            }
        }

        // Caut mutari pe linie in stanga
        if (y > 0) {
            for (int i = y - 1; i >= 0; --i) {
                if (chessBoard[x][i] != 0) {
                    if (chessBoard[x][i] % 2 == 0) {
                        moves.push_back(coords_to_string(x, y, x, i, 0));
                        break;
                    } else {
                        break;
                    }
                } else {
                    moves.push_back(coords_to_string(x, y, x, i, 0));
                }
            }
        }
    }

    return moves;
}

/**
 * Genereaza toate mutarile posibile pentru toate turnurile de engineColor
 * de pe tabla curenta.
 */
std::vector<string> Board::getAllRookMoves(int engineColor) {
    std::vector<string> moves;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (chessBoard[i][j] == WHITE_ROOK + engineColor) {
                std::vector<string> tmpMoves = getRookMoves(i, j);
                moves.insert(moves.end(), tmpMoves.begin(), tmpMoves.end());
            }
        }
    }

    return moves;
}

/**
 * Genereaza mutarile pe care le poate face un nebun de pe pozitia
 * (x, y), cu x, y coordonate in reprezentarea interna.
 * Este de datoria utilizatorului de a verifica daca aceasta functie
 * este apelata pe un nebun sau o alta piesa(explicatii in
 * getAllQueenMoves).
 */
std::vector<std::string> Board::getBishopMoves(int x, int y) {
    std::vector<std::string> moves;
    int j;

    if (chessBoard[x][y] % 2 == 0) {
        // Avem nebun alb

        // Caut mutari jos stanga
        if (x < 7 && y > 0) {
            j = 0;
            for (int i = x + 1; i < 8; ++i) {
                ++j;

                if (y - j < 0) {
                    break;
                }
                
                if (chessBoard[i][y - j] != 0) {
                    if (chessBoard[i][y - j] % 2 == 1) {
                        moves.push_back(coords_to_string(x, y, i, y - j, 0));
                        break;
                    } else {
                        break;
                    }
                } else {
                    moves.push_back(coords_to_string(x, y, i, y - j, 0));
                }

            }
        }

        // Caut mutari jos dreapta
        if (x < 7 && y < 7) {
            j = 0;
            for (int i = x + 1; i < 8; ++i) {
                ++j;

                if (y + j > 7) {
                    break;
                }

                if (chessBoard[i][y + j] != 0) {
                    if (chessBoard[i][y + j] % 2 == 1) {
                        moves.push_back(coords_to_string(x, y, i, y + j, 0));
                        break;
                    } else {
                        break;
                    }
                } else {
                    moves.push_back(coords_to_string(x, y, i, y + j, 0));
                }
            }
        }

        // Caut mutari sus stanga
        if (x > 0 && y > 0) {
            j = 0;
            for (int i = x - 1; i >= 0; --i) {
                ++j;

                if (y - j < 0) {
                    break;
                }

                if (chessBoard[i][y - j] != 0) {
                    if (chessBoard[i][y - j] % 2 == 1) {
                        moves.push_back(coords_to_string(x, y, i, y - j, 0));
                        break;
                    } else {
                        break;
                    }
                } else {
                    moves.push_back(coords_to_string(x, y, i, y - j, 0));
                }
            }
        }

        // Caut mutari sus dreapta
        if (x > 0 && y < 7) {
            j = 0;
            for (int i = x - 1; i >= 0; --i) {
                ++j;
                
                if (y + j > 7) {
                    break;
                }

                if (chessBoard[i][y + j] != 0) {
                    if (chessBoard[i][y + j] % 2 == 1) {
                        moves.push_back(coords_to_string(x, y, i, y + j, 0));
                        break;
                    } else {
                        break;
                    }
                } else {
                    moves.push_back(coords_to_string(x, y, i, y + j, 0));
                }
            }
        }
    } else {
        // Avem nebun negru

        // Caut mutari jos stanga
        if (x < 7 && y > 0) {
            j = 0;
            for (int i = x + 1; i < 8; ++i) {
                ++j;

                if (y - j < 0) {
                    break;
                }

                if (chessBoard[i][y - j] != 0) {
                    if (chessBoard[i][y - j] % 2 == 0) {
                        moves.push_back(coords_to_string(x, y, i, y - j, 0));
                        break;
                    } else {
                        break;
                    }
                } else {
                    moves.push_back(coords_to_string(x, y, i, y - j, 0));
                }

            }
        }

        // Caut mutari jos dreapta
        if (x < 7 && y < 7) {
            j = 0;
            for (int i = x + 1; i < 8; ++i) {
                ++j;

                if (y + j > 7) {
                    break;
                }

                if (chessBoard[i][y + j] != 0) {
                    if (chessBoard[i][y + j] % 2 == 0) {
                        moves.push_back(coords_to_string(x, y, i, y + j, 0));
                        break;
                    } else {
                        break;
                    }
                } else {
                    moves.push_back(coords_to_string(x, y, i, y + j, 0));
                }
            }
        }

        // Caut mutari sus stanga
        if (x > 0 && y > 0) {
            j = 0;
            for (int i = x - 1; i >= 0; --i) {
                ++j;

                if (y - j < 0) {
                    break;
                }

                if (chessBoard[i][y - j] != 0) {
                    if (chessBoard[i][y - j] % 2 == 0) {
                        moves.push_back(coords_to_string(x, y, i, y - j, 0));
                        break;
                    } else {
                        break;
                    }
                } else {
                    moves.push_back(coords_to_string(x, y, i, y - j, 0));
                }
            }
        }

        // Caut mutari sus dreapta
        if (x > 0 && y < 7) {
            j = 0;
            for (int i = x - 1; i >= 0; --i) {
                ++j;

                if (y + j > 7) {
                    break;
                }

                if (chessBoard[i][y + j] != 0) {
                    if (chessBoard[i][y + j] % 2 == 0) {
                        moves.push_back(coords_to_string(x, y, i, y + j, 0));
                        break;
                    } else {
                        break;
                    }
                } else {
                    moves.push_back(coords_to_string(x, y, i, y + j, 0));
                }
            }
        }
    }

    return moves;
}

/**
 * Genereaza toate mutarile posibile pentru toti nebunii de engineColor
 * de pe tabla curenta.
 */
std::vector<string> Board::getAllBishopMoves(int engineColor) {
    std::vector<string> moves;

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (chessBoard[i][j] == WHITE_BISHOP + engineColor) {
                std::vector<string> tmpMoves = getBishopMoves(i, j);
                moves.insert(moves.end(), tmpMoves.begin(), tmpMoves.end());
            }
        }
    }
    return moves;
}

/**
 * Deoarece regina se poate deplasa pe linie/coloana, oricate pozitii,
 * precum turnurile, si pe diagonale, oricate pozitii, precum nebunii,
 * ne vom folosi de functiile deja existente pentru generarea deplasarii
 * reginei in comportamentul nebunilor/al turnurilor.
 */
std::vector<string> Board::getAllQueenMoves(int engineColor) {
    std::vector<string> moves;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (chessBoard[i][j] == WHITE_QUEEN + engineColor) {
                std::vector<string> rookMoves = getRookMoves(i, j);
                moves.insert(moves.end(), rookMoves.begin(), rookMoves.end());
                std::vector<string> bishopMoves = getBishopMoves(i, j);
                moves.insert(moves.end(), bishopMoves.begin(), bishopMoves.end());
            }
        }
    }

    return moves;
}

/**
 * Genereaza toate mutarile posibile pentru regele de culoare engineColor.
 */
std::vector<string> Board::getAllKingMoves(int engineColor) {
    std::vector<string> moves;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (chessBoard[i][j] == WHITE_KING + engineColor) {
                // mutare in fata 
                // (daca nu se termina tabla si (daca casuta din fata este goala sau (daca casuta nu este goala si e ocupata de adversar)))
                if (i > 0 && (chessBoard[i-1][j] == 0 || (chessBoard[i-1][j] != 0 && chessBoard[i-1][j] % 2 != engineColor))) {
                    moves.push_back(coords_to_string(i , j, i - 1, j, 0));
                }

                // mutare in spate 
                // (daca nu se termina tabla si (daca casuta din spate este goala sau (daca casuta nu este goala si e ocupata de adversar)))
                if (i < 7 && (chessBoard[i+1][j] == 0 || (chessBoard[i+1][j] != 0 && chessBoard[i+1][j] % 2 != engineColor))) {
                    moves.push_back(coords_to_string(i , j, i + 1, j, 0));
                }

                // mutare in stanga 
                // (daca nu se termina tabla si (daca casuta din stanga este goala sau (daca casuta nu este goala si e ocupata de adversar)))
                if (j > 0 && (chessBoard[i][j-1] == 0 || (chessBoard[i][j-1] != 0 && chessBoard[i][j-1] % 2 != engineColor))) {
                    moves.push_back(coords_to_string(i , j, i, j - 1, 0));
                }

                // mutare in dreapta
                // (daca nu se termina tabla si (daca casuta din dreapta este goala sau (daca casuta nu este goala si e ocupata de adversar)))
                if (j < 7 && (chessBoard[i][j+1] == 0 || (chessBoard[i][j+1] != 0 && chessBoard[i][j+1] % 2 != engineColor))) {
                    moves.push_back(coords_to_string(i , j, i, j + 1, 0));
                }

                // mutare in stanga fata
                // (daca nu se termina tabla si (daca casuta din stanga-fata este goala sau (daca casuta nu este goala si e ocupata de adversar)))
                if ((i > 0 && j > 0) && (chessBoard[i-1][j-1] == 0 || (chessBoard[i-1][j-1] != 0 && chessBoard[i-1][j-1] % 2 != engineColor))) {
                    moves.push_back(coords_to_string(i , j, i - 1, j - 1, 0));
                }

                // mutare in dreapta fata
                // (daca nu se termina tabla si (daca casuta din dreapta-fata este goala sau (daca casuta nu este goala si e ocupata de adversar)))
                if ((i > 0 && j < 7) && (chessBoard[i-1][j+1] == 0 || (chessBoard[i-1][j+1] != 0 && chessBoard[i-1][j+1] % 2 != engineColor))) {
                    moves.push_back(coords_to_string(i , j, i - 1, j + 1, 0));
                }

                // mutare in stanga spate
                // (daca nu se termina tabla si (daca casuta din stanga-spate este goala sau (daca casuta nu este goala si e ocupata de adversar)))
                if ((i < 7 && j > 0) && (chessBoard[i+1][j-1] == 0 || (chessBoard[i+1][j-1] != 0 && chessBoard[i+1][j-1] % 2 != engineColor))) {
                    moves.push_back(coords_to_string(i , j, i + 1, j - 1, 0));
                }

                // mutare in dreapta spate
                // (daca nu se termina tabla si (daca casuta din dreapta-spate este goala sau (daca casuta nu este goala si e ocupata de adversar)))
                if ((i < 7 && j < 7) && (chessBoard[i+1][j+1] == 0 || (chessBoard[i+1][j+1] != 0 && chessBoard[i+1][j+1] % 2 != engineColor))) {
                    moves.push_back(coords_to_string(i , j, i + 1, j + 1, 0));
                }
            }
        }
    }

    return moves;
}

/**
 * Verifica daca regele de culoare engineColor este expus la vreuna
 * dintre piesele adversarului.
 */
bool Board::isCheck(int engineColor) {
    int xKing, yKing;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (chessBoard[i][j] == WHITE_KING + engineColor) {
                xKing = i;
                yKing = j;
                break;
            }
        }
    }

    // verificare daca regele are pioni pe diagonale
    if (engineColor == 0) {
        // rege alb
        // pion negru pe stanga-fata
        if (xKing > 0 && yKing < 0 && chessBoard[xKing - 1][yKing - 1] == BLACK_PAWN) {
            return true;
        }

        // pion negru pe dreapta-fata
        if (xKing > 0 && yKing < 7 && chessBoard[xKing - 1][yKing + 1] == BLACK_PAWN) {
            return true;
        }
    } else {
        // rege negru
        // pion alb pe stanga-jos
        if (xKing < 7 && yKing < 0 && chessBoard[xKing + 1][yKing - 1] == WHITE_PAWN) {
            return true;
        }

        // pion alb pe dreapta-jos
        if (xKing < 7 && yKing < 7 && chessBoard[xKing + 1][yKing + 1] == WHITE_PAWN) {
            return true;
        }
    }




    // verificare daca regele are cai ai adversarului pe casutele adiacente
    // de verificat, pozitiile: 
    // (xKing-2; yking-1)    (xKing-2; yking+1)
    // (xKing-1; yking-2)    (xKing-1; yking+2)
    // (xKing+1; yking-2)    (xKing+1; yking+2)
    // (xKing+2; yking-1)    (xKing+2; yking+1)
    if (xKing - 2 >= 0 && yKing - 1 >= 0 && chessBoard[xKing-2][yKing-1] == WHITE_KNIGHT + (1 - engineColor)) {
        return true;
    }

    if (xKing - 2 >= 0 && yKing + 1 <= 7 && chessBoard[xKing-2][yKing+1] == WHITE_KNIGHT + (1 - engineColor)) {
        return true;
    }

    if (xKing - 1 >= 0 && yKing - 2 >= 0 && chessBoard[xKing-1][yKing-2] == WHITE_KNIGHT + (1 - engineColor)) {
        return true;
    }

    if (xKing - 1 >= 0 && yKing + 2 <= 7 && chessBoard[xKing-1][yKing+2] == WHITE_KNIGHT + (1 - engineColor)) {
        return true;
    }


    if (xKing + 1 <= 7 && yKing - 2 >= 0 && chessBoard[xKing+1][yKing-2] == WHITE_KNIGHT + (1 - engineColor)) {
        return true;
    }

    if (xKing + 1 <= 7 && yKing + 2 <= 7 && chessBoard[xKing+1][yKing+2] == WHITE_KNIGHT + (1 - engineColor)) {
        return true;
    }

    if (xKing + 2 <= 7 && yKing - 1 >= 0 && chessBoard[xKing+2][yKing-1] == WHITE_KNIGHT + (1 - engineColor)) {
        return true;
    }

    if (xKing + 2 <= 7 && yKing + 1 <= 7 && chessBoard[xKing+2][yKing+1] == WHITE_KNIGHT + (1 - engineColor)) {
        return true;
    }



    // Verificam daca avem regele expus pe diagonale de catre un nebun sau regina adversarului
    // Stanga jos
    for (int i = xKing + 1, j = yKing - 1; i <= 7 && j >= 0; i++, j--) {
        if (chessBoard[i][j] == WHITE_BISHOP + (1 - engineColor)) {
            return true;
        } else if (chessBoard[i][j] == WHITE_QUEEN + (1 - engineColor)) {
            return true;
        } else if (chessBoard[i][j] != 0) {
            break;
        }
    }

    // Dreapta jos
    for (int i = xKing + 1, j = yKing + 1; i <= 7 && j <= 7; i++, j++) {
        if (chessBoard[i][j] == WHITE_BISHOP + (1 - engineColor)) {
            return true;
        } else if (chessBoard[i][j] == WHITE_QUEEN + (1 - engineColor)) {
            return true;
        } else if (chessBoard[i][j] != 0) {
            break;
        }
    }

    // Stanga sus
    for (int i = xKing - 1, j = yKing - 1; i >= 0 && j >= 0; i--, j--) {
        if (chessBoard[i][j] == WHITE_BISHOP + (1 - engineColor)) {
            return true;
        } else if (chessBoard[i][j] == WHITE_QUEEN + (1 - engineColor)) {
            return true;
        } else if (chessBoard[i][j] != 0) {
            break;
        }
    }

    // Dreapta sus
    for (int i = xKing - 1, j = yKing + 1; i >= 0 && j <= 7; i--, j++) {
        if (chessBoard[i][j] == WHITE_BISHOP + (1 - engineColor)) {
            return true;
        } else if (chessBoard[i][j] == WHITE_QUEEN + (1 - engineColor)) {
            return true;
        } else if (chessBoard[i][j] != 0) {
            break;
        }
    }



    // Verificam daca avem regele expus pe linie/coloana de catre un turn sau regina adversarului
    // Verific linia in stanga (tura + regina)
    if (yKing > 0) {
        for (int j = yKing - 1; j >= 0; --j) {
            // Daca este spatiu gol merg mai departe
            if (chessBoard[xKing][j] == 0) {
                continue;
            } else {
                // Verific daca are o piesa din echipa
                if (chessBoard[xKing][j] % 2 == engineColor) {
                        break;
                }

                // Verific daca are o tura sau o regina adversa
                if ((chessBoard[xKing][j] == WHITE_ROOK + (1 - engineColor)) ||
                    (chessBoard[xKing][j] == WHITE_QUEEN + (1 - engineColor))) {
                        return true;
                    } else {
                        // Inseamna ca are o alta piesa adversa
                        break;;
                    }
            }
        }
    }

    // Verific linia in dreapta (tura + regina)
    if (yKing < 7) {
        for (int j = yKing + 1; j < 8; ++j) {
            // Daca este spatiu gol merg mai departe
            if (chessBoard[xKing][j] == 0) {
                continue;
            } else {
                // Verific daca are o piesa din echipa
                if (chessBoard[xKing][j] % 2 == engineColor) {
                    break;
                }

                // Verific daca are o tura sau o regina adversa
                if ((chessBoard[xKing][j] == WHITE_ROOK + (1 - engineColor)) ||
                    (chessBoard[xKing][j] == WHITE_QUEEN + (1 - engineColor))) {
                        return true;
                    } else {
                        // Inseamna ca are o alta piesa adversa
                        break;
                    }
            }
        }
    }

    // Verific coloana in sus (tura + regina)
    if (xKing > 0) {
        for (int i = xKing - 1; i >= 0; --i) {
            // Daca este spatiu gol merg mai departe
            if (chessBoard[i][yKing] == 0) {
                continue;
            } else {
                // Verific daca are o piesa din echipa
                if (chessBoard[i][yKing] % 2 == engineColor) {
                    break;
                }

                // Verific daca are o tura sau o regina adversa
                if ((chessBoard[i][yKing] == WHITE_ROOK + (1 - engineColor)) ||
                    (chessBoard[i][yKing] == WHITE_QUEEN + (1 - engineColor))) {
                        return true;
                    } else {
                        break;
                    }
            }
        }
    }

    // Verific coloana in jos (tura + regina)
    if (xKing < 7) {
        for (int i = xKing + 1; i < 8; ++i) {
            // Daca este spatiu gol merg mai departe
            if (chessBoard[i][yKing] == 0) {
                continue;
            } else {
                // Verific daca are o piesa din echipa
                if (chessBoard[i][yKing] % 2 == engineColor) {
                    break;
                }

                // Verific daca are o tura sau o regina adversa
                if ((chessBoard[i][yKing] == WHITE_ROOK + (1 - engineColor)) ||
                    (chessBoard[i][yKing] == WHITE_QUEEN + (1 - engineColor))) {
                        return true;
                    } else {
                        break;
                    }
            }
        }
    }

    return false;
}

/**
 * Verifica daca sunt doar casute libere intre pozitiile x si y de pe linia line,
 * cu line, x si y in coordonate de reprezentare interna.
 */
bool Board::checkIfEmptyBetween(int line, int x, int y) {
    for (int i = x + 1; i < y; i++) {
        if (chessBoard[line][i] != EMPTY) {
            return false;
        }
    }

    return true;
}
