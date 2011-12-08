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
#include "file_writer.h"
#include "table_of_symbols.h"
#include "spa.h"

/************************************************************/
/*					  TESTE SEMÂNTICO						*/
/************************************************************/

//int main(int argc, char **argv) {
//
//	if(argc < 2) {
//		printf("No input file to compile.");
//	}
//	char * filename = argv[1];
//	init_file_reader(filename);
//	init_file_writer(filename);
//	init_token();
//	init_transition_table();
//	spa_init();
//
//
//	get_next_token(); /* lexical analyser */
//	while (token->type != TTYPE_END_OF_FILE) {
//		if (token->type != TTYPE_COMMENT && token->type != TTYPE_STRING)
//			if (!spa_step()) { /* sintatic analyser */
//				fprintf(stderr, "source code could not be correctly parsed");
//				break;
//			}
//		get_next_token(); /* lexical analyser */
//
//	}
//
//	close_file_writer();
//	merge_code_data();
//
//	return 0;
//}

/************************************************************/
/*					  TESTE SINTÁTICO						*/
/************************************************************/

int main(int argc, char **argv) {

	init_file_reader("reverse.k");
	init_token();
	init_transition_table();
	spa_init();

	get_next_token(); /* lexical analyser */
	while (!spa_did_finish()) {
		spa_step();
		get_next_token(); /* lexical analyser */
	}
	if(!spa_did_finish()) throw_semantic_exception(ERR_SINTATIC, "source code could not be correctly parsed");
	return 0;
}

/************************************************************/
/*					   TESTE LÉXICO							*/
///************************************************************/
//int main(int argc, char **argv) {
//
//	init_file_reader("reverse.k");
//	init_token();
//	init_transition_table();
//
//	get_next_token(); /* lexical analyser */
//	print_token();
//	while (token->type != TTYPE_END_OF_FILE) {
//		get_next_token(); /* lexical analyser */
//		print_token();
//	}
//
//	display_id_table();
//	display_operator_table();
//	display_special_table();
//	return 0;
//}
