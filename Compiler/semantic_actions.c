/* =======================================================
 *	 		PCS 2056 - Linguagens e Compiladores
 * =======================================================
 *
 * semantic.c - Semantic Actions
 *
 * Created on: 31/10/2011
 * 		Authors:
 *     		Bruno Pezzolo dos Santos, 5948816
 *      	Carla Guillen Gomes, 5691366
 *
 */

#include "semantic_actions.h"
#include "file_writer.h"
#include "token.h"

static int constant_counter = 0;

void dummy_semantic_action(Token * token) {
	// DOES NOTHING
	printf("TODO\n");
}

/* Expression Machine Semantic Actions */

void load_true(Token * token) {
	write_to_code("\t\tLD  one\t\t; Carrega true\n");
}

void load_false(Token * token) {
	write_to_code("\t\tLD  zero\t\t; Carrega false\n");
}

char * get_constant_label() {
	char * label = (char *)malloc(sizeof(100*sizeof(char)));
	sprintf(label, "K%d", constant_counter);
	constant_counter++;
	return label;
}

void load_constant(Token * token) {
    char * label = get_constant_label();

    char * line = (char *)malloc(sizeof(100*sizeof(char)));
    sprintf(line, "%s\t\tK  =%d\t\t; Declaração de constante\n", label, token->value);
    write_to_data(line);

    sprintf(line, "\t\tLD  %s\t\t; Carrega constante\n", label);
	write_to_code(line);
}


void init_semantic_actions() {
	int i, j, k = 0;

	for( i = 0; i < MACHINE_TYPES; i++)
		for( j = 0; j < MAX_STATES; j++) {
			for( k = 0; k < MTOKEN_TYPES; k++)
				actions_on_state_transition[i][j][k] = dummy_semantic_action;

			for( k = 0; k < MACHINE_TYPES; k++)
				actions_on_machine_transition[i][j][k] = dummy_semantic_action;

			actions_on_machine_return[i][j] = dummy_semantic_action;
		}

	/* Actions for Expression Machine: MTYPE_EXPRESSION */
	actions_on_state_transition[MTYPE_EXPRESSION][0][MTTYPE_TRUE]  = load_true;
	actions_on_state_transition[MTYPE_EXPRESSION][0][MTTYPE_FALSE] = load_false;
//	actions_on_state_transition[MTYPE_EXPRESSION][0][MTTYPE_NOT] = 2;
	actions_on_state_transition[MTYPE_EXPRESSION][0][MTTYPE_NUMBER] = load_constant;
//	actions_on_state_transition[MTYPE_EXPRESSION][0][MTTYPE_IDENTIFIER] = 3;
//
//	actions_on_state_transition[MTYPE_EXPRESSION][1][MTTYPE_GREATER_THAN] = 4;
//	actions_on_state_transition[MTYPE_EXPRESSION][1][MTTYPE_LESS_THAN] = 4;
//	actions_on_state_transition[MTYPE_EXPRESSION][1][MTTYPE_EQUAL_EQUAL] = 4;
//	actions_on_state_transition[MTYPE_EXPRESSION][1][MTTYPE_AND] = 4;
//	actions_on_state_transition[MTYPE_EXPRESSION][1][MTTYPE_OR] = 4;
//	actions_on_state_transition[MTYPE_EXPRESSION][1][MTTYPE_PLUS] = 4;
//	actions_on_state_transition[MTYPE_EXPRESSION][1][MTTYPE_MINUS] = 4;
//	actions_on_state_transition[MTYPE_EXPRESSION][1][MTTYPE_MULTIPLICATION] = 4;
//	actions_on_state_transition[MTYPE_EXPRESSION][1][MTTYPE_DIVISION] = 4;
//
//	actions_on_state_transition[MTYPE_EXPRESSION][3][MTTYPE_LEFT_SQUARE_BRACKET] = 5;
//	actions_on_state_transition[MTYPE_EXPRESSION][3][MTTYPE_DOT] = 6;
//	actions_on_state_transition[MTYPE_EXPRESSION][3][MTTYPE_LEFT_PARENTHESES] = 7;
//	actions_on_state_transition[MTYPE_EXPRESSION][3][MTTYPE_GREATER_THAN] = 4;
//	actions_on_state_transition[MTYPE_EXPRESSION][3][MTTYPE_LESS_THAN] = 4;
//	actions_on_state_transition[MTYPE_EXPRESSION][3][MTTYPE_EQUAL_EQUAL] = 4;
//	actions_on_state_transition[MTYPE_EXPRESSION][3][MTTYPE_AND] = 4;
//	actions_on_state_transition[MTYPE_EXPRESSION][3][MTTYPE_OR] = 4;
//	actions_on_state_transition[MTYPE_EXPRESSION][3][MTTYPE_PLUS] = 4;
//	actions_on_state_transition[MTYPE_EXPRESSION][3][MTTYPE_MINUS] = 4;
//	actions_on_state_transition[MTYPE_EXPRESSION][3][MTTYPE_MULTIPLICATION] = 4;
//	actions_on_state_transition[MTYPE_EXPRESSION][3][MTTYPE_DIVISION] = 4;
//
//	actions_on_state_transition[MTYPE_EXPRESSION][5][MTTYPE_NUMBER] = 11;
//
//	actions_on_state_transition[MTYPE_EXPRESSION][6][MTTYPE_IDENTIFIER] = 1;
//
//	actions_on_state_transition[MTYPE_EXPRESSION][7][MTTYPE_RIGHT_PARENTHESES] = 1;
//
//	actions_on_state_transition[MTYPE_EXPRESSION][9][MTTYPE_COMMA] = 10;
//	actions_on_state_transition[MTYPE_EXPRESSION][9][MTTYPE_RIGHT_PARENTHESES] = 1;
//
//	actions_on_state_transition[MTYPE_EXPRESSION][11][MTTYPE_RIGHT_SQUARE_BRACKET] = 1;
//
//	/* machine call transitions */
//	actions_on_machine_transition[2][MTYPE_EXPRESSION] = 1;
//	actions_on_machine_transition[4][MTYPE_EXPRESSION] = 8;
//	actions_on_machine_transition[7][MTYPE_EXPRESSION] = 9;
//	actions_on_machine_transition[10][MTYPE_EXPRESSION] = 9;
}
