/* =======================================================
 *	 		PCS 2056 - Linguagens e Compiladores
 * =======================================================
 *
 * token.c - Token
 *
 * Criado em: 21/09/2011
 * 		Autores:
 *     		Bruno Pezzolo dos Santos, 5948816
 *      	Carla Guillen Gomes, 5691366
 */

#include "token.h"
#include "stdlib.h" 
#include "stdio.h" 

/***********************************************************/
/* 						PUBLIC							   */
/***********************************************************/

Token *token;

void init_token() {
	token = (Token *) malloc(sizeof(Token));
}

void release_token() {
	free(token);
}

char * string_for_type_token(int type) {
	switch (type) {
		case TTYPE_RESERVED_WORD:
			return "TTYPE_RESERVED_WORD";
		case TTYPE_IDENTIFIER:
			return "TTYPE_IDENTIFIER";
		case TTYPE_SPECIAL_CHARACTER:
			return "TTYPE_SPECIAL_CHARACTER";
		case TTYPE_NUM:
			return "TTYPE_NUM";
		case TTYPE_STRING:
			return "TTYPE_STRING";
		case TTYPE_COMMENT:
			return "TTYPE_COMMENT";
		case TTYPE_INVALID:
			return "TTYPE_INVALID";
		case TTYPE_END_OF_FILE:
			return "TTYPE_END_OF_FILE";
		case TTYPE_IDENT_OR_RESERV:
			return "TTYPE_IDENT_OR_RESERV";
		default:
			return "Not a type";
	}
}

void print_token() {
	printf("%s\t\t%d\t%s\n", string_for_type_token(token->type), token->value, token->lexeme);
} 