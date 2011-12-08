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
	char c;
	switch(token->type) {
	case TTYPE_SPECIAL:
		token->value = search_special_table(token->lexeme);
		break;
	case TTYPE_NUM:
		token->value = atoi(token->lexeme);
		break;
	case TTYPE_ID:
		/* changing upper case to lower case*/
		c = token->lexeme[0];
		if(c >= 'A' && c <= 'Z') {
			c -= 'A' - 'a';
			char s[] = {c,'\0'};
			token->value = search_id_table(s);
			token->lexeme = s;
		} else{
			token->value = search_id_table(token->lexeme);
		}
		break;
	case TTYPE_OPERATOR:
			token->value = search_operator_table(token->lexeme);
			break;
	default:
		/* Other types do not have token->value */
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
