/* =======================================================
 *	 		PCS 2056 - Linguagens e Compiladores
 * =======================================================
 *
 * Criado em: 21/09/2011
 * 		Autores:
 *     		Bruno Pezzolo dos Santos, 5948816
 *      	Carla Guillen Gomes, 5691366
 */

#ifndef TOKEN_H_
#define TOKEN_H_

#define TTYPE_RESERV 0x01
#define TTYPE_SPECIAL_CHARACTER 0x01 << 1
#define TTYPE_IDENTIFIER 0x01 << 2
#define TTYPE_NUM 0x01 << 3
#define TTYPE_STRING 0x01 << 4
#define TTYPE_COMMENT 0x01 << 5
#define TTYPE_INVALID 0x01 << 6

#define TTYPE_IDENT_OR_RESERV (TTYPE_RESERV | TTYPE_SPECIAL_CHARACTER)

typedef struct {
	int type;
	char *lexeme;
	int value; // index_on_table for TYPE_IDENTIFIER, OPER, RESERV or value for TYPE_NUMBER / not used when TYPE_STRING
} Token;

Token *token; // global

#endif /* TOKEN_H_ */