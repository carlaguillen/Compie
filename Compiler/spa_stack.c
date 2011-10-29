/* =======================================================
 *	 		PCS 2056 - Linguagens e Compiladores
 * =======================================================
 *
 * spa_stack.c - Structured Pushdown Automata Stack
 *
 * Created on: 29/10/2011
 * 		Authors:
 *     		Bruno Pezzolo dos Santos, 5948816
 *      	Carla Guillen Gomes, 5691366
 *
 */

#include "spa_stack.h"

SpaNode * create_spa_node(int state) {
	SpaNode * newNode = malloc(sizeof(SpaNode));

	if(newNode == NULL) {
		fputs("Error: could not allocate SpaNode\n", stderr);
		abort();
	}

	newNode->state = state;
	newNode->next = NULL;
	return newNode;
}

SpaStack * empty_spa_stack() {
	SpaStack * emptyStack = malloc(sizeof(SpaStack));
	emptyStack->head = NULL;
	return emptyStack;
}

void push_state_spa_stack(int state, SpaStack * stack) {
	SpaNode * node = create_spa_node(state);
	node->next = stack->head;
	stack->head = node;
}

int pop_spa_stack(SpaStack * stack) {
	if (spa_stack_is_empty(stack)) {
		fputs("Error: underflow on SpaStack\n", stderr);
		abort();
	}

	SpaNode * head = stack->head;
	int state = head->state;
	stack->head = head->next;
	free(head);
	return state;
}

int spa_stack_is_empty(SpaStack *stack) {
	return stack->head == NULL;
}
