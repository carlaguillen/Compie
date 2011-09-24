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

// This functions is not used because when asked for length(table)
// It always returns 1, even though you can run trough all of table's
// elements.
// It's probably something with argument passing, didn't find out
int search_table(char * data, char * table[]) {
	int i;
	for(i = 0; i < lenght(table); i++)
		if(strcmp(table[i], data) == 0) return i;
	return INDEX_NOT_FOUND;
}

/***********************************************************/
/* 						PUBLIC							   */
/***********************************************************/

char * reserved_words_table[15] = { "void", "boolean", "int", "string", "main", "def", "if", "else", "while", "return", "true", "false", "and", "or", "not" };
char * operators_table[14] = { "+", "-", "*", "/", ">", "<", "==", "=", "(", ")", "\"", "{", "}", ";" };
List * identifiers_table = NULL;

int search_reserved_words_table(char * data) {
	int i;
	for(i = 0; i < lenght(reserved_words_table); i++)
		if(strcmp(reserved_words_table[i], data) == 0) return i;
	return INDEX_NOT_FOUND;
}

int search_special_characters_table(char * data) {
	int i;
	for(i = 0; i < lenght(operators_table); i++)
		if(strcmp(operators_table[i], data) == 0) return i;
	return INDEX_NOT_FOUND;
}

int search_identifiers_table(char * data) {
	if (identifiers_table == NULL) identifiers_table =  empty_list();
	return search_list(data, identifiers_table);
}

int add_identifiers_table(char * data) {
	if (identifiers_table == NULL) identifiers_table =  empty_list();
	return add_list(data, identifiers_table);
}

int add_if_new_indetifiers_table(char * data) {
	int index = search_identifiers_table(data);
	if(index == INDEX_NOT_FOUND) index = add_identifiers_table(data);
	return index;
}

void display_identifiers_table() {
	display_list(identifiers_table);
}
