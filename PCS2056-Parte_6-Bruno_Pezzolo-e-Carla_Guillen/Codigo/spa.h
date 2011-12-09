/* =======================================================
 *	 		PCS 2056 - Linguagens e Compiladores
 * =======================================================
 *
 * spa.h - Structured Pushdown Automata
 *
 * Created on: 30/10/2011
 * 		Authors:
 *     		Bruno Pezzolo dos Santos, 5948816
 *      	Carla Guillen Gomes, 5691366
 *
 */

#ifndef SPA_H_
#define SPA_H_

#include "spa_stack.h"
#include "lexico.h"

Machine current_machine;

int spa_convert_token_to_machine_type();
void spa_init();
int spa_step();

void throw_semantic_exception(int code, char * err);
#endif /* SPA_H_ */
