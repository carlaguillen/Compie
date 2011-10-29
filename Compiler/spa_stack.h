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

#ifndef STACK_H_
#define STACK_H_

#include <stdio.h>
#include <stdlib.h>

typedef struct spaNode {
	int state;
	struct spaNode * next;
} SpaNode;

typedef struct spaStack {
	struct spaNode * head;
} SpaStack;

SpaStack * empty_spa_stack();
void push_state_spa_stack(int state, SpaStack * stack);
int pop_spa_stack(SpaStack * stack);
int spa_stack_is_empty(SpaStack *stack);

#endif /* STACK_H_ */
