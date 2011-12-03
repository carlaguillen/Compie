/* =======================================================
 *	 		PCS 2056 - Linguagens e Compiladores
 * =======================================================
 *
 * writer.h - File writer
 *
 * Created on: 03/12/2011
 * 		Authors:
 *     		Bruno Pezzolo dos Santos, 5948816
 *      	Carla Guillen Gomes, 5691366
 *
 */

#ifndef WRITER_H_
#define WRITER_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct _writer {
	char * filepath;
	FILE * code_buffer;
    FILE * data_buffer;
} Writer;

Writer * writer;

void init_file_writer(char * path);

void write_to_data(char * string);
void write_to_code(char * string);
int merge_code_data();
void close_file_writer();

#endif /* WRITER_H_ */
