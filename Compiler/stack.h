/* =======================================================
 *	 		PCS 2056 - Linguagens e Compiladores
 * =======================================================
 *
 * stack.h - Stack
 *
 * Created on: 03/12/2011
 * 		Authors:
 *     		Bruno Pezzolo dos Santos, 5948816
 *      	Carla Guillen Gomes, 5691366
 *
 */

#ifndef STACK_H
#define STACK_H

#include <stdlib.h>
#include <stdio.h>

typedef struct _node {
	char * content;
	struct _node *next;
} StackNode;

typedef struct _stack {
	StackNode * head;
	struct _stack * parent;
} Stack;

Stack * empty_stack();
void stack_push(Stack * head, char * c);
char * stack_pop(Stack * head);
char * stack_check(Stack * head);
int stack_is_empty(Stack * head);

#endif
