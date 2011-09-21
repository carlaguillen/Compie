/* =======================================================
 *	 		PCS 2056 - Linguagens e Compiladores
 * =======================================================
 *
 * Analisador léxico
 *
 * Criado em: 15/09/2011
 * 		Autores:
 *     		Bruno Pezzolo dos Santos, 5948816
 *      	Carla Guillen Gomes, 5948816
 */

#ifndef LEXICO_H_
#define LEXICO_H_

#include <stdio.h>

extern FILE *file;

extern void getNextTokenInto(char *token);

#endif /* LEXICO_H_ */
