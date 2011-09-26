/* =======================================================
 *	 		PCS 2056 - Linguagens e Compiladores
 * =======================================================
 *
 * lexico.c - Lexical analyzer
 *
 * Created on: 15/09/2011
 * 		Authors:
 *     		Bruno Pezzolo dos Santos, 5948816
 *      	Carla Guillen Gomes, 5691366
 */

#include "lexico.h"
#include "string.h"

/***********************************************************/
/* 						PRIVATE							   */
/***********************************************************/

char *substring(char *from, size_t begin, size_t len) {
	char *substring;
	if(from == 0 || strlen(from) == 0 || strlen(from) < begin || strlen(from) < (begin+len)) return 0;

	substring = (char *)malloc(len);
	strncpy(substring, from+begin, len);
	substring[len+1] = '\0';
	return substring;
}

void complete_token() {
	switch(token->type) {
	case TTYPE_IDENT_OR_RESERV:
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
		break;
	case TTYPE_SPECIAL_CHARACTER:
		token->value = search_special_characters_table(token->lexeme);
		break;
	case TTYPE_NUM:
		token->value = atoi(token->lexeme);
		break;
	case TTYPE_STRING:
		token->lexeme = substring(token->lexeme, 1, strlen(token->lexeme)-2);
		break;
	default:
		// Other types do not have token->value
		break;
	}
}

/***********************************************************/
/* 						PUBLIC							   */
/***********************************************************/

void get_next_token() {
	transducer_get_next_token();
	complete_token();
}
