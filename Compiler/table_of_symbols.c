/* =======================================================
 *	 		PCS 2056 - Linguagens e Compiladores
 * =======================================================
 *
 * table_of_symbols.c - Symbol table
 *
 * Created on: 21/09/2011
 * 		Authors:
 *     		Bruno Pezzolo dos Santos, 5948816
 *      	Carla Guillen Gomes, 5691366
 */

#include "table_of_symbols.h"
#include "string.h"

/***********************************************************/
/* 						PRIVATE							   */
/***********************************************************/

/* This functions is not used because when asked for length(table)
 It always returns 1, even though you can run trough all of table's
 elements.
 It's probably something with argument passing, didn't find out */
int search_table(char * data, char * table[]) {
	int i;
	for(i = 0; i < lenght(table); i++)
		if(strcmp(table[i], data) == 0) return i;
	return INDEX_NOT_FOUND;
}

/***********************************************************/
/* 						PUBLIC							   */
/***********************************************************/

char * reserved_words_table[LEN_OF_RESERVED_TABLE] = { "program", "main", "typedef", "declare", "function", "return", "struct", "int", "boolean", "if", "else", "while", "input", "output", "true", "false", "and", "or", "not", "void","string", "def"};
char * special_characters_table[LEN_OF_SPECIAL_CHAR_TABLE] = { "{", "}", "[", "]", "(", ")", ";", ",", ".", "=", ">", "<", "==", "+", "-", "*", "/", "\""};
List * identifiers_table = NULL;
List * constants_table = NULL;

int search_reserved_words_table(char * data) {
	int i;
	for(i = 0; i < lenght(reserved_words_table); i++)
		if(strcmp(reserved_words_table[i], data) == 0) return i;
	return INDEX_NOT_FOUND;
}

int search_special_characters_table(char * data) {
	int i;
	for(i = 0; i < lenght(special_characters_table); i++)
		if(strcmp(special_characters_table[i], data) == 0) return i;
	return INDEX_NOT_FOUND;
}

void enter_new_table_of_symbols() {
	List *newTable = empty_list();
	newTable->parent = identifiers_table;
	identifiers_table = newTable;
}

void exit_current_table_of_symbols() {
	List *current_table = identifiers_table;
	identifiers_table = current_table->parent;
	free(current_table);
}

int search_identifiers_table(char * data) {
	if (identifiers_table == NULL) identifiers_table =  empty_list();
	return search_list(data, identifiers_table);
}

int add_identifiers_table(char * data) {
	if (identifiers_table == NULL) identifiers_table =  empty_list();
	return add_list(data, identifiers_table);
}

int add_if_new_identifiers_table(char * data) {
	int index = search_identifiers_table(data);
	if(index == INDEX_NOT_FOUND) index = add_identifiers_table(data);
	return index;
}

Node * get_identifier_at_index(int index) {
	return get_node_at_index(index, identifiers_table);
}

Node * get_identifier_for_data_on_all_tables(char * data, List * identifiers_table) {
	if(identifiers_table == NULL) return NULL;
	if(search_list(data, identifiers_table) != INDEX_NOT_FOUND)
			return get_node_at_index(search_identifiers_table(data), identifiers_table);
	return get_identifier_for_data_on_all_tables(data, identifiers_table->parent);
}

Node * get_identifier_for_data(char * data) {
		return get_identifier_for_data_on_all_tables(data, identifiers_table);
}

Node * get_identifier_for_data_on_current_table(char * data) {
	if(search_list(data, identifiers_table) == INDEX_NOT_FOUND) return NULL;
	return get_node_at_index(search_identifiers_table(data), identifiers_table);
}


int search_constants_table(char * data) {
	if (constants_table == NULL) constants_table =  empty_list();
	return search_list(data, constants_table);
}

int add_constants_table(char * data) {
	if (constants_table == NULL) constants_table =  empty_list();
	return add_list(data, constants_table);
}

int add_if_new_constants_table(char * data) {
	int index = search_constants_table(data);
	if(index == INDEX_NOT_FOUND) index = add_constants_table(data);
	return index;
}

Node * get_constant_at_index(int index) {
	return get_node_at_index(index, constants_table);
}

void display_identifiers_table() {
	printf("\nIdentifiers table\n\n");
	display_list(identifiers_table);
}

void display_reserved_words_table() {
	int i;
	printf("\nReserved words table \n\n");
	for (i = 0; i < LEN_OF_RESERVED_TABLE; i++) {
		printf("%d\t|\t%s\n", i, reserved_words_table[i]);
	}
}

void display_special_characters_table() {
	int i;
	printf("\nSpecial characters table\n\n");
	for (i = 0; i < LEN_OF_SPECIAL_CHAR_TABLE; i++) {
		printf("%d\t|\t%s\n", i, special_characters_table[i]);
	}
} 
