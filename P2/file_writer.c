/* =======================================================
 *	 		PCS 2056 - Linguagens e Compiladores
 * =======================================================
 *
 * writer.c - File writer
 *
 * Created on: 03/12/2011
 * 		Authors:
 *     		Bruno Pezzolo dos Santos, 5948816
 *      	Carla Guillen Gomes, 5691366
 *
 */

#include "file_writer.h"
#include "util.h"

#define CODE_APPEND "_code.il"
#define DATA_APPEND "_data.il"

void init_code_buffer() {
	FILE * execution_enviroment = fopen("ambienteexecucao.il", "r");
	char buffer[128];

	if (execution_enviroment != NULL)
		while (!feof(execution_enviroment)) {
			fgets(buffer, sizeof(buffer), execution_enviroment);
			if(buffer == NULL) break;
			else {
				printf("%s\n", buffer);
				fputs(buffer, writer->code_buffer);
			}
		}
	fclose(execution_enviroment);
}

void init_data_buffer() {
}

void init_file_writer(char * path) {
	writer = (Writer *)malloc(sizeof(Writer));
	writer->filepath = path;

	char * filepath = remove_ext(path, '.', '/');
	strcat(filepath, CODE_APPEND);
	writer->code_buffer = fopen(filepath, "w+");
	if(writer->code_buffer == NULL) fprintf(stderr,"Could not create writer code buffer.\n");
	else init_code_buffer();

	filepath = remove_ext(path, '.', '/');
	strcat(filepath, DATA_APPEND);
	writer->data_buffer = fopen(filepath, "w+");
	if (writer->data_buffer == NULL) fprintf(stderr,"Could not create writer data buffer.\n");
	else init_data_buffer();
}

void write_to_data(char * string) {
	fprintf(writer->data_buffer, string);
}
void write_to_code(char * string) {
	fprintf(writer->code_buffer, string);
}

void close_file_writer() {
	fclose(writer->code_buffer);
	fclose(writer->data_buffer);
}

int merge_code_data() {
	char * filepath = remove_ext(writer->filepath, '.', '/');
	FILE *object = fopen(strcat(filepath, ".il"), "w");
	if (object == NULL) return 0;

	char buffer[128];
	filepath = remove_ext(writer->filepath, '.', '/');
	writer->code_buffer = fopen(strcat(filepath, CODE_APPEND), "r");
	if (writer->code_buffer != NULL)
		while (!feof(writer->code_buffer))
			fputs(fgets(buffer, sizeof(buffer), writer->code_buffer), object);
	fclose(writer->code_buffer);

	filepath = remove_ext(writer->filepath, '.', '/');
		writer->data_buffer = fopen(strcat(filepath, DATA_APPEND), "r");
	if (writer->data_buffer != NULL)
		while (!feof(writer->data_buffer))
			fputs(fgets(buffer, sizeof(buffer), writer->data_buffer), object);
	fclose(writer->data_buffer);

	fclose(object);
	return 1;
}
