/* =======================================================
 *	 		PCS 2056 - Linguagens e Compiladores
 * =======================================================
 *
 * spa.c - Structured Pushdown Automata
 *
 * Created on: 30/10/2011
 * 		Authors:
 *     		Bruno Pezzolo dos Santos, 5948816
 *      	Carla Guillen Gomes, 5691366
 *
 */

#include "spa.h"
#include "semantic.h"

/***********************************************************/
/* 						PRIVATE							   */
/***********************************************************/

SpaStack *spa_stack;

int spa_convert_token_to_machine_type() {
	switch (token->type) {
		case TTYPE_RESERVED_WORD:
			if ( strcmp(token->lexeme, "program")	== 0 ) 			return MTTYPE_PROGRAM;
			if ( strcmp(token->lexeme, "main") 		== 0 ) 			return MTTYPE_MAIN;
			if ( strcmp(token->lexeme, "typedef") 	== 0 ) 			return MTTYPE_TYPEDEF;
			if ( strcmp(token->lexeme, "declare") 	== 0 ) 			return MTTYPE_DECLARE;
			if ( strcmp(token->lexeme, "function")	== 0 ) 			return MTTYPE_FUNCTION;
			if ( strcmp(token->lexeme, "return") 	== 0 ) 			return MTTYPE_RETURN;
			if ( strcmp(token->lexeme, "struct")	== 0 ) 			return MTTYPE_STRUCT;
			if ( strcmp(token->lexeme, "int")		== 0 ) 			return MTTYPE_INT;
			if ( strcmp(token->lexeme, "boolean")	== 0 ) 			return MTTYPE_BOOLEAN;
			if ( strcmp(token->lexeme, "if")		== 0 ) 			return MTTYPE_IF;
			if ( strcmp(token->lexeme, "else")		== 0 )			return MTTYPE_ELSE;
			if ( strcmp(token->lexeme, "while")		== 0 )			return MTTYPE_WHILE;
			if ( strcmp(token->lexeme, "input")		== 0 )			return MTTYPE_INPOUT;
			if ( strcmp(token->lexeme, "output")	== 0 )			return MTTYPE_OUTPUT;
			if ( strcmp(token->lexeme, "true")		== 0 )			return MTTYPE_TRUE;
			if ( strcmp(token->lexeme, "false")		== 0 ) 			return MTTYPE_FALSE;
			if ( strcmp(token->lexeme, "not")		== 0 ) 			return MTTYPE_NOT;
			if ( strcmp(token->lexeme, "and")		== 0 )	 		return MTTYPE_AND;
			if ( strcmp(token->lexeme, "or")		== 0 ) 			return MTTYPE_OR;
			break;
		case TTYPE_SPECIAL_CHARACTER:
			if( strcmp(token->lexeme, "{")  == 0 )   return MTTYPE_LEFT_CURLY_BRACKET;
			if( strcmp(token->lexeme, "}")  == 0 )   return MTTYPE_RIGHT_CURLY_BRACKET;
			if( strcmp(token->lexeme, "[")  == 0 )   return MTTYPE_LEFT_SQUARE_BRACKET;
			if( strcmp(token->lexeme, "]")  == 0 )   return MTTYPE_RIGHT_SQUARE_BRACKET;
			if( strcmp(token->lexeme, "(")  == 0 )   return MTTYPE_LEFT_PARENTHESES;
			if( strcmp(token->lexeme, ")")  == 0 )   return MTTYPE_RIGHT_PARENTHESES;
			if( strcmp(token->lexeme, ";")  == 0 )   return MTTYPE_SEMICOLON;
			if( strcmp(token->lexeme, ",")  == 0 )   return MTTYPE_COMMA;
			if( strcmp(token->lexeme, ".")  == 0 )   return MTTYPE_DOT;
			if( strcmp(token->lexeme, "=")  == 0 )   return MTTYPE_EQUAL;
			if( strcmp(token->lexeme, ">")  == 0 )   return MTTYPE_GREATER_THAN;
			if( strcmp(token->lexeme, "<")  == 0 )   return MTTYPE_LESS_THAN;
			if( strcmp(token->lexeme, "==") == 0 )	 return MTTYPE_EQUAL_EQUAL;
			if( strcmp(token->lexeme, "+")  == 0 )   return MTTYPE_PLUS;
			if( strcmp(token->lexeme, "-")  == 0 )   return MTTYPE_MINUS;
			if( strcmp(token->lexeme, "*")  == 0 )   return MTTYPE_MULTIPLICATION;
			if( strcmp(token->lexeme, "/")  == 0 )   return MTTYPE_DIVISION;
			break;
		case TTYPE_IDENTIFIER:
			return MTTYPE_IDENTIFIER;
			break;
		case TTYPE_NUM:
			return MTTYPE_NUMBER;
			break;
		default:
			break;
	}
	return MTTYPE_INVALID;
}

int current_machine_is_in_final_state() {
	int i;
	for (i = 0; i < MAX_FINAL_STATES; i++)
		if ( current_machine.current_state == current_machine.final_state[i])
			return 1;
	return 0;
}

void return_machine() {
	semantico_tbd();
	current_machine = spa_stack_pop(spa_stack);
}

int machine_call_for_current_machine() {
	int next_machine;
	for (next_machine = 0; next_machine <  MACHINE_TYPES; next_machine++) {
		if(current_machine.machine_calls[current_machine.current_state][next_machine] != MACHINE_INVALID_STATE)
			return next_machine;
	}
	return MTYPE_INVALID;
}

void call_machine(int machine_type) {
	semantico_tbd();

	current_machine.current_state = current_machine.machine_calls[current_machine.current_state][machine_type];
	spa_stack_push(current_machine, spa_stack);

	current_machine = machines_array[machine_type];
	current_machine.current_state = 0;
}

void transition_to_next_state (int next_state) {
	semantico_tbd();

	current_machine.current_state = next_state;
}

int transition_current_machine_with_token(int machine_token_type) {

	int next_state = current_machine.transition_table[current_machine.current_state][machine_token_type];

	/* transition is valid */
	if (next_state != MACHINE_INVALID_STATE) {
		transition_to_next_state(next_state);
		return 1;
	}

	/* transition was not valid -> check if can call another machine */
	else {
		int next_machine = machine_call_for_current_machine();
		/* can call another machine */
		if (next_machine != MTYPE_INVALID) {
			call_machine(next_machine);
			return transition_current_machine_with_token(machine_token_type);
		}
		/* cannot call another machine -> check if can pop a machine */
		if (current_machine_is_in_final_state() && !spa_stack_is_empty(spa_stack)) {
			return_machine();
			return transition_current_machine_with_token(machine_token_type);
		}
	}

	return 0;
}

/***********************************************************/
/* 						PUBLIC							   */
/***********************************************************/

void spa_init() {
	init_machines();
	spa_stack = empty_spa_stack();
	current_machine = machines_array[MTYPE_PROGRAM];
}

int spa_step() {
	int machine_token_type = spa_convert_token_to_machine_type();

	if (machine_token_type == MTTYPE_INVALID) return 0;

	return transition_current_machine_with_token(machine_token_type);
}
