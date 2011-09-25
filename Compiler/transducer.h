/* =======================================================
 *	 		PCS 2056 - Linguagens e Compiladores
 * =======================================================
 *
 *
 * Criado em: 15/09/2011
 * 		Autores:
 *     		Bruno Pezzolo dos Santos, 5948816
 *      	Carla Guillen Gomes, 5691366
 */

#ifndef TRANSDUCER_H_
#define TRANSDUCER_H_

#include "token.h"
#include <stdlib.h>
#include <string.h>
#include "file_reader.h"
#include "linked_list.h" 

#define NUM_STATES 9
#define NUM_TYPE_CHARS 10

typedef enum {
	letter,
	digit,
	space,
	sharp,
	equal,
	quote,
	linebreak,
	special,
	eof,
	invalid_char
} type_char;

int transition_table[NUM_STATES][NUM_TYPE_CHARS];
void transducer_get_next_token();

#endif /* TRANSDUCER_H_ */


//CARLA



