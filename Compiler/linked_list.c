/* =======================================================
 *	 		PCS 2056 - Linguagens e Compiladores
 * =======================================================
 *
 * linked_list.c - Linked list
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

#include "linked_list.h"
#include "string.h"

Node * createnode(char * data);

Node * createnode(char * data){
  Node * newNode = malloc(sizeof(Node));
  newNode->data = data;
  newNode->next = NULL;
  return newNode;
}

List * empty_list(){
  List * list = malloc(sizeof(List));
  list->head = NULL;
  return list;
}

void display_list(List * list) {
  Node * current = list->head;
  if(list->head == NULL)
    return;
  while(current->next != NULL){
    printf("%s,", current->data);
    current = current->next;
  }
  printf("%s\n", current->data);
}

int add_list(char * data, List * list){
  Node * current = NULL;
  int index = 0;
  if(list->head == NULL){
    list->head = createnode(data);
  }
  else {
    current = list->head;
    while (current->next != NULL){
      current = current->next;
      index++;
    }
    current->next = createnode(data);
    index++;
  }
  return index;
}

int search_list(char * data, List *list) {
	int index = 0;
	Node * current = list->head;
	while (current != NULL) {
		if(strcmp(current->data, data) == 0) return index;
		current = current->next;
		index++;
	}
	return INDEX_NOT_FOUND;
}

void delete_list(char * data, List * list){
  Node * current = list->head;
  Node * previous = current;
  while(current != NULL){
    if(current->data == data){
      previous->next = current->next;
      if(current == list->head)
        list->head = current->next;
      free(current);
      return;
    }
    previous = current;
    current = current->next;
  }
}

void destroy_list(List * list){
  Node * current = list->head;
  Node * next = current;
  while(current != NULL){
    next = current->next;
    free(current);
    current = next;
  }
  free(list);
}

char * get_string_array(List * list) {
	int size = 0;
	Node * node;
	char * string;
	int i;

	for(node = list->head; node != NULL; node = node->next) {
		size ++;
	}
	string = (char *)malloc(size*sizeof(char));
	i = 0;
	for(node = list->head; node != NULL; node = node->next) {
		string[i] = *(node->data);
		i++;
	}
	return string;
} 

int alloc_add_list(char data, List * list) {
	char * ptr = (char *)malloc(sizeof(char));
	*ptr = data;
	add_list(ptr, list);
}
