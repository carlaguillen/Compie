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
#include "table_of_symbols.h"
#include "stack.h"

char buffer[100];

static int constant_counter = 0;
static int temp_counter = 0;

Stack * operand_stack;
Stack * operator_stack;

void dummy_semantic_action(Token * token) {
	// DOES NOTHING
	printf("TODO\n");
}

char * get_constant_label() {
	Node * constant = get_constant_at_index(add_if_new_constants_table(token->lexeme));
	if (constant->label == NULL) {
		constant->label = (char *)malloc(10*sizeof(char));
		sprintf(constant->label, "K%d", constant_counter);
		constant_counter++;

		sprintf(buffer, "%s\t\tK  =%d\t\t; Declaracao de constante\n", constant->label, token->value);
		write_to_data(buffer);
	}
	return constant->label;
}

int operator_precedence(char * operator) {
	if(operator != NULL) {
		if(strcmp(operator, "+") == 0) return 0;
		if(strcmp(operator, "-") == 0) return 0;
		if(strcmp(operator, "*") == 0) return 1;
		if(strcmp(operator, "/") == 0) return 1;
	}
	return -1;
}

char * get_mvn_operator(char * operator) {
	if(strcmp(operator, "+") == 0) return "+ ";
	if(strcmp(operator, "-") == 0) return "- ";
	if(strcmp(operator, "*") == 0) return "* ";
	/*if(strcmp(operator, "/") == 0)*/ return "/ ";
}

/***********************************************************/
/* 					EXPRESSION	ACTIONS					   */
/***********************************************************/

// Called in the end of an expression
//  X o Y
// o is the top of the operator stack (or LD, if stack is empty)
// X is the second on the operand stack
// Y is the top on the operand stack
void resolve_expression() {
	char * Y = stack_pop(operand_stack);
	char * o = get_mvn_operator(stack_pop(operator_stack));
	char * X = stack_pop(operand_stack);

	sprintf(buffer, "\t\t\tLD  %s\t\t;\n", X);
	write_to_code(buffer);

	sprintf(buffer, "\t\t\t%s  %s\t\t;\n", o, Y);
	write_to_code(buffer);

	char * temp = (char *)malloc(10*sizeof(char));
	sprintf(temp, "T%d", temp_counter);
	temp_counter++;

	sprintf(buffer, "%s\t\t\tK  =%d\t\t; Declaracao de temporario\n", temp, 0);
	write_to_data(buffer);
	sprintf(buffer, "\t\t\tMM  %s\t\t;\n", temp);
	write_to_code(buffer);

	stack_push(operand_stack, temp);
}

void push_operand_true(Token *token) {
	stack_push(operand_stack, "one");
}

void push_operand_false(Token *token) {
	stack_push(operand_stack, "zero");
}

void push_operand(Token * token) {
    char * label = get_constant_label(token->value);
	stack_push(operand_stack, label);
}

void push_operator(Token *token) {
	if(operator_precedence(stack_check(operator_stack)) > operator_precedence(token->lexeme)) {
		resolve_expression();
		push_operator(token);
	} else {
		stack_push(operator_stack, token->lexeme);
	}
}

void push_identifier(Token *token) {

}

void expression_end(Token *token) {
	if(!stack_is_empty(operator_stack)) {
		resolve_expression();
		expression_end(token);
	}
}

/***********************************************************/
/* 					PROGRAM ACTIONS						   */
/***********************************************************/

void print_main(Token *token) {
	sprintf(buffer, "main\t\tJP  /0000\t\t;\n");
	write_to_code(buffer);
}

void end_program(Token *token) {
	sprintf(buffer, "\t\t\tHM  /00\t\t;\n");
	write_to_code(buffer);
	sprintf(buffer, "\t\t\t#  P \t\t;\n\n");
	write_to_code(buffer);
}

