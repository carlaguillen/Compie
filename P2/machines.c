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
	Machine program, operation_right, operation_left;

	/* Machine Program: MTYPE_PROGRAM */
	init_machine(&program);

	program.machine_id = MTYPE_PROGRAM;
	program.current_state = 0;

	program.final_state[0] = 6;

	/* transition table */
	program.transition_table[0][MTTYPE_ID] = 1;
	program.transition_table[0][MTTYPE_NUMBER] = 2;
	program.transition_table[0][MTTYPE_LEFT_PARENTHESES] = 3;

	program.transition_table[1][MTTYPE_GREATER_THAN] =  4;
	program.transition_table[1][MTTYPE_LESS_THAN] =  5;
	program.transition_table[1][MTTYPE_PLUS] =  5;
	program.transition_table[1][MTTYPE_MINUS] =  5;
	program.transition_table[1][MTTYPE_QUESTION_MARK] =  6;

	program.transition_table[2][MTTYPE_GREATER_THAN] =  4;

	program.transition_table[3][MTTYPE_ID] =  7;
	program.transition_table[3][MTTYPE_NUMBER] =  8;

	program.transition_table[6][MTTYPE_ID] =  1;
	program.transition_table[6][MTTYPE_NUMBER] =  2;
	program.transition_table[6][MTTYPE_LEFT_PARENTHESES] =  3;

	program.transition_table[7][MTTYPE_ID] =  11;
	program.transition_table[7][MTTYPE_GREATER_THAN] =  9;
	program.transition_table[7][MTTYPE_LESS_THAN] =  12;
	program.transition_table[7][MTTYPE_PLUS] =  12;
	program.transition_table[7][MTTYPE_MINUS] =  12;
	program.transition_table[7][MTTYPE_QUESTION_MARK] =  10;
	program.transition_table[7][MTTYPE_NUMBER] =  8;
	program.transition_table[7][MTTYPE_RIGHT_PARENTHESES] =  6;

	program.transition_table[8][MTTYPE_GREATER_THAN] =  9;

	program.transition_table[10][MTTYPE_ID] =  11;
	program.transition_table[10][MTTYPE_NUMBER] =  8;
	program.transition_table[10][MTTYPE_RIGHT_PARENTHESES] =  6;

	program.transition_table[11][MTTYPE_GREATER_THAN] =  9;
	program.transition_table[11][MTTYPE_LESS_THAN] =  12;
	program.transition_table[11][MTTYPE_PLUS] =  12;
	program.transition_table[11][MTTYPE_MINUS] =  12;
	program.transition_table[11][MTTYPE_QUESTION_MARK] =  10;

	/* machine call transitions */
	program.machine_calls[4][MTYPE_OPERATION_RIGHT] = 6;
	program.machine_calls[5][MTYPE_OPERATION_LEFT] = 6;
	program.machine_calls[9][MTYPE_OPERATION_RIGHT] = 10;
	program.machine_calls[12][MTYPE_OPERATION_LEFT] = 10;

	/* Machine Command: MTYPE_OPERATION_RIGHT */
	init_machine(&operation_right);

	operation_right.machine_id = MTYPE_OPERATION_RIGHT;
	operation_right.current_state = 0;

	operation_right.final_state[0] = 1;
	operation_right.final_state[1] = 4;

	/* transition table */
	operation_right.transition_table[0][MTTYPE_ID] =  1;

	operation_right.transition_table[1][MTTYPE_GREATER_THAN] =  2;
	operation_right.transition_table[1][MTTYPE_LESS_THAN] =  3;
	operation_right.transition_table[1][MTTYPE_PLUS] =  3;
	operation_right.transition_table[1][MTTYPE_MINUS] =  3;
	operation_right.transition_table[1][MTTYPE_QUESTION_MARK] =  4;

	/* machine call transitions */
	operation_right.machine_calls[2][MTYPE_OPERATION_RIGHT] = 4;
	operation_right.machine_calls[3][MTYPE_OPERATION_LEFT] = 4;

	/* Machine Expression: MTYPE_OPERATION_LEFT */
	init_machine(&operation_left);

	operation_left.machine_id = MTYPE_OPERATION_LEFT;
	operation_left.current_state = 0;

	operation_left.final_state[0] = 1;
	operation_left.final_state[1] = 2;
	operation_left.final_state[2] = 5;

	/* transition table */
	operation_left.transition_table[0][MTTYPE_ID] =  1;
	operation_left.transition_table[0][MTTYPE_NUMBER] =  2;

	operation_left.transition_table[1][MTTYPE_GREATER_THAN] =  3;
	operation_left.transition_table[1][MTTYPE_LESS_THAN] =  4;
	operation_left.transition_table[1][MTTYPE_PLUS] =  4;
	operation_left.transition_table[1][MTTYPE_MINUS] =  4;
	operation_left.transition_table[1][MTTYPE_QUESTION_MARK] =  5;

	operation_left.transition_table[2][MTTYPE_GREATER_THAN] =  3;

	/* machine call transitions */

	operation_left.machine_calls[3][MTYPE_OPERATION_RIGHT] = 5;
	operation_left.machine_calls[4][MTYPE_OPERATION_LEFT] = 5;

	machines_array[MTYPE_PROGRAM] = program;
	machines_array[MTYPE_OPERATION_RIGHT] = operation_right;
	machines_array[MTYPE_OPERATION_LEFT] = operation_left;
}
