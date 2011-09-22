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

typedef struct {
	int type;
	char *lexeme;
	int value; // index_on_table for TYPE_IDENTIFIER, OPER, RESERV or value for TYPE_NUMBER / not used when TYPE_STRING
} Token;

Token *token; // global

#endif /* TOKEN_H_ */