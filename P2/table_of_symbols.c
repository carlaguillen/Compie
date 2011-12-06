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

char * id_table[LEN_OF_ID_TABLE] = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", "@" };
char * special_table[LEN_OF_SPECIAL_TABLE] = { "(", ")" };
char * operator_table[LEN_OF_OPERATOR_TABLE] = { "<", "+", "-", ">", "?" };

List * constants_table = NULL;
List * identifiers_table = NULL;

int search_id_table(char * data) {
	int i;
	for(i = 0; i < lenght(id_table); i++)
		if(strcmp(id_table[i], data) == 0) return i;
	return INDEX_NOT_FOUND;
}

int search_special_table(char * data) {
	int i;
	for(i = 0; i < lenght(special_table); i++)
		if(strcmp(special_table[i], data) == 0) return i;
	return INDEX_NOT_FOUND;
}

int search_operator_table(char * data) {
	int i;
	for(i = 0; i < lenght(operator_table); i++)
		if(strcmp(operator_table[i], data) == 0) return i;
	return INDEX_NOT_FOUND;
}


void display_id_table() {
	int i;
	printf("\nId table\n\n");
	for (i = 0; i < LEN_OF_ID_TABLE; i++) {
		printf("%d\t|\t%s\n", i, id_table[i]);
	}
}

void display_special_table() {
	int i;
	printf("\nSpecial table\n\n");
	for (i = 0; i < LEN_OF_SPECIAL_TABLE; i++) {
		printf("%d\t|\t%s\n", i, special_table[i]);
	}
}

void display_operator_table() {
	int i;
	printf("\nOperator table \n\n");
	for (i = 0; i < LEN_OF_OPERATOR_TABLE; i++) {
		printf("%d\t|\t%s\n", i, operator_table[i]);
	}
}


/*NOT USED YET*/

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

int search_identifiers_table_on_label(char * label) {
	if (identifiers_table == NULL) identifiers_table = empty_list();
	return search_list_on_label(label, identifiers_table);
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
	int index = search_list(data, identifiers_table);
	if(index != INDEX_NOT_FOUND) {
			Node * identifier = get_node_at_index(index, identifiers_table);
			if(identifier->wasDeclared) return identifier;
	}
	return get_identifier_for_data_on_all_tables(data, identifiers_table->parent);
}

Node * get_identifier_for_data(char * data) {
		return get_identifier_for_data_on_all_tables(data, identifiers_table);
}

Node * get_identifier_for_data_on_current_table(char * data) {
	if(search_list(data, identifiers_table) == INDEX_NOT_FOUND) return NULL;
	return get_node_at_index(search_identifiers_table(data), identifiers_table);
}

Node * get_identifier_for_label_on_current_table(char * label) {
	if(search_list_on_label(label, identifiers_table) == INDEX_NOT_FOUND) return NULL;
		return get_node_at_index(search_identifiers_table_on_label(label), identifiers_table);
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

