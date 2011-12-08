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
#include "spa.h"

#define ERR_VARIABLE_UNDECLARED 1
#define ERR_VARIABLE_REDECLARED 2
#define	ERR_UNSUPORTED_TYPE 3
#define	ERR_FUNCTION_OVERIDING 4
#define ERR_PARAMETER_REDECLARED 5
#define ERR_UNDECLARED_FUNCTION 6

char buffer[100];

static int loop_counter 		= 0;

static char * operand1 = NULL;
static char * operand2 = NULL;
static char * operator1 = NULL;

Stack * command_operator_stack;
Stack * command_operand_stack;


void dummy_semantic_action(Token * token) {
	// DOES NOTHING
	printf("TODO\n");
}

char * get_loop_label() {
	char * loop = (char *)malloc(10*sizeof(char));
	sprintf(loop, "L%d", loop_counter);
	loop_counter++;

	return loop;
}

void end_program() {
	sprintf(buffer, "\t\tret \n\t\t } // end of method Program::Main \n  } // end of class ConsoleApplication2.Program\n");
	write_to_code(buffer);
}

void push_control_command(Token *token) {

}

void push_command_operand(Token *token) {

}

void resolve_while() {

}

void resolve_end_while() {

}

void resolve_command(Token *token) {

}

char * get_mvn_operator(char * operator) {
	if(strcmp(operator, "+") == 0) return "+ ";
	if(strcmp(operator, "-") == 0) return "- ";
	if(strcmp(operator, "*") == 0) return "* ";
	/*if(strcmp(operator, "/") == 0)*/ return "/ ";
}

void resolve_compare_greater_than() {

}

void resolve_compare_less_than() {

}

// X o Y
// o is the top of the operator stack
// X is the second on the operand stack
// Y is the top on the operand stack
void resolve_arithmetic(char * o) {

}

int is_identifier(char * string) {
	char c = string[0];
	if((c >= 'a' && c <= 'z') || c == '@') return 1;
	return 0;
}

void resolve_greater_than() {
	sprintf(buffer, "\t\tldloc  %s\n", operand2);
	write_to_code(buffer);

	if(is_identifier(operand1)) {
		sprintf(buffer, "\t\tldloc  %s\n", operand1);
		write_to_code(buffer);
		sprintf(buffer,"\t\tcallvirt instance object [mscorlib]System.Collections.Stack::Pop()\n");
		write_to_code(buffer);
	}
	else {
		sprintf(buffer, "\t\tldc.i4 %s\n\t\tbox [mscorlib]System.Int32 \n", operand1);
		write_to_code(buffer);
	}


	sprintf(buffer,"\t\tcallvirt instance void [mscorlib]System.Collections.Stack::Push(object)\n\n");
	write_to_code(buffer);
}

void resolve_less_than() {
	sprintf(buffer, "\t\tldloc  %s\n", operand1);
	write_to_code(buffer);

	if(is_identifier(operand2)) {
		sprintf(buffer, "\t\tldloc  %s\n", operand2);
		write_to_code(buffer);
		sprintf(buffer,"\t\tcallvirt instance object [mscorlib]System.Collections.Stack::Pop()\n");
		write_to_code(buffer);
	}
	else {
		sprintf(buffer, "\t\tldc.i4 %s\n\t\tbox [mscorlib]System.Int32 \n", operand2);
		write_to_code(buffer);
	}

	sprintf(buffer,"\t\tcallvirt instance void [mscorlib]System.Collections.Stack::Push(object)\n\n");
	write_to_code(buffer);
}

void resolve_plus() {
	sprintf(buffer, "\t\tldloc  %s\n", operand1);  //preload para push
	write_to_code(buffer);
	sprintf(buffer, "\t\tldloc  %s\n", operand1);
	write_to_code(buffer);
	sprintf(buffer,"\t\tcallvirt instance object [mscorlib]System.Collections.Stack::Peek()\n");
	write_to_code(buffer);
	if(is_identifier(operand2)) {
		sprintf(buffer, "\t\tldloc  %s\n", operand2);
		write_to_code(buffer);
		sprintf(buffer,"\t\tcallvirt instance object [mscorlib]System.Collections.Stack::Pop()\n");
		write_to_code(buffer);
	}
	else {
		sprintf(buffer, "\t\tldc.i4 %s\n\t\tbox [mscorlib]System.Int32 \n", operand2);
		write_to_code(buffer);
	}
	sprintf(buffer,"\t\tadd\n");
	write_to_code(buffer);
	sprintf(buffer,"\t\tcallvirt instance void [mscorlib]System.Collections.Stack::Push(object)\n\n");  //instance = operand1 preloaded
	write_to_code(buffer);
}

