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
