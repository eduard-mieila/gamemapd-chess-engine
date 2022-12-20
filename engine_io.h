#ifndef __ENGINE_IO_H__
#define __ENGINE_IO_H__

/*
 * Declaratiile functiilor de input/output ale engine-ului; 
 * Aceste functii sunt folosite pentru a comunica cu xboardul:
 *        - de la input citeste comenzile primite de la xboard
 *        - la output scrie comenzi pentru xboard
 */

/*
 * Citeste comenzile primite de la xboard.
 */
void cmd_interpreter();

/*
 * Transmite comenzi la xboard.
 */
void print_cmd(char *cmd);

#endif