void resolve_minus() {
	sprintf(buffer, "\t\tldloc  %s\n", operand1);  //preload para push
	write_to_code(buffer);
	sprintf(buffer, "\t\tldloc  %s\n", operand1);
	write_to_code(buffer);
	sprintf(buffer,"\t\tcallvirt instance object [mscorlib]System.Collections.Stack::Peek()\n");
	write_to_code(buffer);
	if(is_identifier(operand2)) {
		sprintf(buffer, "\t\tldloc  %s\n", operand2);
		write_to_code(buffer);
		sprintf(buffer,"\t\tcallvirt instance object [mscorlib]System.Collections.Stack::Pop()\n");
		write_to_code(buffer);
	}
	else {
		sprintf(buffer, "\t\tldc.i4 %s\n\t\tbox [mscorlib]System.Int32 \n", operand2);
		write_to_code(buffer);
	}
	sprintf(buffer,"\t\tsub\n");
	write_to_code(buffer);
	sprintf(buffer,"\t\tcallvirt instance void [mscorlib]System.Collections.Stack::Push(object)\n\n");  //instance = operand1 preloaded
	write_to_code(buffer);
}

void resolve_operation_and_continue(Token *t) {
	if (strcmp(operator1,">") == 0) resolve_greater_than();
	if (strcmp(operator1,"<") == 0) resolve_less_than();
	if (strcmp(operator1,"+") == 0) resolve_plus();
	if (strcmp(operator1,"-") == 0) resolve_minus();
	if (strcmp(operator1,"?") == 0) resolve_question_mark(t);

	// special case
	if (strcmp(t->lexeme,"?") == 0) operator1 = "?";
	else operator1 = NULL;
	operand1 = operand2;
	operand2 = NULL;
}

void resolve_operation(Token *t) {
	if (strcmp(operator1,">") == 0) resolve_greater_than();
	if (strcmp(operator1,"<") == 0) resolve_less_than();
	if (strcmp(operator1,"+") == 0) resolve_plus();
	if (strcmp(operator1,"-") == 0) resolve_minus();
	if (strcmp(operator1,"?") == 0) resolve_question_mark(t);

	operand1 = NULL;
	operand2 = NULL;
	operator1 = NULL;
}

void save_operand(Token * token) {
	if (operator1 == NULL) operand1 = token->lexeme;
	else operand2 = token->lexeme;
}

void save_operator(Token *token) {
	operator1 = token->lexeme;
}

void expression_end(Token *token) {

}

void resolve_question_mark(Token *token) {
	sprintf(buffer, "\t\tldloc  %s\n", operand1);
	write_to_code(buffer);
    sprintf(buffer,"\t\tcallvirt instance object [mscorlib]System.Collections.Stack::Peek()\n");
    write_to_code(buffer);
    sprintf(buffer,"\t\tldc.i4 0\n");
    write_to_code(buffer);
    sprintf(buffer,"\t\tbox [mscorlib]System.Int32\n");
    write_to_code(buffer);
    sprintf(buffer,"\t\tbne.un _ENDCLEAR\n");
    write_to_code(buffer);
    sprintf(buffer,"\t\tldloc %s\n", operand1);
    write_to_code(buffer);
    sprintf(buffer,"\t\tcallvirt instance void [mscorlib]System.Collections.Stack::Clear()\n");
    write_to_code(buffer);
    sprintf(buffer,"\t\t_ENDCLEAR:\n");
    write_to_code(buffer);

    operand1 = NULL;
}

void init_semantic_actions() {

	command_operand_stack = empty_stack();
	command_operator_stack = empty_stack();

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
	/* transition table */
	actions_on_state_transition[MTYPE_PROGRAM][0][MTTYPE_ID] = save_operand;
	actions_on_state_transition[MTYPE_PROGRAM][0][MTTYPE_NUMBER] = save_operand;

	actions_on_state_transition[MTYPE_PROGRAM][1][MTTYPE_QUESTION_MARK] = resolve_question_mark;

	actions_on_state_transition[MTYPE_PROGRAM][3][MTTYPE_ID] = save_operand;
	actions_on_state_transition[MTYPE_PROGRAM][3][MTTYPE_NUMBER] = save_operand;

	/* machine call transitions */
//	program.machine_calls[0][MTYPE_LOOP] = 3;
//	program.machine_calls[1][MTYPE_OPERATION_RIGHT] = 3;
//	program.machine_calls[1][MTYPE_OPERATION_LEFT] = 3;
//	program.machine_calls[2][MTYPE_OPERATION_RIGHT] = 3;
//	program.machine_calls[3][MTYPE_LOOP] = 3;

	/***********************************************************/
	/* 					  OPERATION RIGHT					   */
	/***********************************************************/
	/* transition table */
	actions_on_state_transition[MTYPE_OPERATION_RIGHT][0][MTTYPE_GREATER_THAN] =  save_operator;
	actions_on_state_transition[MTYPE_OPERATION_RIGHT][1][MTTYPE_ID] =  save_operand;
	actions_on_state_transition[MTYPE_OPERATION_RIGHT][2][MTTYPE_QUESTION_MARK] =  resolve_operation_and_continue;
	/* machine call transitions */
	actions_on_machine_transition[MTYPE_OPERATION_RIGHT][2][MTYPE_OPERATION_RIGHT] = resolve_operation_and_continue;
	actions_on_machine_transition[MTYPE_OPERATION_RIGHT][2][MTYPE_OPERATION_LEFT] = resolve_operation_and_continue;
	/* machine return */
	actions_on_machine_return[MTYPE_OPERATION_RIGHT][2] = resolve_operation;
	/***********************************************************/
	/* 					  OPERATION LEFT					   */
	/***********************************************************/

}
