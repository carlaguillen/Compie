/* =======================================================
 *	 		PCS 2056 - Linguagens e Compiladores
 * =======================================================
 *
 * spa_stack.h - Structured Pushdown Automata Stack
 *
 * Created on: 29/10/2011
 * 		Authors:
 *     		Bruno Pezzolo dos Santos, 5948816
 *      	Carla Guillen Gomes, 5691366
 *
 */

#ifndef SPA_STACK_H_
#define SPA_STACK_H_

#include <stdio.h>
#include <stdlib.h>
#include "machines.h"

typedef struct spaNode {
	int machine_id;
	int state;
	struct spaNode * next;
} SpaNode;

typedef struct spaStack {
	struct spaNode * head;
} SpaStack;

SpaStack * empty_spa_stack();
void spa_stack_push(Machine machine, SpaStack * stack);
Machine spa_stack_pop(SpaStack * stack);
int spa_stack_is_empty(SpaStack *stack);

#endif /* SPA_STACK_H_ */
