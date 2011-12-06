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
#include "file_reader.h"

/***********************************************************/
/* 						PRIVATE							   */
/***********************************************************/

SpaStack *spa_stack;
static int did_finish = 0;

int spa_convert_token_to_machine_type() {
	switch (token->type) {

		case TTYPE_SPECIAL:
			if( strcmp(token->lexeme, "(")  == 0 )   return MTTYPE_LEFT_PARENTHESES;
			if( strcmp(token->lexeme, ")")  == 0 )   return MTTYPE_RIGHT_PARENTHESES;
			break;
		case TTYPE_OPERATOR:
			if( strcmp(token->lexeme, ">")  == 0 )   return MTTYPE_GREATER_THAN;
			if( strcmp(token->lexeme, "<")  == 0 )   return MTTYPE_LESS_THAN;
			if( strcmp(token->lexeme, "+")  == 0 )   return MTTYPE_PLUS;
			if( strcmp(token->lexeme, "-")  == 0 )   return MTTYPE_MINUS;
			if( strcmp(token->lexeme, "?")  == 0 )   return MTTYPE_MINUS;
			break;
		case TTYPE_ID:
			return MTTYPE_ID;
			break;
		case TTYPE_NUM:
			return MTTYPE_NUMBER;
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
	actions_on_machine_return[current_machine.machine_id][current_machine.current_state](token);
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
	actions_on_machine_transition[current_machine.machine_id][current_machine.current_state][machine_type](token);

	current_machine.current_state = current_machine.machine_calls[current_machine.current_state][machine_type];
	spa_stack_push(current_machine, spa_stack);

	current_machine = machines_array[machine_type];
	current_machine.current_state = 0;
}

void transition_to_next_state (int next_state) {
	actions_on_state_transition[current_machine.machine_id][current_machine.current_state][spa_convert_token_to_machine_type()](token);
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
	int next_machine = machine_call_for_current_machine();
	/* can call another machine */
	if (next_machine != MTYPE_INVALID) {
		call_machine(next_machine);
		return transition_current_machine_with_token(machine_token_type);
	}
	/* cannot call another machine -> check if can pop a machine */
	if (current_machine_is_in_final_state()) {
		if (!spa_stack_is_empty(spa_stack)) {
			return_machine();
			return transition_current_machine_with_token(machine_token_type);
		} else {
			did_finish = 1;
			return 1;
		}
	}

	throw_semantic_exception(ERR_SINTATIC, "source code could not be correctly parsed");
	did_finish = 1;
	return 0;
}

/***********************************************************/
/* 						PUBLIC							   */
/***********************************************************/

int spa_did_finish() {
	return did_finish;
}

void spa_init() {
	init_machines();
	spa_stack = empty_spa_stack();
	current_machine = machines_array[MTYPE_PROGRAM];
	init_semantic_actions();
}

static int halt = 0;

int spa_step() {
	if(halt) return 0;

	int machine_token_type = spa_convert_token_to_machine_type();

	return transition_current_machine_with_token(machine_token_type);
}

void throw_sintatic_exception(int code, char *err) {
	fprintf(stderr, "@sintatic - exception code-%d: %s\n", code, err);
	halt = 1;
}

void throw_semantic_exception(int code, char * err) {
	fprintf(stderr, "@semantic - exception code-%d: %s\n", code, err);
	halt = 1;
}
