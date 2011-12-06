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

#define LEN_OF_ID_TABLE 27
#define LEN_OF_SPECIAL_TABLE 2
#define LEN_OF_OPERATOR_TABLE 5

#define lenght(array) (sizeof(array)/sizeof(*array))

extern char * reserved_words_table[];
extern char * special_characters_table[];
extern List * identifiers_table;
extern List * constants_table;

int search_id_table(char * data);
int search_special_table(char * data);
int search_operator_table(char * data);

void display_id_table();
void display_special_table();
void display_operator_table();


/*NOT USED YET*/
void enter_new_table_of_symbols();
void exit_current_table_of_symbols();
int search_identifiers_table(char * data);
int add_identifiers_table(char * data);
int add_if_new_identifiers_table(char * data);
Node * get_identifier_at_index(int index);
Node * get_identifier_for_data(char * data);
Node * get_identifier_for_data_on_current_table(char * data);
Node * get_identifier_for_label_on_current_table(char * label);
int search_constants_table(char * data);
int add_constants_table(char * data);
int add_if_new_constants_table(char * data);
Node * get_constant_at_index(int index);



#endif /* TABLE_OF_SYMBOLS_H_ */
