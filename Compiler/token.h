/* =======================================================
 *	 		PCS 2056 - Linguagens e Compiladores
 * =======================================================
 *
 * token.h - Token
 *
 * Criado em: 21/09/2011
 * 		Autores:
 *     		Bruno Pezzolo dos Santos, 5948816
 *      	Carla Guillen Gomes, 5691366
 */

#ifndef TOKEN_H_
#define TOKEN_H_

#include <stdlib.h>

#define TTYPE_RESERVED_WORD 0x01
#define TTYPE_IDENTIFIER 0x01 << 1
#define TTYPE_SPECIAL_CHARACTER 0x01 << 2
#define TTYPE_NUM 0x01 << 3
#define TTYPE_STRING 0x01 << 4
#define TTYPE_COMMENT 0x01 << 5
#define TTYPE_INVALID 0x01 << 6

#define TTYPE_IDENT_OR_RESERV (TTYPE_RESERVED_WORD | TTYPE_IDENTIFIER)

typedef struct {
	int type;
	char *lexeme;
	int value; // index_on_table for TTYPE_IDENTIFIER, TTYPE_OPERATOR, TTYPE_RESERVED_WORD or value for TTYPE_NUMBER / not used when TTYPE_STRING
} Token;

extern Token *token; // global

void init_token();
void release_token();
void set_lexeme(char *lexeme);

#endif /* TOKEN_H_ */
