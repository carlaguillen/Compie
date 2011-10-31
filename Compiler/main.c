/* =======================================================
 *	 		PCS 2056 - Linguagens e Compiladores
 * =======================================================
 * This program consist on the 1st stage of the PCS2056
 * course's compiler. The implementation of a lexical analyser.
 *
 * Created on: 15/09/2011
 * 		Authors:
 *     		Bruno Pezzolo dos Santos, 5948816
 *      	Carla Guillen Gomes, 5691366
 */

#include <stdlib.h>
#include <stdio.h>
#include "file_reader.h"
#include "table_of_symbols.h"
#include "spa.h"

int main(int argc, char **argv) {

	init_file_reader("entrada_teste.txt");
	init_token();
	init_transition_table();
	spa_init();

	get_next_token(); /* lexical analyser */
//	print_token();

	while (token->type != TTYPE_END_OF_FILE) {
		if (token->type != TTYPE_COMMENT && token->type != TTYPE_STRING)
			if (!spa_step()) { /* sintatic analyser */
				printf("Source code would not be correctly parsed;");
				break;
			}
		get_next_token(); /* lexical analyser */
//		print_token();
	}

//	display_reserved_words_table();
//	display_special_characters_table();
//	display_identifiers_table();

	return 0;
}
