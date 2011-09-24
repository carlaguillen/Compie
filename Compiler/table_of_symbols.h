/* =======================================================
 *	 		PCS 2056 - Linguagens e Compiladores
 * =======================================================
 *
 * table_of_symbols.h - Symbol table
 *
 * Created on: 21/09/2011
 * 		Authors:
 *     		Bruno Pezzolo dos Santos, 5948816
 *      	Carla Guillen Gomes, 5691366
 *
 */

#ifndef TABLE_OF_SYMBOLS_H_
#define TABLE_OF_SYMBOLS_H_

#include "linked_list.h"

#define lenght(array) (sizeof(array)/sizeof(*array))

extern char * reserved_words_table[];
extern char * special_characters_table[];
extern List * identifiers_table;

int search_reserved_words_table(char * data);
int search_operators_table(char * data);

int search_identifiers_table(char * data);
int add_identifiers_table(char * data);
int add_if_new_indetifiers_table(char * data);
void display_identifiers_table();

#endif /* TABLE_OF_SYMBOLS_H_ */
