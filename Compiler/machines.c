/* =======================================================
 *	 		PCS 2056 - Linguagens e Compiladores
 * =======================================================
 *
 * machines.c - Finite state machines module
 *
 * Created on: 29/10/2011
 * 		Authors:
 *     		Bruno Pezzolo dos Santos, 5948816
 *      	Carla Guillen Gomes, 5691366
 *
 */

#include "machines.h"

/***********************************************************/
/* 						PRIVATE							   */
/***********************************************************/

void init_machine(Machine *machine) {
	int i, j;

	machine->current_state = MACHINE_INVALID_STATE;
	machine->machine_id = MTYPE_INVALID;

	for(i = 0; i < MAX_FINAL_STATES; i++)
		machine->final_state[i] = MACHINE_INVALID_STATE;
	for(i=0; i < MAX_STATES; i++)
		for(j=0; j < MTOKEN_TYPES; j++)
			machine->transition_table[i][j] = MACHINE_INVALID_STATE;
	for(i=0; i < MAX_STATES; i++)
		for(j=0; j < MACHINE_TYPES; j++)
			machine->machine_calls[i][j] = MACHINE_INVALID_STATE;
}

/***********************************************************/
/* 						PUBLIC							   */
/***********************************************************/

void init_machines() {
	Machine program, command, expression;

	// Machine Program: MTYPE_PROGRAM
	init_machine(&program);

	program.machine_id = MTYPE_PROGRAM;
	program.current_state = 0;

	program.final_state[0] = 10;

	// transition table
	program.transition_table[0][MTTYPE_PROGRAM] = 1;

	program.transition_table[1][MTTYPE_TYPEDEF] = 2;
	program.transition_table[1][MTTYPE_DECLARE] = 3;
	program.transition_table[1][MTTYPE_FUNCTION] = 4;
	program.transition_table[1][MTTYPE_MAIN] = 5;

	program.transition_table[2][MTTYPE_STRUCT] = 6;

	program.transition_table[3][MTTYPE_STRUCT] = 35;
	program.transition_table[3][MTTYPE_INT] = 36;
	program.transition_table[3][MTTYPE_BOOLEAN] = 36;

	program.transition_table[4][MTTYPE_INT] = 34;
	program.transition_table[4][MTTYPE_BOOLEAN] = 34;

	program.transition_table[5][MTTYPE_LEFT_CURLY_BRACKET] = 7;

	program.transition_table[6][MTTYPE_IDENTIFIER] = 8;

	program.transition_table[7][MTTYPE_DECLARE] = 9;
	program.transition_table[7][MTTYPE_RIGHT_CURLY_BRACKET] = 10;

	program.transition_table[8][MTTYPE_LEFT_CURLY_BRACKET] = 12;

	program.transition_table[9][MTTYPE_STRUCT] = 14;
	program.transition_table[9][MTTYPE_INT] = 15;
	program.transition_table[9][MTTYPE_BOOLEAN] = 15;

	program.transition_table[11][MTTYPE_RIGHT_CURLY_BRACKET] = 10;

	program.transition_table[12][MTTYPE_DECLARE] = 13;
	program.transition_table[12][MTTYPE_RIGHT_CURLY_BRACKET] = 1;

	program.transition_table[13][MTTYPE_STRUCT] = 16;
	program.transition_table[13][MTTYPE_INT] = 17;
	program.transition_table[13][MTTYPE_BOOLEAN] = 17;

	program.transition_table[14][MTTYPE_IDENTIFIER] = 28;

	program.transition_table[15][MTTYPE_IDENTIFIER] = 18;

	program.transition_table[16][MTTYPE_IDENTIFIER] = 29;

	program.transition_table[17][MTTYPE_IDENTIFIER] = 19;

	program.transition_table[18][MTTYPE_LEFT_SQUARE_BRACKET] = 20;
	program.transition_table[18][MTTYPE_COMMA] = 21;
	program.transition_table[18][MTTYPE_SEMICOLON] = 7;

	program.transition_table[19][MTTYPE_LEFT_SQUARE_BRACKET] = 22;
	program.transition_table[19][MTTYPE_COMMA] = 23;
	program.transition_table[19][MTTYPE_SEMICOLON] = 12;

	program.transition_table[20][MTTYPE_NUMBER] = 24;

	program.transition_table[21][MTTYPE_INT] = 15;
	program.transition_table[21][MTTYPE_BOOLEAN] = 15;

	program.transition_table[22][MTTYPE_NUMBER] = 25;

	program.transition_table[23][MTTYPE_INT] = 17;
	program.transition_table[23][MTTYPE_BOOLEAN] = 17;

	program.transition_table[24][MTTYPE_RIGHT_SQUARE_BRACKET] = 26;

	program.transition_table[25][MTTYPE_RIGHT_SQUARE_BRACKET] = 27;

	program.transition_table[26][MTTYPE_COMMA] = 21;
	program.transition_table[26][MTTYPE_SEMICOLON] = 7;

	program.transition_table[27][MTTYPE_COMMA] = 23;
	program.transition_table[27][MTTYPE_SEMICOLON] = 12;

	program.transition_table[28][MTTYPE_IDENTIFIER] = 30;

	program.transition_table[29][MTTYPE_IDENTIFIER] = 31;

	program.transition_table[30][MTTYPE_COMMA] = 32;
	program.transition_table[30][MTTYPE_SEMICOLON] = 7;

	program.transition_table[31][MTTYPE_COMMA] = 33;
	program.transition_table[31][MTTYPE_SEMICOLON] = 12;

	program.transition_table[32][MTTYPE_STRUCT] = 14;

	program.transition_table[33][MTTYPE_STRUCT] = 16;

	program.transition_table[34][MTTYPE_IDENTIFIER] = 37;

	program.transition_table[35][MTTYPE_IDENTIFIER] = 57;

	program.transition_table[36][MTTYPE_IDENTIFIER] = 38;

	program.transition_table[37][MTTYPE_LEFT_PARENTHESES] = 39;

	program.transition_table[38][MTTYPE_LEFT_SQUARE_BRACKET] = 40;
	program.transition_table[38][MTTYPE_COMMA] = 41;
	program.transition_table[38][MTTYPE_SEMICOLON] = 42;

	program.transition_table[39][MTTYPE_INT] = 43;
	program.transition_table[39][MTTYPE_BOOLEAN] = 43;
	program.transition_table[39][MTTYPE_RIGHT_PARENTHESES] = 44;

	program.transition_table[40][MTTYPE_NUMBER] = 49;

	program.transition_table[41][MTTYPE_INT] = 36;
	program.transition_table[41][MTTYPE_BOOLEAN] = 36;

	program.transition_table[42][MTTYPE_DECLARE] = 3;
	program.transition_table[42][MTTYPE_FUNCTION] = 4;
	program.transition_table[42][MTTYPE_MAIN] = 5;

	program.transition_table[43][MTTYPE_IDENTIFIER] = 62;

	program.transition_table[44][MTTYPE_LEFT_CURLY_BRACKET] = 45;

	program.transition_table[45][MTTYPE_DECLARE] = 46;
	program.transition_table[45][MTTYPE_RIGHT_CURLY_BRACKET] = 47;

	program.transition_table[46][MTTYPE_STRUCT] = 50;
	program.transition_table[46][MTTYPE_INT] = 51;
	program.transition_table[46][MTTYPE_BOOLEAN] = 51;

	program.transition_table[47][MTTYPE_FUNCTION] = 4;
	program.transition_table[47][MTTYPE_MAIN] = 5;

	program.transition_table[48][MTTYPE_RIGHT_CURLY_BRACKET] = 47;

	program.transition_table[49][MTTYPE_RIGHT_SQUARE_BRACKET] = 52;

	program.transition_table[50][MTTYPE_IDENTIFIER] = 61;

	program.transition_table[51][MTTYPE_IDENTIFIER] = 53;

	program.transition_table[52][MTTYPE_COMMA] = 41;
	program.transition_table[52][MTTYPE_SEMICOLON] = 42;

	program.transition_table[53][MTTYPE_LEFT_SQUARE_BRACKET] = 54;
	program.transition_table[53][MTTYPE_COMMA] = 55;
	program.transition_table[53][MTTYPE_SEMICOLON] = 45;

	program.transition_table[54][MTTYPE_NUMBER] = 56;

	program.transition_table[55][MTTYPE_INT] = 51;
	program.transition_table[55][MTTYPE_BOOLEAN] = 51;

	program.transition_table[56][MTTYPE_RIGHT_SQUARE_BRACKET] = 58;

	program.transition_table[57][MTTYPE_IDENTIFIER] = 59;

	program.transition_table[58][MTTYPE_COMMA] = 55;
	program.transition_table[58][MTTYPE_SEMICOLON] = 45;

	program.transition_table[59][MTTYPE_COMMA] = 60;
	program.transition_table[59][MTTYPE_SEMICOLON] = 42;

	program.transition_table[60][MTTYPE_STRUCT] = 35;

	program.transition_table[61][MTTYPE_IDENTIFIER] = 63;

	program.transition_table[62][MTTYPE_LEFT_SQUARE_BRACKET] = 64;
	program.transition_table[62][MTTYPE_COMMA] = 65;
	program.transition_table[62][MTTYPE_RIGHT_PARENTHESES] = 44;

	program.transition_table[63][MTTYPE_COMMA] = 66;
	program.transition_table[63][MTTYPE_SEMICOLON] = 45;

	program.transition_table[64][MTTYPE_NUMBER] = 67;

	program.transition_table[65][MTTYPE_INT] = 43;
	program.transition_table[65][MTTYPE_BOOLEAN] = 43;

	program.transition_table[66][MTTYPE_STRUCT] = 50;

	program.transition_table[67][MTTYPE_RIGHT_SQUARE_BRACKET] = 68;

	program.transition_table[68][MTTYPE_COMMA] = 65;
	program.transition_table[68][MTTYPE_RIGHT_PARENTHESES] = 44;

	// machine call transitions
	program.machine_calls[7][MTYPE_COMMAND] = 11;
	program.machine_calls[11][MTYPE_COMMAND] = 11;
	program.machine_calls[45][MTYPE_COMMAND] = 48;
	program.machine_calls[48][MTYPE_COMMAND] = 48;


	// Machine Command: MTYPE_COMMAND
	init_machine(&command);

	command.machine_id = MTYPE_COMMAND;
	command.current_state = 0;

	command.final_state[0] = 11;
	command.final_state[1] = 25;

	// transition table
	command.transition_table[0][MTTYPE_IDENTIFIER] = 1;
	command.transition_table[0][MTTYPE_IF] = 2;
	command.transition_table[0][MTTYPE_WHILE] = 3;
	command.transition_table[0][MTTYPE_INPOUT] = 4;
	command.transition_table[0][MTTYPE_OUTPUT] = 4;
	command.transition_table[0][MTTYPE_RETURN] = 5;

	command.transition_table[1][MTTYPE_EQUAL] = 6;
	command.transition_table[1][MTTYPE_LEFT_SQUARE_BRACKET] = 7;
	command.transition_table[1][MTTYPE_LEFT_PARENTHESES] = 8;

	command.transition_table[2][MTTYPE_LEFT_PARENTHESES] = 21;

	command.transition_table[3][MTTYPE_LEFT_PARENTHESES] = 15;

	command.transition_table[4][MTTYPE_IDENTIFIER] = 9;

	command.transition_table[6][MTTYPE_LEFT_CURLY_BRACKET] = 18;

	command.transition_table[7][MTTYPE_NUMBER] = 13;

	command.transition_table[8][MTTYPE_RIGHT_PARENTHESES] = 9;

	command.transition_table[9][MTTYPE_SEMICOLON] = 11;

	command.transition_table[10][MTTYPE_COMMA] = 12;
	command.transition_table[10][MTTYPE_RIGHT_PARENTHESES] = 9;

	command.transition_table[13][MTTYPE_RIGHT_SQUARE_BRACKET] = 14;

	command.transition_table[14][MTTYPE_EQUAL] = 5;

	command.transition_table[16][MTTYPE_RIGHT_PARENTHESES] = 17;

	command.transition_table[17][MTTYPE_RIGHT_CURLY_BRACKET] = 19;

	command.transition_table[19][MTTYPE_RIGHT_CURLY_BRACKET] = 11;

	command.transition_table[20][MTTYPE_COMMA] = 18;
	command.transition_table[20][MTTYPE_RIGHT_CURLY_BRACKET] = 9;

	command.transition_table[22][MTTYPE_RIGHT_PARENTHESES] = 23;

	command.transition_table[23][MTTYPE_LEFT_CURLY_BRACKET] = 24;

	command.transition_table[24][MTTYPE_RIGHT_CURLY_BRACKET] = 25;

	command.transition_table[25][MTTYPE_ELSE] = 17;

	// machine call transitions
	command.machine_calls[5][MTYPE_EXPRESSION] = 9;
	command.machine_calls[6][MTYPE_EXPRESSION] = 9;
	command.machine_calls[8][MTYPE_EXPRESSION] = 10;
	command.machine_calls[12][MTYPE_EXPRESSION] = 10;
	command.machine_calls[15][MTYPE_EXPRESSION] = 16;
	command.machine_calls[18][MTYPE_EXPRESSION] = 20;
	command.machine_calls[19][MTYPE_COMMAND] = 19;
	command.machine_calls[21][MTYPE_EXPRESSION] = 22;
	command.machine_calls[24][MTYPE_COMMAND] = 24;


	// Machine Expression: MTYPE_EXPRESSION
	init_machine(&expression);

	expression.machine_id = MTYPE_EXPRESSION;
	expression.current_state = 0;

	expression.final_state[0] = 1;
	expression.final_state[1] = 4;

	// transition table
	expression.transition_table[0][MTTYPE_TRUE]  = 1;
	expression.transition_table[0][MTTYPE_FALSE] = 1;
	expression.transition_table[0][MTTYPE_NOT] = 2;
	expression.transition_table[0][MTTYPE_NUMBER] = 1;
	expression.transition_table[0][MTTYPE_IDENTIFIER] = 4;

	expression.transition_table[3][MTTYPE_GREATER_THAN] = 2;
	expression.transition_table[3][MTTYPE_LESS_THAN] = 2;
	expression.transition_table[3][MTTYPE_EQUAL_EQUAL] = 2;
	expression.transition_table[3][MTTYPE_AND] = 2;
	expression.transition_table[3][MTTYPE_OR] = 2;
	expression.transition_table[3][MTTYPE_PLUS] = 2;
	expression.transition_table[3][MTTYPE_MINUS] = 2;
	expression.transition_table[3][MTTYPE_MULTIPLICATION] = 2;
	expression.transition_table[3][MTTYPE_DIVISION] = 2;

	expression.transition_table[4][MTTYPE_LEFT_SQUARE_BRACKET] = 5;
	expression.transition_table[4][MTTYPE_DOT] = 6;
	expression.transition_table[4][MTTYPE_LEFT_PARENTHESES] = 7;

	expression.transition_table[5][MTTYPE_NUMBER] = 10;

	expression.transition_table[6][MTTYPE_IDENTIFIER] = 1;

	expression.transition_table[7][MTTYPE_RIGHT_PARENTHESES] = 1;

	expression.transition_table[8][MTTYPE_COMMA] = 9;
	expression.transition_table[8][MTTYPE_RIGHT_PARENTHESES] = 1;

	expression.transition_table[10][MTTYPE_RIGHT_SQUARE_BRACKET] = 1;

	// machine call transitions
	expression.machine_calls[0][MTYPE_EXPRESSION] = 3;
	expression.machine_calls[2][MTYPE_EXPRESSION] = 1;
	expression.machine_calls[7][MTYPE_EXPRESSION] = 8;
	expression.machine_calls[9][MTYPE_EXPRESSION] = 8;

	machines_array[MTYPE_PROGRAM] = program;
	machines_array[MTYPE_COMMAND] = command;
	machines_array[MTYPE_EXPRESSION] = expression;
}
