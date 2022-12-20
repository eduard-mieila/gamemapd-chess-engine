#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "commands.h"
#include "board.h"

#define WHITE 0
#define BLACK 1

#define FORCE 0
#define GO 1

Board *myBoard;

// a, b in ['a', 'h'] ?
int cols(char a, char b) {
  if ('a' <= a && 'a' <= b && a <= 'h' && b <= 'h') {
    return 1;
  }

  return 0;
}

// a, b in ['1', '8'] ?
int rows(char a, char b) {
  if ('1' <= a && '1' <= b && a <= '8' && b <= '8') {
    return 1;
  }

  return 0;
}

// Verifica daca $cmd are format de miscare.
// De exemplu: cmd -> ..."b7b8q"
char *is_move(char *buff) {
  int len = strlen(buff);
  
  if (buff[len-1] == '\n' && 
      rows(buff[len-2], buff[len-4]) &&
      cols(buff[len-3], buff[len-5])) {
    return buff + len - 5;
  }

  if (buff[len-1] == '\n' &&
       buff[len-2] == 'q' &&
       rows(buff[len-3], buff[len-5]) &&
       cols(buff[len-4], buff[len-6])) {
    return buff + len - 6;
  }

  return NULL;
}

/*
 * Citeste orice intr-un ciclu infinit orice caracter ce apare la stdin.
 * Cand detecteaza o comanda cunoscuta (citeste si parametrul acesteia daca
 * este cazul) apeleaza o "functie comanda" ce executa acea comanda.
 * "Functia comanda" ce este apelata depinde si de variabilele de stare:
 *      $engine_mode si $engine_color.
 * Ciclul infinit este intrerupt prin break, la detectarea unei comenzi ce
 * indica acest lucru (ceea ce inseamna ca si procesul xboard-ului se termina).
 *
 * ----------------------------------------------------------------------------
 * input_cmd    ->    "functia comanda"
 * ----------------------------------------------------------------------------
 * xboard       ->    initial_cmd()
 * protover N   ->    feature_cmd()
 | * new          ->    new_cmd()
 * force        ->    force_cmd()
 * go           ->    go_cmd()
 * XXXX (if "go mode") -> move_update(XXXX) && think_cmd()
 * XXXX (if "force mode") ->  forced_move_cmd(XXXX)
 * white                  -> white_cmd()
 * black                  -> black_cmd()
 * quit                   -> break
 */
void cmd_interpreter() {
  // Variabilele de stare: 
  int engine_color = BLACK;
  int engine_mode = GO;
  ////////////////////////////

  char *cmd;
  char c;
  char *buff_start, *buff_end;
  

  buff_start = (char *)malloc(2048 * sizeof(*buff_start));
  buff_end = buff_start;


  while (read(STDIN_FILENO, &c, 1)) {
    *buff_end = c;
    buff_end++;
    *buff_end = '\0';

    if (strstr(buff_start, "xboard\n")) {
      myBoard = new Board();

      buff_end = buff_start;
      continue;
    }

    if (strstr(buff_start, "protover ")) {
      feature_cmd();
      buff_end = buff_start;
      continue;
    }

    if (strstr(buff_start, "new\n")) {
      delete myBoard;
      myBoard = new Board();
      new_cmd(&engine_color, &engine_mode);

      myBoard->printBoard();

      buff_end = buff_start;
      continue;
    }

    if (strstr(buff_start, "force\n")) {
      force_cmd(&engine_mode);
      buff_end = buff_start;
      continue;
    }

    if (strstr(buff_start, "go\n")) {
      go_cmd(myBoard, &engine_mode, engine_color);

      if (DEBUG) {
        myBoard->printBoard();
      }

      buff_end = buff_start;
      continue;
    }

    if (is_move(buff_start)) {
      cmd = is_move(buff_start);
      if (engine_mode == GO) {
        myBoard->move(cmd);
        think_cmd(myBoard, engine_color);

        if (DEBUG) {
          myBoard->printBoard();
        }

        buff_end = buff_start;
        continue;
      } else if (engine_mode == FORCE) {
        myBoard->move(cmd);

        if (DEBUG) {
          myBoard->printBoard();
        }

        buff_end = buff_start;
        continue;
      } else {
        // ERROR
      }
    }

    if (strstr(buff_start, "white\n")) {
      white_cmd(&engine_color);
      buff_end = buff_start;
      continue;
    }

    if (strstr(buff_start, "black\n")) {
      black_cmd(&engine_color);
      buff_end = buff_start;
      continue;
    }

    if (strstr(buff_start, "quit\n")) {
      break;
    }
  }

  free(buff_start);
}

/*
 * Printeaza la stdout comanda ($cmd) primita.
 * $cmd este un sir de caractere ce are la final '\n', dupa care terminatorul
 * de sir '\0'. De exemplu: cmd -> "move b2b4\n\0"
 */
void print_cmd(char *cmd) {
  write(STDOUT_FILENO, cmd, strlen(cmd));
}