void init_semantic_actions() {

	operand_stack = empty_stack();
	operator_stack = empty_stack();

	int i, j, k = 0;

	for( i = 0; i < MACHINE_TYPES; i++)
		for( j = 0; j < MAX_STATES; j++) {
			for( k = 0; k < MTOKEN_TYPES; k++)
				actions_on_state_transition[i][j][k] = dummy_semantic_action;

			for( k = 0; k < MACHINE_TYPES; k++)
				actions_on_machine_transition[i][j][k] = dummy_semantic_action;

			actions_on_machine_return[i][j] = dummy_semantic_action;
		}

	/***********************************************************/
	/* 						 PROGRAM						   */
	/***********************************************************/


//	actions_on_state_transition[MTYPE_PROGRAM][0][MTTYPE_PROGRAM] = 1;
//
//	actions_on_state_transition[MTYPE_PROGRAM][1][MTTYPE_TYPEDEF] = 2;
//	actions_on_state_transition[MTYPE_PROGRAM][1][MTTYPE_DECLARE] = 3;
//	actions_on_state_transition[MTYPE_PROGRAM][1][MTTYPE_FUNCTION] = 4;
	actions_on_state_transition[MTYPE_PROGRAM][1][MTTYPE_MAIN] = print_main;
//
//	actions_on_state_transition[MTYPE_PROGRAM][2][MTTYPE_STRUCT] = 6;
//
//	actions_on_state_transition[MTYPE_PROGRAM][3][MTTYPE_STRUCT] = 35;
//	actions_on_state_transition[MTYPE_PROGRAM][3][MTTYPE_INT] = 36;
//	actions_on_state_transition[MTYPE_PROGRAM][3][MTTYPE_BOOLEAN] = 36;
//
//	actions_on_state_transition[MTYPE_PROGRAM][4][MTTYPE_INT] = 34;
//	actions_on_state_transition[MTYPE_PROGRAM][4][MTTYPE_BOOLEAN] = 34;
//
//	actions_on_state_transition[MTYPE_PROGRAM][5][MTTYPE_LEFT_CURLY_BRACKET] = 7;
//
//	actions_on_state_transition[MTYPE_PROGRAM][6][MTTYPE_IDENTIFIER] = 8;
//
//	actions_on_state_transition[MTYPE_PROGRAM][7][MTTYPE_DECLARE] = 9;
	actions_on_state_transition[MTYPE_PROGRAM][7][MTTYPE_RIGHT_CURLY_BRACKET] = end_program;
//
//	actions_on_state_transition[MTYPE_PROGRAM][8][MTTYPE_LEFT_CURLY_BRACKET] = 12;
//
//	actions_on_state_transition[MTYPE_PROGRAM][9][MTTYPE_STRUCT] = 14;
//	actions_on_state_transition[MTYPE_PROGRAM][9][MTTYPE_INT] = 15;
//	actions_on_state_transition[MTYPE_PROGRAM][9][MTTYPE_BOOLEAN] = 15;
//
	actions_on_state_transition[MTYPE_PROGRAM][11][MTTYPE_RIGHT_CURLY_BRACKET] = end_program;
//
//	actions_on_state_transition[MTYPE_PROGRAM][12][MTTYPE_DECLARE] = 13;
//	actions_on_state_transition[MTYPE_PROGRAM][12][MTTYPE_RIGHT_CURLY_BRACKET] = 1;
//
//	actions_on_state_transition[MTYPE_PROGRAM][13][MTTYPE_STRUCT] = 16;
//	actions_on_state_transition[MTYPE_PROGRAM][13][MTTYPE_INT] = 17;
//	actions_on_state_transition[MTYPE_PROGRAM][13][MTTYPE_BOOLEAN] = 17;
//
//	actions_on_state_transition[MTYPE_PROGRAM][14][MTTYPE_IDENTIFIER] = 28;
//
//	actions_on_state_transition[MTYPE_PROGRAM][15][MTTYPE_IDENTIFIER] = 18;
//
//	actions_on_state_transition[MTYPE_PROGRAM][16][MTTYPE_IDENTIFIER] = 29;
//
//	actions_on_state_transition[MTYPE_PROGRAM][17][MTTYPE_IDENTIFIER] = 19;
//
//	actions_on_state_transition[MTYPE_PROGRAM][18][MTTYPE_LEFT_SQUARE_BRACKET] = 20;
//	actions_on_state_transition[MTYPE_PROGRAM][18][MTTYPE_COMMA] = 21;
//	actions_on_state_transition[MTYPE_PROGRAM][18][MTTYPE_SEMICOLON] = 7;
//
//	actions_on_state_transition[MTYPE_PROGRAM][19][MTTYPE_LEFT_SQUARE_BRACKET] = 22;
//	actions_on_state_transition[MTYPE_PROGRAM][19][MTTYPE_COMMA] = 23;
//	actions_on_state_transition[MTYPE_PROGRAM][19][MTTYPE_SEMICOLON] = 12;
//
//	actions_on_state_transition[MTYPE_PROGRAM][20][MTTYPE_NUMBER] = 24;
//
//	actions_on_state_transition[MTYPE_PROGRAM][21][MTTYPE_INT] = 15;
//	actions_on_state_transition[MTYPE_PROGRAM][21][MTTYPE_BOOLEAN] = 15;
//
//	actions_on_state_transition[MTYPE_PROGRAM][22][MTTYPE_NUMBER] = 25;
//
//	actions_on_state_transition[MTYPE_PROGRAM][23][MTTYPE_INT] = 17;
//	actions_on_state_transition[MTYPE_PROGRAM][23][MTTYPE_BOOLEAN] = 17;
//
//	actions_on_state_transition[MTYPE_PROGRAM][24][MTTYPE_RIGHT_SQUARE_BRACKET] = 26;
//
//	actions_on_state_transition[MTYPE_PROGRAM][25][MTTYPE_RIGHT_SQUARE_BRACKET] = 27;
//
//	actions_on_state_transition[MTYPE_PROGRAM][26][MTTYPE_COMMA] = 21;
//	actions_on_state_transition[MTYPE_PROGRAM][26][MTTYPE_SEMICOLON] = 7;
//
//	actions_on_state_transition[MTYPE_PROGRAM][27][MTTYPE_COMMA] = 23;
//	actions_on_state_transition[MTYPE_PROGRAM][27][MTTYPE_SEMICOLON] = 12;
//
//	actions_on_state_transition[MTYPE_PROGRAM][28][MTTYPE_IDENTIFIER] = 30;
//
//	actions_on_state_transition[MTYPE_PROGRAM][29][MTTYPE_IDENTIFIER] = 31;
//
//	actions_on_state_transition[MTYPE_PROGRAM][30][MTTYPE_COMMA] = 32;
//	actions_on_state_transition[MTYPE_PROGRAM][30][MTTYPE_SEMICOLON] = 7;
//
//	actions_on_state_transition[MTYPE_PROGRAM][31][MTTYPE_COMMA] = 33;
//	actions_on_state_transition[MTYPE_PROGRAM][31][MTTYPE_SEMICOLON] = 12;
//
//	actions_on_state_transition[MTYPE_PROGRAM][32][MTTYPE_STRUCT] = 14;
//
//	actions_on_state_transition[MTYPE_PROGRAM][33][MTTYPE_STRUCT] = 16;
//
//	actions_on_state_transition[MTYPE_PROGRAM][34][MTTYPE_IDENTIFIER] = 37;
//
//	actions_on_state_transition[MTYPE_PROGRAM][35][MTTYPE_IDENTIFIER] = 57;
//
//	actions_on_state_transition[MTYPE_PROGRAM][36][MTTYPE_IDENTIFIER] = 38;
//
//	actions_on_state_transition[MTYPE_PROGRAM][37][MTTYPE_LEFT_PARENTHESES] = 39;
//
//	actions_on_state_transition[MTYPE_PROGRAM][38][MTTYPE_LEFT_SQUARE_BRACKET] = 40;
//	actions_on_state_transition[MTYPE_PROGRAM][38][MTTYPE_COMMA] = 41;
//	actions_on_state_transition[MTYPE_PROGRAM][38][MTTYPE_SEMICOLON] = 42;
//
//	actions_on_state_transition[MTYPE_PROGRAM][39][MTTYPE_INT] = 43;
//	actions_on_state_transition[MTYPE_PROGRAM][39][MTTYPE_BOOLEAN] = 43;
//	actions_on_state_transition[MTYPE_PROGRAM][39][MTTYPE_RIGHT_PARENTHESES] = 44;
//
//	actions_on_state_transition[MTYPE_PROGRAM][40][MTTYPE_NUMBER] = 49;
//
//	actions_on_state_transition[MTYPE_PROGRAM][41][MTTYPE_INT] = 36;
//	actions_on_state_transition[MTYPE_PROGRAM][41][MTTYPE_BOOLEAN] = 36;
//
//	actions_on_state_transition[MTYPE_PROGRAM][42][MTTYPE_DECLARE] = 3;
//	actions_on_state_transition[MTYPE_PROGRAM][42][MTTYPE_FUNCTION] = 4;
//	actions_on_state_transition[MTYPE_PROGRAM][42][MTTYPE_MAIN] = 5;
//
//	actions_on_state_transition[MTYPE_PROGRAM][43][MTTYPE_IDENTIFIER] = 62;
//
//	actions_on_state_transition[MTYPE_PROGRAM][44][MTTYPE_LEFT_CURLY_BRACKET] = 45;
//
//	actions_on_state_transition[MTYPE_PROGRAM][45][MTTYPE_DECLARE] = 46;
//	actions_on_state_transition[MTYPE_PROGRAM][45][MTTYPE_RIGHT_CURLY_BRACKET] = 47;
//
//	actions_on_state_transition[MTYPE_PROGRAM][46][MTTYPE_STRUCT] = 50;
//	actions_on_state_transition[MTYPE_PROGRAM][46][MTTYPE_INT] = 51;
//	actions_on_state_transition[MTYPE_PROGRAM][46][MTTYPE_BOOLEAN] = 51;
//
//	actions_on_state_transition[MTYPE_PROGRAM][47][MTTYPE_FUNCTION] = 4;
//	actions_on_state_transition[MTYPE_PROGRAM][47][MTTYPE_MAIN] = 5;
//
//	actions_on_state_transition[MTYPE_PROGRAM][48][MTTYPE_RIGHT_CURLY_BRACKET] = 47;
//
//	actions_on_state_transition[MTYPE_PROGRAM][49][MTTYPE_RIGHT_SQUARE_BRACKET] = 52;
//
//	actions_on_state_transition[MTYPE_PROGRAM][50][MTTYPE_IDENTIFIER] = 61;
//
//	actions_on_state_transition[MTYPE_PROGRAM][51][MTTYPE_IDENTIFIER] = 53;
//
//	actions_on_state_transition[MTYPE_PROGRAM][52][MTTYPE_COMMA] = 41;
//	actions_on_state_transition[MTYPE_PROGRAM][52][MTTYPE_SEMICOLON] = 42;
//
//	actions_on_state_transition[MTYPE_PROGRAM][53][MTTYPE_LEFT_SQUARE_BRACKET] = 54;
//	actions_on_state_transition[MTYPE_PROGRAM][53][MTTYPE_COMMA] = 55;
//	actions_on_state_transition[MTYPE_PROGRAM][53][MTTYPE_SEMICOLON] = 45;
//
//	actions_on_state_transition[MTYPE_PROGRAM][54][MTTYPE_NUMBER] = 56;
//
//	actions_on_state_transition[MTYPE_PROGRAM][55][MTTYPE_INT] = 51;
//	actions_on_state_transition[MTYPE_PROGRAM][55][MTTYPE_BOOLEAN] = 51;
//
//	actions_on_state_transition[MTYPE_PROGRAM][56][MTTYPE_RIGHT_SQUARE_BRACKET] = 58;
//
//	actions_on_state_transition[MTYPE_PROGRAM][57][MTTYPE_IDENTIFIER] = 59;
//
//	actions_on_state_transition[MTYPE_PROGRAM][58][MTTYPE_COMMA] = 55;
//	actions_on_state_transition[MTYPE_PROGRAM][58][MTTYPE_SEMICOLON] = 45;
//
//	actions_on_state_transition[MTYPE_PROGRAM][59][MTTYPE_COMMA] = 60;
//	actions_on_state_transition[MTYPE_PROGRAM][59][MTTYPE_SEMICOLON] = 42;
//
//	actions_on_state_transition[MTYPE_PROGRAM][60][MTTYPE_STRUCT] = 35;
//
//	actions_on_state_transition[MTYPE_PROGRAM][61][MTTYPE_IDENTIFIER] = 63;
//
//	actions_on_state_transition[MTYPE_PROGRAM][62][MTTYPE_LEFT_SQUARE_BRACKET] = 64;
//	actions_on_state_transition[MTYPE_PROGRAM][62][MTTYPE_COMMA] = 65;
//	actions_on_state_transition[MTYPE_PROGRAM][62][MTTYPE_RIGHT_PARENTHESES] = 44;
//
//	actions_on_state_transition[MTYPE_PROGRAM][63][MTTYPE_COMMA] = 66;
//	actions_on_state_transition[MTYPE_PROGRAM][63][MTTYPE_SEMICOLON] = 45;
//
//	actions_on_state_transition[MTYPE_PROGRAM][64][MTTYPE_NUMBER] = 67;
//
//	actions_on_state_transition[MTYPE_PROGRAM][65][MTTYPE_INT] = 43;
//	actions_on_state_transition[MTYPE_PROGRAM][65][MTTYPE_BOOLEAN] = 43;
//
//	actions_on_state_transition[MTYPE_PROGRAM][66][MTTYPE_STRUCT] = 50;
//
//	actions_on_state_transition[MTYPE_PROGRAM][67][MTTYPE_RIGHT_SQUARE_BRACKET] = 68;
//
//	actions_on_state_transition[MTYPE_PROGRAM][68][MTTYPE_COMMA] = 65;
//	actions_on_state_transition[MTYPE_PROGRAM][68][MTTYPE_RIGHT_PARENTHESES] = 44;
//
//	/* machine call transitions */
//	actions_on_machine_transition[MTYPE_PROGRAM][7][MTYPE_COMMAND] = 11;
//	actions_on_machine_transition[MTYPE_PROGRAM][11][MTYPE_COMMAND] = 11;
//	actions_on_machine_transition[MTYPE_PROGRAM][45][MTYPE_COMMAND] = 48;
//	actions_on_machine_transition[MTYPE_PROGRAM][48][MTYPE_COMMAND] = 48;

	/***********************************************************/
	/* 						EXPRESSION						   */
	/***********************************************************/
	actions_on_state_transition[MTYPE_EXPRESSION][0][MTTYPE_TRUE]  = push_operand_true;
	actions_on_state_transition[MTYPE_EXPRESSION][0][MTTYPE_FALSE] = push_operand_false;
//	actions_on_state_transition[MTYPE_EXPRESSION][0][MTTYPE_NOT] = 2;
	actions_on_state_transition[MTYPE_EXPRESSION][0][MTTYPE_NUMBER] = push_operand;
	actions_on_state_transition[MTYPE_EXPRESSION][0][MTTYPE_IDENTIFIER] = push_identifier;
//
//	actions_on_state_transition[MTYPE_EXPRESSION][1][MTTYPE_GREATER_THAN] = 4;
//	actions_on_state_transition[MTYPE_EXPRESSION][1][MTTYPE_LESS_THAN] = 4;
//	actions_on_state_transition[MTYPE_EXPRESSION][1][MTTYPE_EQUAL_EQUAL] = 4;
//	actions_on_state_transition[MTYPE_EXPRESSION][1][MTTYPE_AND] = 4;
//	actions_on_state_transition[MTYPE_EXPRESSION][1][MTTYPE_OR] = 4;
	actions_on_state_transition[MTYPE_EXPRESSION][1][MTTYPE_PLUS] = push_operator;
	actions_on_state_transition[MTYPE_EXPRESSION][1][MTTYPE_MINUS] = push_operator;
	actions_on_state_transition[MTYPE_EXPRESSION][1][MTTYPE_MULTIPLICATION] = push_operator;
	actions_on_state_transition[MTYPE_EXPRESSION][1][MTTYPE_DIVISION] = push_operator;
//
//	actions_on_state_transition[MTYPE_EXPRESSION][3][MTTYPE_LEFT_SQUARE_BRACKET] = 5;
//	actions_on_state_transition[MTYPE_EXPRESSION][3][MTTYPE_DOT] = 6;
//	actions_on_state_transition[MTYPE_EXPRESSION][3][MTTYPE_LEFT_PARENTHESES] = 7;
//	actions_on_state_transition[MTYPE_EXPRESSION][3][MTTYPE_GREATER_THAN] = 4;
//	actions_on_state_transition[MTYPE_EXPRESSION][3][MTTYPE_LESS_THAN] = 4;
//	actions_on_state_transition[MTYPE_EXPRESSION][3][MTTYPE_EQUAL_EQUAL] = 4;
//	actions_on_state_transition[MTYPE_EXPRESSION][3][MTTYPE_AND] = 4;
//	actions_on_state_transition[MTYPE_EXPRESSION][3][MTTYPE_OR] = 4;
	actions_on_state_transition[MTYPE_EXPRESSION][3][MTTYPE_PLUS] = push_operator;
	actions_on_state_transition[MTYPE_EXPRESSION][3][MTTYPE_MINUS] = push_operator;
	actions_on_state_transition[MTYPE_EXPRESSION][3][MTTYPE_MULTIPLICATION] = push_operator;
	actions_on_state_transition[MTYPE_EXPRESSION][3][MTTYPE_DIVISION] = push_operator;
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

	actions_on_machine_return[MTYPE_EXPRESSION][1] = expression_end;
	actions_on_machine_return[MTYPE_EXPRESSION][3] = expression_end;
	actions_on_machine_return[MTYPE_EXPRESSION][8] = expression_end;
}
