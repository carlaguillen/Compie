/* =======================================================
 *	 		PCS 2056 - Linguagens e Compiladores
 * =======================================================
 *
 * lexico.c - Analisador léxico
 *
 * Criado em: 15/09/2011
 * 		Autores:
 *     		Bruno Pezzolo dos Santos, 5948816
 *      	Carla Guillen Gomes, 5691366
 */

#include "lexico.h"

/***********************************************************/
/* 						PRIVATE							   */
/***********************************************************/

void complete_token() {
	if(token->type == TTYPE_IDENT_OR_RESERV) {
		int index_of_reserved = search_reserved_words_table(token->lexeme);
		if (index_of_reserved != INDEX_NOT_FOUND) {
			token->type = TTYPE_RESERVED_WORD;
			token->value = index_of_reserved;
		} else {
			token->type = TTYPE_IDENTIFIER;
			token->value = add_if_new_indetifiers_table(token->lexeme);
		}
	}
}

/***********************************************************/
/* 						PUBLIC							   */
/***********************************************************/

void get_next_token() {
	transducer_get_next_token();
	complete_token();
}
