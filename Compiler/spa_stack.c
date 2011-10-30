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

/***********************************************************/
/* 						PRIVATE							   */
/***********************************************************/

SpaNode * create_spa_node(Machine machine) {
	SpaNode * newNode = malloc(sizeof(SpaNode));

	if(newNode == NULL) {
		fputs("Error: could not allocate SpaNode\n", stderr);
		abort();
	}

	newNode->machine_id = machine.machine_id;
	newNode->state = machine.current_state;
	newNode->next = NULL;
	return newNode;
}

/***********************************************************/
/* 						PUBLIC							   */
/***********************************************************/

SpaStack * empty_spa_stack() {
	SpaStack * emptyStack = malloc(sizeof(SpaStack));
	emptyStack->head = NULL;
	return emptyStack;
}

void spa_stack_push(Machine machine, SpaStack * stack) {
	SpaNode * node = create_spa_node(machine);
	node->next = stack->head;
	stack->head = node;
}

Machine spa_stack_pop(SpaStack * stack) {
	if (spa_stack_is_empty(stack)) {
		fputs("Error: underflow on SpaStack\n", stderr);
		abort();
	}

	SpaNode * head = stack->head;
	Machine machine = machines_array[head->machine_id];
	machine.current_state = head->state;
	stack->head = head->next;
	free(head);
	return machine;
}

int spa_stack_is_empty(SpaStack *stack) {
	return stack->head == NULL;
}
