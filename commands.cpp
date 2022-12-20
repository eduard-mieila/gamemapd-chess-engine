#include "engine_io.h"
#include "board.h"
#include <string.h>
#include <string>
#include <stdlib.h>
#include <iostream>

#define WHITE 0
#define BLACK 1

#define FORCE 0
#define GO 1


/*
 * Transmite la xboard (cu print_cmd(.)) comanda:
 * <    feature sigint=0 san=0 name=GAMEMAPD
 */
void feature_cmd() {
  char cmd[100];

  cmd[0] = '\0';
  strcat(cmd, "feature sigint=0 san=0 name=GAMEMAPD\n");

  print_cmd(cmd);
}

/*
 * Intra in starea "force mode" -> variabila de stare $engine_mode;
 * Mutarile se vor face doar de la xboard pentru ambele culori.
 * (forced_move_cmd(.) va prelua mutarile de la xboard de aici inainte, si le
 * va executa ca atare.
 */
void force_cmd(int *engine_mode) {
  *engine_mode = FORCE;
}

/*
 * Analizeaza structurile de date folosind algoritmii. Gaseste o mutare valida
 * si buna ( YYYY ) pentru bot(engine_color):
 *      - Salveaza in data_base miscarea facuta
 *      - Comunica xboard-ului mutarea facuta (print_cmd(move YYYY));
 *      - sau, daca nu a gasit o miscare bune -> se preda si comunica
 * xboard-ului acest lucru (print_cmd(resign));
 *
 */
