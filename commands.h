#ifndef __COMMANDS_H_
#define __COMMANDS_H_

#include "board.h"
#include <iostream>

/*
 * Creaza si initializeaza structurile de date.
 */
void initial_cmd(Board *myBoard);

/*
 * Transmite la xboard (cu print_cmd(.)) comanda:
 * <    feature sigint=0 san=0 name=GAMEMAPD
 */
void feature_cmd();

/*
 * Reinitializeaza pozitiile pe tabla de sah;
 * Initializeaza partida;
 */
void new_cmd(int *engine_color, int *engine_mode);

/*
 * Intra in starea "force mode"
 */
void force_cmd(int *engine_mode);

/*
 * Muta piesa pentru culoarea curenta.
 * Schimba randul culorii ce va muta.
 */
void forced_move_cmd(char *movement);

/*
 * Intra in starea "go mode".
 */
void go_cmd(Board *myBoard, int *engine_mode, int engine_color);

/*
 * Analizeaza structurile de date folosind algoritmii. Gaseste o mutare valida
 * si buna pentru bot. Sau se preda.
 */
void think_cmd(Board *myBoard, int engine_color);

/*
 * Set engine as white ($engine_color).
 */
void white_cmd(int *engine_color);

/*
 * Set engine as black ($engine_color).
 */
void black_cmd(int *engine_color);

#endif
