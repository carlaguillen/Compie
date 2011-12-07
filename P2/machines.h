/* =======================================================
 *	 		PCS 2056 - Linguagens e Compiladores
 * =======================================================
 *
 * machines.h - Finite state machines module
 *
 * Created on: 29/10/2011
 * 		Authors:
 *     		Bruno Pezzolo dos Santos, 5948816
 *      	Carla Guillen Gomes, 5691366
 *
 */

#ifndef MACHINES_H_
#define MACHINES_H_

#include <stdlib.h>

#define MACHINE_INVALID_STATE -1

/************************************************************/
/*						MACHINE TYPES						*/
/************************************************************/
#define MTYPE_INVALID			-1

#define MTYPE_PROGRAM			0
#define MTYPE_OPERATION_RIGHT	1
#define MTYPE_OPERATION_LEFT	2
#define MTYPE_LOOP				3

/************************************************************/
/*					MACHINE TOKEN TYPES						*/
/************************************************************/
#define MTTYPE_INVALID				-1

#define MTTYPE_ID					0		/*  stack identifier 	*/
#define MTTYPE_NUMBER 				1		/*  número 				*/
#define MTTYPE_LEFT_PARENTHESES		2		/*  ( 					*/
#define MTTYPE_RIGHT_PARENTHESES	3		/*  )  					*/
#define MTTYPE_GREATER_THAN			4		/*  >  					*/
#define MTTYPE_LESS_THAN			5		/*  <  					*/
#define MTTYPE_PLUS					6		/*  +  					*/
#define MTTYPE_MINUS				7		/*  -  					*/
#define MTTYPE_QUESTION_MARK		8		/*  ?  					*/
#define MTTYPE_END_OF_FILE			9		/*  EOF  				*/

/************************************************************/
/*						MACHINE								*/
/************************************************************/

#define MAX_FINAL_STATES 3
#define MAX_STATES 8
#define MTOKEN_TYPES 9
#define MACHINE_TYPES 4

typedef struct machine {
	int machine_id;
	int current_state;
	int final_state[MAX_FINAL_STATES];
	int state_transition_table[MAX_STATES][MTOKEN_TYPES];
	int machine_calls[MAX_STATES][MACHINE_TYPES];
} Machine;

Machine machines_array[MACHINE_TYPES];

void init_machines();
void is_machine_in_final_state(int machineId);

#endif /* MACHINES_H_ */
