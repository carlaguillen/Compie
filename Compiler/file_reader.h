/* =======================================================
 *	 		PCS 2056 - Linguagens e Compiladores
 * =======================================================
 *
 * lexico.h - Lexical analyzer
 *
 * Created on: 15/09/2011
 * 		Authors:
 *     		Bruno Pezzolo dos Santos, 5948816
 *      	Carla Guillen Gomes, 5691366
 */

#ifndef FILE_READER_H_
#define FILE_READER_H_

char get_next_char();
char get_look_ahead();
void init_file_reader(char *path);

#endif /* FILE_READER_H_ */
