/* =======================================================
 *	 		PCS 2056 - Linguagens e Compiladores
 * =======================================================
 *
 * stack.c - Stack
 *
 * Created on: 03/12/2011
 * 		Authors:
 *     		Bruno Pezzolo dos Santos, 5948816
 *      	Carla Guillen Gomes, 5691366
 *
 */

#include "stack.h"

Stack * empty_stack() {
	Stack *stack = (Stack *)malloc(sizeof(Stack));
	stack->head = NULL;
	stack->parent = NULL;
	return stack;
}

void stack_push(Stack * stack, char * c, int f) {
	StackNode * node = malloc(sizeof(StackNode));

	if (node == NULL) {
		fprintf(stderr, "Error: no space available for Stack node\n");
	} else {
		node->content = c;
		node->functionFlag = f;
		node->next = stack->head;
		stack->head = node;
	}

}

StackNode * stack_pop(Stack * stack) {
	if (stack_is_empty(stack)) {
		fprintf(stderr, "Error: Underflow on Stack\n");
		abort();
	}

	StackNode * top = stack->head;
	stack->head = top->next;
//	free(top);
	return top;
}

StackNode * stack_check(Stack * stack) {
	if (stack_is_empty(stack)) {
		return NULL;
	} else {
		return stack->head;
	}

}

int stack_is_empty(Stack * stack) {
	return stack->head == NULL;
}