void think_cmd(Board *myBoard, int engine_color) {
  std::string myMove;
  char cmd[20] = "move ";
  
  // Vectori in care vom tine minte toate mutarile posibile, valide sau nu.
  // allMoves va reuni toate mutarile posibile, mai putin rocadele.
  std::vector<std::string> allMoves;
  std::vector<std::string> pawnMoves = myBoard->getAllPawnMoves(engine_color);
  std::vector<std::string> knightMoves = myBoard->getAllKnightMoves(engine_color);
  std::vector<std::string> rookMoves = myBoard->getAllRookMoves(engine_color);
  std::vector<std::string> bishopMoves = myBoard->getAllBishopMoves(engine_color);
  std::vector<std::string> queenMoves = myBoard->getAllQueenMoves(engine_color);
  std::vector<std::string> kingMoves = myBoard->getAllKingMoves(engine_color);
  std::vector<std::string> castlingMoves;


  char tmp[10];
  // Generam rocadele, daca exista
  if (engine_color == 0) {
    if (!(myBoard->isCheck(engine_color)) && !(myBoard->whiteKingMoved)) {
      if (!(myBoard->whiteLeftRookMoved)) {
        if (myBoard->checkIfEmptyBetween(7, 0, 4)) {
          // Rocada mare alb - e1c1
          // Dacă regele trece prin sah sau ajunge in sah, nu se poate efectua rocada
          Board copyBoard1 = Board(*myBoard);
          sprintf(tmp, "e1d1\n");
          copyBoard1.move(tmp);
          Board copyBoard2 = Board(*myBoard);
          sprintf(tmp, "e1c1\n");
          copyBoard2.move(tmp);
          if (!copyBoard1.isCheck(engine_color) && !copyBoard2.isCheck(engine_color)) {
            sprintf(tmp, "e1c1\n");
            castlingMoves.push_back(tmp);
          }
        }
      }

      if (!(myBoard->whiteRightRookMoved)) {
        if (myBoard->checkIfEmptyBetween(7, 4, 7)) {
          // Rocada mica alb - e1g1
          // Dacă regele trece prin sah sau ajunge in sah, nu se poate efectua rocada
          Board copyBoard1 = Board(*myBoard);
          sprintf(tmp, "e1f1\n");
          copyBoard1.move(tmp);
          Board copyBoard2 = Board(*myBoard);
          sprintf(tmp, "e1g1\n");
          copyBoard2.move(tmp);
          if (!copyBoard1.isCheck(engine_color) && !copyBoard2.isCheck(engine_color)) {
            sprintf(tmp, "e1g1\n");
            castlingMoves.push_back(tmp);
          }
        }
      }
    }
  } else {
    if (!(myBoard->isCheck(engine_color)) && !(myBoard->blackKingMoved)) {
      if (!(myBoard->blackLeftRookMoved)) {
        if (myBoard->checkIfEmptyBetween(0, 0, 4)) {
          // Rocada mare negru - e8c8
          // Dacă regele trece prin sah sau ajunge in sah, nu se poate efectua rocada
          Board copyBoard1 = Board(*myBoard);
          sprintf(tmp, "e8d8\n");
          copyBoard1.move(tmp);
          Board copyBoard2 = Board(*myBoard);
          sprintf(tmp, "e8c8\n");
          copyBoard2.move(tmp);
          if (!copyBoard1.isCheck(engine_color) && !copyBoard2.isCheck(engine_color)) {
            sprintf(tmp, "e8c8\n");
            castlingMoves.push_back(tmp);
          }
        }
      }
    

      if (!(myBoard->blackRightRookMoved)) {
        if (myBoard->checkIfEmptyBetween(0, 4, 7)) {
          // rocada mica negru - e8g8
          // Dacă regele trece prin sah sau ajunge in sah, nu se poate efectua rocada
          Board copyBoard1 = Board(*myBoard);
          sprintf(tmp, "e8f8\n");
          copyBoard1.move(tmp);
          Board copyBoard2 = Board(*myBoard);
          sprintf(tmp, "e8g8\n");
          copyBoard1.move(tmp);
          if (!copyBoard1.isCheck(engine_color) && !copyBoard2.isCheck(engine_color)) {
            sprintf(tmp, "e8g8\n");
            castlingMoves.push_back(tmp);
          }
        }
      }
    }
  }
  
  // Reunim toate celelalte mutari posibile, valide sau nu
  allMoves.insert(allMoves.end(), pawnMoves.begin(), pawnMoves.end());
  allMoves.insert(allMoves.end(), knightMoves.begin(), knightMoves.end());
  allMoves.insert(allMoves.end(), rookMoves.begin(), rookMoves.end());
  allMoves.insert(allMoves.end(), bishopMoves.begin(), bishopMoves.end());
  allMoves.insert(allMoves.end(), queenMoves.begin(), queenMoves.end());
  allMoves.insert(allMoves.end(), kingMoves.begin(), kingMoves.end());

  if (DEBUG) {
    // Afisare mutari posibile in fisier debug xboard
    std::cout << "***************************************" << std::endl;
    std::cout << "******** Moves Possible ***************" << std::endl;
    std::cout << "******** Castling Moves ***************" << std::endl;
    for (int i = 0; i < (int)castlingMoves.size(); i++) {
      std::cout << castlingMoves[i];
    }

    std::cout << "******** Normal Moves *****************" << std::endl;
    
    for (int i = 0; i < (int)allMoves.size(); i++) {
      std::cout << allMoves[i];
    }
    std::cout << "***************************************" << std::endl;
  }

  
  char myMoveChar[7];
  if (castlingMoves.size() > 0){
    // Daca se poate face o rocada, mutam astfel.
    // Actualizam statusul pentru en passant.
    myBoard->enPassantPossible = false;
    myBoard->xEnPassant = -1;
    myBoard->yEnPassant = -1;

    memset(myMoveChar, 0, 7);
    memcpy(myMoveChar, castlingMoves[0].c_str(), strlen(castlingMoves[0].c_str()));
    myBoard->move(myMoveChar);
  } else {
    // Daca nu sunt rocade de facut, cautam prima mutare valida.
    bool legalMove = false;
    int size, pos;
    
    while (!legalMove) {
      size = allMoves.size();
      if (size == 0) {
        // Daca nu sunt mutari posibile => resign.
        if (myBoard->isCheck(engine_color)) {
          sprintf(tmp, "resign\n");
          print_cmd(tmp);
          return;
        }
      }
      
      // Generare numar aleator
      srand(time(NULL));
      pos = rand() % size;
      myMove = allMoves[pos];

      memset(myMoveChar, 0, 7);
      memcpy(myMoveChar, myMove.c_str(), strlen(myMove.c_str()));
      // Pentru a verifica daca mutarea este valida sau nu, fara sa
      // influentam jocul, cream o copie a tablei curente.
      // Verificam daca suntem in sah pe aceasta copie.
      Board copyBoard = Board(*myBoard);
      copyBoard.move(myMoveChar);

      if (copyBoard.isCheck(engine_color)) {
        // Daca mutarea nu este valida, o scoatem din vector.
        allMoves.erase(allMoves.begin() + pos);
        if (DEBUG) {
          char tmp[90];
          sprintf(tmp, "!!! illegal move: %s", myMoveChar);
          print_cmd(tmp);
        }
      } else {
        // Resetare enPassant.
        myBoard->enPassantPossible = false;
        myBoard->xEnPassant = -1;
        myBoard->yEnPassant = -1;

        // Efectuare mutare valida.
        myBoard->move(myMoveChar);
        legalMove = true;
      }
    }
  }

  // Trimitere mutare catre xboard.
  strcat(cmd, myMoveChar);
  print_cmd(cmd);
}

/*
 * Intra in starea "go mode". 
 * Mutarile lui $engine-color vor fi facute de engine.
 *  Daca s-a dat comanda asta -> este randul botului, deci:
 *          - think_cmd(engine_color);
 */
void go_cmd(Board *myBoard, int *engine_mode, int engine_color) {
  *engine_mode = GO;

  think_cmd(myBoard, engine_color);
}

/*
 * Set engine as white ($engine_color).
 */
void white_cmd(int *engine_color) {
  *engine_color = WHITE;
}

/*
 * Set engine as black ($engine_color).
 */
void black_cmd(int *engine_color) {
  *engine_color = BLACK;
}

/*
 * Initializeaza partida:
 *          - white is first (engine_color);
 *          - engine is black (black_cmd());
 *          - set GO mode;
 */
void new_cmd(int *engine_color, int *engine_mode) {

  black_cmd(engine_color);
  *engine_mode = GO;  // nu apela go() ca trebuie sa mute mai intai white
}
