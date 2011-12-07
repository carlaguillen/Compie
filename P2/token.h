/* =======================================================
 *	 		PCS 2056 - Linguagens e Compiladores
 * =======================================================
 *
 * token.h - Token
 *
 * Created on: 21/09/2011
 * 		Authors:
 *     		Bruno Pezzolo dos Santos, 5948816
 *      	Carla Guillen Gomes, 5691366
 */

#ifndef TOKEN_H_
#define TOKEN_H_

#include <stdlib.h>

#define TTYPE_ID 0x01
#define TTYPE_OPERATOR 0x01 << 1
#define TTYPE_NUM 0x01 << 2
#define TTYPE_SPECIAL 0x01 << 3
#define TTYPE_INVALID 0x01 << 4
#define TTYPE_END_OF_FILE 0x01 << 5

typedef struct {
	int type;
	char *lexeme;
	int value; /* index_on_table for TTYPE_ID, TTYPE_OPERATOR, TTYPE_SPECIAL or value for TTYPE_NUM */
} Token;

extern Token *token; /* global */

void print_token();
void init_token();
void release_token();
void set_lexeme(char *lexeme);


#endif /* TOKEN_H_ */
