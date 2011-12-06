/* =======================================================
 *	 		PCS 2056 - Linguagens e Compiladores
 * =======================================================
 *
 * token.c - Token
 *
 * Crreated on: 21/09/2011
 * 		Authors:
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
		case TTYPE_ID:
			return "TTYPE_ID";
		case TTYPE_OPERATOR:
			return "TTYPE_OPERATOR";
		case TTYPE_NUM:
			return "TTYPE_NUM";
		case TTYPE_SPECIAL:
			return "TTYPE_SPECIAL";
		case TTYPE_COMMENT:
			return "TTYPE_COMMENT";
		case TTYPE_INVALID:
			return "TTYPE_INVALID";
		case TTYPE_END_OF_FILE:
			return "TTYPE_END_OF_FILE";
		default:
			return "Not a type";
	}
}

void print_token() {
	printf("TOKEN: %s\t%s (%d)\n", token->lexeme, string_for_type_token(token->type), token->value);
} 
