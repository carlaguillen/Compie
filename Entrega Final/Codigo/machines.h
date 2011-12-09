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

#define MTYPE_PROGRAM		0
#define MTYPE_COMMAND		1
#define MTYPE_EXPRESSION	2
#define MTYPE_INVALID		-1

/************************************************************/
/*					MACHINE TOKEN TYPES						*/
/************************************************************/
#define MTTYPE_INVALID				-1

#define MTTYPE_PROGRAM				0		/* program */
#define MTTYPE_MAIN					1		/* main */
#define MTTYPE_TYPEDEF				2		/* typedef */
#define MTTYPE_DECLARE				3		/* declare */
#define MTTYPE_FUNCTION				4		/* function */
#define MTTYPE_RETURN 				5		/* return */
#define MTTYPE_STRUCT 				6		/* struct */
#define MTTYPE_INT 					7		/* int */
#define MTTYPE_BOOLEAN				8		/* boolean */
#define MTTYPE_IF  					9		/* if */
#define MTTYPE_ELSE 				10		/* else */
#define MTTYPE_WHILE 				11		/* while */
#define MTTYPE_INPUT 				12		/* input */
#define MTTYPE_OUTPUT 				13		/* output */
#define MTTYPE_TRUE 				14		/* true */
#define MTTYPE_FALSE 				15		/* false */
#define MTTYPE_NOT 					16		/* not */
#define MTTYPE_AND 					17		/* and */
#define MTTYPE_OR 					18		/* or */

#define MTTYPE_IDENTIFIER 			19		/* identificador */
#define MTTYPE_NUMBER 				20		/* numero */

#define MTTYPE_LEFT_CURLY_BRACKET 	21		/* { */ 
#define MTTYPE_RIGHT_CURLY_BRACKET 	22		/* }  */
#define MTTYPE_LEFT_SQUARE_BRACKET 	23		/* [  */
#define MTTYPE_RIGHT_SQUARE_BRACKET	24		/* ]  */
#define MTTYPE_LEFT_PARENTHESES		25		/* (  */
#define MTTYPE_RIGHT_PARENTHESES	26		/* )  */
#define MTTYPE_SEMICOLON			27		/* ;  */
#define MTTYPE_COMMA				28		/* ,  */
#define MTTYPE_DOT 					29		/* .  */
#define MTTYPE_EQUAL				30		/* =  */
#define MTTYPE_GREATER_THAN			31		/* >  */
#define MTTYPE_LESS_THAN			32		/* <  */
#define MTTYPE_EQUAL_EQUAL			33		/* == */
#define MTTYPE_PLUS					34		/* +  */
#define MTTYPE_MINUS				35		/* -  */
#define MTTYPE_MULTIPLICATION		36		/* *  */
#define MTTYPE_DIVISION				37		/* /  */

/************************************************************/
/*						MACHINE								*/
/************************************************************/

#define MAX_FINAL_STATES 10
#define MAX_STATES 68
#define MTOKEN_TYPES 37
#define MACHINE_TYPES 3

typedef struct machine {
	int machine_id;
	int current_state;
	int final_state[MAX_FINAL_STATES];
	int transition_table[MAX_STATES][MTOKEN_TYPES];
	int machine_calls[MAX_STATES][MACHINE_TYPES];
} Machine;

Machine machines_array[3];

void init_machines();
void is_machine_in_final_state(int machineId);

#endif /* MACHINES_H_ */
