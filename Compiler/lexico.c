/* =======================================================
 *	 		PCS 2056 - Linguagens e Compiladores
 * =======================================================
 *
 * Analisador léxico
 *
 * Criado em: 15/09/2011
 * 		Autores:
 *     		Bruno Pezzolo dos Santos, 5948816
 *      	Carla Guillen Gomes, 5691366
 */

#include "lexico.h"
#include "token.h"

/*
void getNextTokenInto(char *token) {

}
*/

Token get_next_token() {
	Token * token;
	token = transducer_get_next_token();
	complete_token(token);
	return token; 
}

void complete_token(Token * token) {
	if (token->type == IDENT_OR_RESERV) {
		int index_of_reserved = id_of_reserved(token.lexeme);
		if (index_of_reserved != -1) {
			token->type = RESERV;
			token->index_on_table = index_of_reserved;
		} else {
			token->type = IDENT;
			token->index_on_table = get_or_insert_identifier(token.lexeme);
		}
	}
	else {
		//todo:
		//token.index_on_table = ...
	}

}