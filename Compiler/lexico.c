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

void get_next_token() {
	transducer_get_next_token();
	complete_token();
}

void complete_token() {
	if (token->type == TTYPE_IDENT_OR_RESERV) {
		int index_of_reserved = id_of_reserved(token.lexeme);
		if (index_of_reserved != -1) {
			token->type = TTYPE_RESERV;
			token->value = index_of_reserved;
		} else {
			token->type = TTYPE_IDENTIFIER;
			token->value = get_or_insert_identifier(token.lexeme);
		}
	}
	else {
		//todo:
		//token.index_on_table = ...
	}

}
