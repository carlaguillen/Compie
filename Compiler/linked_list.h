/* =======================================================
 *	 		PCS 2056 - Linguagens e Compiladores
 * =======================================================
 *
 * linked_list.h - Linked list
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

#define INDEX_NOT_FOUND -1

/*TODO: Create strcut to hold attributes of the declarations
 Attributes {
 String lexeme;
 Tipo tipo;
 bool wasDeclared;
 bool wasReferenced;
 int posMem;
 }
*/

/*TODO: Modify linked list to support attributes */
typedef struct node {
  char * data;
  char * label;
  struct node * next;
} Node;

typedef struct list {
  Node * head;
  struct list * parent;
} List;

List * empty_list();
Node * get_node_at_index(int index, List *list);
int add_list(char * data, List * list);
int search_list(char * data, List *list);
void delete_list(char * data, List * list);
void display_list(List * list);
void destroy_list(List * list);
char * get_string_array(List * list);
void alloc_add_list(char data, List * list);
void create_child(List * list);

#endif /* LINKED_LIST_H_ */
