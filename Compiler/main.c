/* =======================================================
 *	 		PCS 2056 - Linguagens e Compiladores
 * =======================================================
 * Este programa consiste na 1� parte do projeto da disci-
 * plina PCS2056, a constru��o de um analisador l�xico.
 *
 * Criado em: 15/09/2011
 * 		Autores:
 *     		Bruno Pezzolo dos Santos, 5948816
 *      	Carla Guillen Gomes, 5691366
 */

#include <stdlib.h>
#include <stdio.h>
#include "lexico.h"
#include "file_reader.h"

int main(int argc, char **argv) {

	// init file_reader
	init_file_reader(&path);

	do {
		get_next_token(); // função do analisador lexico
		imprime_token(token);
	} while (token.type != END_0F_FILE)

	display_identifiers_table();
	return 0;
}
