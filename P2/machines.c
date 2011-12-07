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
			machine->state_transition_table[i][j] = MACHINE_INVALID_STATE;
	for(i=0; i < MAX_STATES; i++)
		for(j=0; j < MACHINE_TYPES; j++)
			machine->machine_calls[i][j] = MACHINE_INVALID_STATE;
}

/***********************************************************/
/* 						PUBLIC							   */
/***********************************************************/

void init_machines() {
	Machine program, operation_right, operation_left, loop;

	/* Machine Program: MTYPE_PROGRAM */
	init_machine(&program);

	program.machine_id = MTYPE_PROGRAM;
	program.current_state = 0;

	program.final_state[0] = 3;

	/* transition table */
	program.state_transition_table[0][MTTYPE_ID] =  1;
	program.state_transition_table[0][MTTYPE_NUMBER] =  2;

	program.state_transition_table[1][MTTYPE_QUESTION_MARK] =  3;

	program.state_transition_table[3][MTTYPE_ID] =  1;
	program.state_transition_table[3][MTTYPE_NUMBER] =  2;

	/* machine call transitions */
	program.machine_calls[0][MTYPE_LOOP] = 3;
	program.machine_calls[1][MTYPE_OPERATION_RIGHT] = 3;
	program.machine_calls[1][MTYPE_OPERATION_LEFT] = 3;
	program.machine_calls[2][MTYPE_OPERATION_RIGHT] = 3;
	program.machine_calls[3][MTYPE_LOOP] = 3;

	/* Machine Command: MTYPE_OPERATION_RIGHT */
	init_machine(&operation_right);

	operation_right.machine_id = MTYPE_OPERATION_RIGHT;
	operation_right.current_state = 0;

	operation_right.final_state[0] = 2;
	operation_right.final_state[1] = 3;

	/* transition table */
	operation_right.state_transition_table[0][MTTYPE_GREATER_THAN] =  1;

	operation_right.state_transition_table[1][MTTYPE_ID] =  2;

	operation_right.state_transition_table[2][MTTYPE_QUESTION_MARK] =  3;

	/* machine call transitions */
	operation_right.machine_calls[2][MTYPE_OPERATION_RIGHT] = 3;
	operation_right.machine_calls[2][MTYPE_OPERATION_LEFT] = 3;

	/* Machine Expression: MTYPE_OPERATION_LEFT */
	init_machine(&operation_left);

	operation_left.machine_id = MTYPE_OPERATION_LEFT;
	operation_left.current_state = 0;

	operation_left.final_state[0] = 2;
	operation_left.final_state[1] = 3;
	operation_left.final_state[2] = 4;

	/* transition table */
	operation_left.state_transition_table[0][MTTYPE_LESS_THAN] =  1;
	operation_left.state_transition_table[0][MTTYPE_PLUS] =  1;
	operation_left.state_transition_table[0][MTTYPE_MINUS] =  1;

	operation_left.state_transition_table[1][MTTYPE_ID] =  2;
	operation_left.state_transition_table[1][MTTYPE_NUMBER] =  3;

	operation_left.state_transition_table[2][MTTYPE_QUESTION_MARK] =  4;

	/* machine call transitions */
	operation_left.machine_calls[2][MTYPE_OPERATION_RIGHT] = 4;
	operation_left.machine_calls[2][MTYPE_OPERATION_LEFT] = 4;
	operation_left.machine_calls[3][MTYPE_OPERATION_RIGHT] = 4;


	/* Machine Expression: MTYPE_LOOP */
	init_machine(&loop);

	loop.machine_id = MTYPE_LOOP;
	loop.current_state = 0;

	loop.final_state[0] = 6;

	/* transition table */
	loop.state_transition_table[0][MTTYPE_LEFT_PARENTHESES] =  1;

	loop.state_transition_table[1][MTTYPE_ID] =  2;
	loop.state_transition_table[1][MTTYPE_NUMBER] =  3;

	loop.state_transition_table[2][MTTYPE_ID] =  4;
	loop.state_transition_table[2][MTTYPE_QUESTION_MARK] =  5;
	loop.state_transition_table[2][MTTYPE_NUMBER] =  3;
	loop.state_transition_table[2][MTTYPE_RIGHT_PARENTHESES] =  6;

	loop.state_transition_table[4][MTTYPE_QUESTION_MARK] =  5;

	loop.state_transition_table[5][MTTYPE_ID] =  4;
	loop.state_transition_table[5][MTTYPE_NUMBER] =  3;
	loop.state_transition_table[5][MTTYPE_RIGHT_PARENTHESES] =  6;

	/* machine call transitions */
	loop.machine_calls[2][MTYPE_OPERATION_RIGHT] = 5;
	loop.machine_calls[2][MTYPE_OPERATION_LEFT] = 5;
	loop.machine_calls[2][MTYPE_LOOP] = 5;
	loop.machine_calls[3][MTYPE_OPERATION_RIGHT] = 5;
	loop.machine_calls[4][MTYPE_OPERATION_RIGHT] = 5;
	loop.machine_calls[4][MTYPE_OPERATION_LEFT] = 5;
	loop.machine_calls[5][MTYPE_LOOP] = 5;

	machines_array[MTYPE_PROGRAM] = program;
	machines_array[MTYPE_OPERATION_RIGHT] = operation_right;
	machines_array[MTYPE_OPERATION_LEFT] = operation_left;
	machines_array[MTYPE_LOOP] = loop;
}
