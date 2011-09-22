/* =======================================================
 *	 		PCS 2056 - Linguagens e Compiladores
 * =======================================================
 *
 * linked_list.h
 *
 * Created on: 22/09/2011
 * 		Authors:
 *     		Bruno Pezzolo dos Santos, 5948816
 *      	Carla Guillen Gomes, 5691366
 *
 *  obs.: This file was based on an open source linked list
 *  implementation by Alan Skorkin (alan@skorks.com)
 *  available at: https://github.com/skorks/c-linked-list
 */

#ifndef LINKED_LIST_H_
#define LINKED_LIST_H_

#include <stdio.h>
#include <stdlib.h>

typedef struct node {
  int data;
  struct node * next;
  struct node * prev;
} Node;

typedef struct list {
  Node * head;
} List;

List * emptylist();
void add(int data, List * list);
void delete(int data, List * list);
void display(List * list);
void destroy(List * list);

#endif /* LINKED_LIST_H_ */
