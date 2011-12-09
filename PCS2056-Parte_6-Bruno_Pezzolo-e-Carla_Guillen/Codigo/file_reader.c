/* =======================================================
 *	 		PCS 2056 - Linguagens e Compiladores
 * =======================================================
 *
 *
 * Created on: 15/09/2011
 * 		Authors:
 *     		Bruno Pezzolo dos Santos, 5948816
 *      	Carla Guillen Gomes, 5691366
 */

#include <stdio.h>
#include "file_reader.h"

FILE *file;

void init_file_reader(char *path) {
	/* open file */
	file = fopen(path, "r");
	if (file == NULL) {
		printf("Error: could not open file %s", path);
	}
}

char get_next_char() {
	return (char)fgetc(file);
} 

char get_look_ahead() {
	int c = fgetc(file);
	ungetc(c, file);
	return (char)c;
}
