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

char buffer[100];

static int constant_counter 	= 0;
static int temp_counter 		= 0;
static int temp_label_counter	= 0;
static int variable_counter 	= 0;
static int loop_counter 		= 0;
static int if_counter			= 0;
static int else_counter			= 0;

static int addr_counter			= 256;

static int param_counter 		= 0;
Node * current_function = NULL;

Stack * operand_stack;
Stack * operator_stack;

Stack * command_operator_stack;
Stack * command_operand_stack;

void enter_new_scope() {
	enter_new_table_of_symbols();
}

void exit_current_scope() {
	exit_current_table_of_symbols();
}

void dummy_semantic_action(Token * token) {
	// DOES NOTHING
	printf("TODO\n");
}

char * get_constant_for_number(int number) {
	char buf[5];
	itoa(number, buf, 10);
	Node * constant = get_identifier_at_index(add_if_new_identifiers_table(buf));
	if (constant->label == NULL) {
		constant->label = (char *)malloc(10*sizeof(char));
		sprintf(constant->label, "K%d", constant_counter);
		constant_counter++;

		sprintf(buffer, "%s\t\t\tK  =%d\t\t; Declaracao de constante\n", constant->label, token->value);
		write_to_data(buffer);
	}
	return constant->label;
}

char * get_constant_label() {
	Node * constant = get_identifier_at_index(add_if_new_identifiers_table(token->lexeme));
	if (constant->label == NULL) {
		constant->label = (char *)malloc(10*sizeof(char));
		sprintf(constant->label, "K%d", constant_counter);
		constant_counter++;

		sprintf(buffer, "%s\t\t\tK  =%d\t\t; Declaracao de constante\n", constant->label, token->value);
		write_to_data(buffer);
	}
	return constant->label;
}

char * get_temp_label() {
	char * temp = (char *)malloc(10*sizeof(char));
	sprintf(temp, "T%d", temp_counter);
	temp_counter++;

	sprintf(buffer, "%s\t\t\tK  =%d\t\t; Declaracao de temporario\n", temp, 0);
	write_to_data(buffer);
	return temp;
}

char * get_temp_label_label() {
	char * temp = (char *)malloc(10*sizeof(char));
	sprintf(temp, "TL%d", temp_label_counter);
	temp_label_counter++;

	return temp;
}

char * get_variable_label() {
	char * var = (char *)malloc(10*sizeof(char));
	sprintf(var, "V%d", variable_counter);
	variable_counter++;

	sprintf(buffer, "%s\t\t\tK  =%d\t\t; Declaracao de variavel\n", var, 0);
	write_to_data(buffer);
	return var;
}

char * get_loop_label() {
	char * loop = (char *)malloc(10*sizeof(char));
	sprintf(loop, "L%d", loop_counter);
	loop_counter++;

	return loop;
}

char * get_if_label() {
	char * iff = (char *)malloc(10*sizeof(char));
	sprintf(iff, "I%d", if_counter);
	if_counter++;

	return iff;
}

char * get_else_label() {
	char * elsee = (char *)malloc(10*sizeof(char));
	sprintf(elsee, "E%d", else_counter);
	else_counter++;

	return elsee;
}

int is_in_function() {
	return !(current_function == NULL);
}

void print_load_variable_from_RA(int position) {
	sprintf(buffer, "\t\t\tLD  %s\t\t; Carrega variavel do RA\n", get_constant_label(position));
	write_to_code(buffer);

	sprintf(buffer, "\t\t\tMM  pos_param\t\t;\n");
	write_to_code(buffer);

	sprintf(buffer, "\t\t\tSC  load_ra_pos\t\t;\n");
	write_to_code(buffer);

	addr_counter = addr_counter + 6;
}

void print_store_variable_from_RA(int position) {
	sprintf(buffer, "\t\t\tLD  %s\t\t; Armazena acumulado na variavel do RA\n", get_constant_label(position));
	write_to_code(buffer);

	sprintf(buffer, "\t\t\tMM  pos_param\t\t;\n");
	write_to_code(buffer);

	sprintf(buffer, "\t\t\tSC  store_ra_pos\t\t;\n");
	write_to_code(buffer);

	addr_counter = addr_counter + 6;
}

/***********************************************************/
/* 					PROGRAM ACTIONS						   */
/***********************************************************/

void print_main(Token *token) {
	sprintf(buffer, "main\t\tJP  /0000\t\t;\n");
	write_to_code(buffer);
	addr_counter = addr_counter + 2;
}

void end_program(Token *token) {
	sprintf(buffer, "\t\t\tHM  /00\t\t;\n");
	write_to_code(buffer);
	sprintf(buffer, "\t\t\t#  P \t\t;\n\n");
	write_to_code(buffer);

	addr_counter = addr_counter + 4;
}

void declare_variable(Token *token) {
	Node *identifier = get_identifier_for_data_on_current_table(token->lexeme);
	if(identifier->wasDeclared) {
		char err[200];
		sprintf(err, "redeclaration of variable \"%s\" in function main", token->lexeme);
		throw_semantic_exception(ERR_VARIABLE_REDECLARED, err);
	} else {
		identifier->wasDeclared = 1;
	}
}

void declare_function(Token *token) {
	Node *function = get_identifier_for_data_on_current_table(token->lexeme);
	current_function = function;
	if(function->wasDeclared) {
		char err[200];
		sprintf(err, "overiding of function \"%s\" in same scope", token->lexeme);
		throw_semantic_exception(ERR_FUNCTION_OVERIDING, err);
	} else {
		sprintf(buffer, "; Function %s\n", token->lexeme);
		write_to_code(buffer);
		function->wasDeclared = 1;
		function->functionAddress = addr_counter;
		enter_new_scope();
	}
}

void declare_parameter(Token *token) {
	Node *parameter = get_identifier_for_data_on_current_table(token->lexeme);
	if(parameter->wasDeclared) {
		char err[200];
		sprintf(err, "redeclaration of parameter \"%s\" in function \"%s\"", token->lexeme, current_function->data);
		throw_semantic_exception(ERR_PARAMETER_REDECLARED, err);
	} else {
		parameter->wasDeclared = 1;
		parameter->raPosition = param_counter;
		param_counter++;
	}
}

void declare_function_variable(Token *token) {
	Node *variable = get_identifier_for_data_on_current_table(token->lexeme);
		if(variable->wasDeclared) {
			char err[200];
			sprintf(err, "redeclaration of variable \"%s\" in function \"%s\"", token->lexeme, current_function->data);
			throw_semantic_exception(ERR_VARIABLE_REDECLARED, err);
		} else {
			variable->wasDeclared = 1;
			variable->raPosition = param_counter;
			param_counter++;
		}
}

void end_function(Token *token) {

	print_load_variable_from_RA(current_function->parameterNumber);

	char * temp = get_temp_label();
	sprintf(buffer, "\t\t\tMM  %s\t\t;\n", temp);
    write_to_code(buffer);

	sprintf(buffer, "\t\t\tRS  %s\t\t;\n", temp);
    write_to_code(buffer);

	sprintf(buffer, "; End of function %s\n\n", current_function->data);
	write_to_code(buffer);

	current_function->parameterNumber = param_counter;
	current_function = NULL;
	exit_current_scope();
}

void throw_boolean_exception(Token *token) {
	throw_semantic_exception(ERR_UNSUPORTED_TYPE, "boolean type is currently unsupported by the compiler");
}

/***********************************************************/
/* 					COMMAND ACTIONS						   */
/***********************************************************/

void push_control_command(Token *token) {
	char * command = token->lexeme;
	char * label;

	if(strcmp(command, "while") == 0) {
		label = get_loop_label();
		sprintf(buffer, "%s\t\t\tLD  zero\t; Begin while loop\n", label);
		write_to_code(buffer);

		addr_counter = addr_counter + 2;
	} else if (strcmp(command, "if") == 0) {
		label = get_if_label();
		sprintf(buffer, "%s\t\t\tLD  zero\t; Begin if case\n", label);
		write_to_code(buffer);

		addr_counter = addr_counter + 2;
	} else if (strcmp(command, "else") == 0) {
		label = get_else_label();
	}

	stack_push(command_operator_stack, command);
	stack_push(command_operand_stack, label);
}

void push_command(Token *token) {
	stack_push(command_operator_stack, token->lexeme);
}

void push_command_operand(Token *token) {
	Node *variable = get_identifier_for_data(token->lexeme);
	if(variable->wasDeclared) {
		if(variable->label == NULL)
			variable->label = get_variable_label();
		stack_push(command_operand_stack, variable->label);
	} else {
		char err[200];
		sprintf(err, "trying to use undeclared variable \"%s\"", variable->data);
		throw_semantic_exception(ERR_VARIABLE_REDECLARED, err);
	}
}

void resolve_output() {
	if (is_in_function()) {
		print_load_variable_from_RA(get_identifier_for_label_on_current_table(stack_pop(command_operand_stack))->raPosition);
	} else {
		sprintf(buffer, "\t\t\tLD  %s\t\t\t\t; Comando de output\n", stack_pop(command_operand_stack));
		write_to_code(buffer);

		addr_counter = addr_counter + 2;
	}

	sprintf(buffer, "\t\t\tMM  output_number\t;\n");
	write_to_code(buffer);

	sprintf(buffer, "\t\t\tSC  output\t\t\t;\n");
	write_to_code(buffer);

	addr_counter = addr_counter + 4;
}

void resolve_input() {
	sprintf(buffer, "\t\t\tSC  input\t; Comando de input\n");
	write_to_code(buffer);

	addr_counter = addr_counter + 2;

	if (is_in_function()) print_store_variable_from_RA(get_identifier_for_label_on_current_table(stack_pop(command_operand_stack))->raPosition);
	else {
		sprintf (buffer, "\t\t\tMM  %s\t\t;\n", stack_pop(command_operand_stack));
		write_to_code(buffer);

		addr_counter = addr_counter + 2;
	}
}

void resolve_assign() {
	sprintf(buffer, "\t\t\tLD  %s\t\t; Atribuicao de variavel\n", stack_pop(operand_stack));
	write_to_code(buffer);

	addr_counter = addr_counter + 2;

	if (is_in_function()) print_store_variable_from_RA(get_identifier_for_label_on_current_table(stack_pop(command_operand_stack))->raPosition);
	else {
		sprintf(buffer, "\t\t\tMM  %s\t\t;\n", stack_pop(command_operand_stack));
		write_to_code(buffer);

		addr_counter = addr_counter + 2;
	}
}

void resolve_while() {
	char * label = stack_check(command_operand_stack);

	sprintf(buffer, "\t\t\tLD  %s\t\t;\n", stack_pop(operand_stack));
	write_to_code(buffer);

	sprintf(buffer, "\t\t\tJN  _%s\t\t;\n", label);
	write_to_code(buffer);

	sprintf(buffer, "\t\t\tJZ  _%s\t\t;\n", label);
	write_to_code(buffer);

	stack_push(command_operator_stack, "endwhile");
	enter_new_scope();

	addr_counter = addr_counter + 6;
}

void resolve_end_while() {
	char * label = stack_pop(command_operand_stack);

	sprintf(buffer, "\t\t\tJP  %s\t\t;\n", label);
	write_to_code(buffer);

	sprintf(buffer, "_%s\t\t\tLD  zero\t; End while loop\n", label);
	write_to_code(buffer);

	exit_current_scope();

	addr_counter = addr_counter + 4;
}

void resolve_if() {
	char * label = stack_check(command_operand_stack);

	sprintf(buffer, "\t\t\tLD  %s\t\t;\n", stack_pop(operand_stack));
	write_to_code(buffer);

	sprintf(buffer, "\t\t\tJN  _%s\t\t;\n", label);
	write_to_code(buffer);

	sprintf(buffer, "\t\t\tJZ  _%s\t\t;\n", label);
	write_to_code(buffer);

	stack_push(command_operator_stack, "endif");
	enter_new_scope();

	addr_counter = addr_counter + 4;
}

void resolve_end_if() {
	char * label = stack_pop(command_operand_stack);

	sprintf(buffer, "_%s\t\t\tLD  zero\t; End if case\n", label);
	write_to_code(buffer);

	exit_current_scope();

	addr_counter = addr_counter + 2;
}

void resolve_else() {
	char * else_label = stack_pop(command_operand_stack);
	char * end_if_label = stack_pop(command_operand_stack);
	stack_pop(command_operator_stack);

	sprintf(buffer, "\t\t\tJP  %s\t\t;\n", else_label);
	write_to_code(buffer);

	sprintf(buffer, "_%s\t\t\tLD  zero\t; End if case/Begin else case\n", end_if_label);
	write_to_code(buffer);

	stack_push(command_operand_stack, else_label);
	stack_push(command_operator_stack, "endelse");

	addr_counter = addr_counter + 4;
}

void resolve_end_else() {
	char * label = stack_pop(command_operand_stack);

	sprintf(buffer, "%s\t\t\tLD  zero\t; End else case\n", label);
	write_to_code(buffer);

	addr_counter = addr_counter + 2;
}

void resolve_return() {
	char * label = stack_pop(operand_stack);
	sprintf(buffer, "\t\t\tLD  %s\t\t; Returns value\n", label);
	write_to_code(buffer);

	addr_counter = addr_counter + 2;
}

void resolve_command(Token *token) {
	char * command = stack_pop(command_operator_stack);

	if(strcmp(command, "=") == 0) resolve_assign();
	else if(strcmp(command, "output") == 0) resolve_output();
	else if(strcmp(command, "input") == 0) resolve_input();
	else if(strcmp(command, "while") == 0) resolve_while();
	else if(strcmp(command, "endwhile") == 0) resolve_end_while();
	else if(strcmp(command, "if") == 0) resolve_if();
	else if(strcmp(command, "endif") == 0) resolve_end_if();
	else if(strcmp(command, "else") == 0) resolve_else();
	else if(strcmp(command, "endelse") == 0) resolve_end_else();
	else if(strcmp(command, "return") == 0) resolve_return();
}


/***********************************************************/
/* 					EXPRESSION	ACTIONS					   */
/***********************************************************/

int operator_precedence(char * operator) {
	if(operator != NULL) {
		if(strcmp(operator, "+") == 0) return 2;
		if(strcmp(operator, "-") == 0) return 2;
		if(strcmp(operator, "*") == 0) return 3;
		if(strcmp(operator, "/") == 0) return 3;
		if(strcmp(operator, ">") == 0) return 1;
		if(strcmp(operator, "<") == 0) return 1;
	}
	return -1;
}

char * get_mvn_operator(char * operator) {
	if(strcmp(operator, "+") == 0) return "+ ";
	if(strcmp(operator, "-") == 0) return "- ";
	if(strcmp(operator, "*") == 0) return "* ";
	/*if(strcmp(operator, "/") == 0)*/ return "/ ";
}

void resolve_compare_greater_than() {
	char * Y = stack_pop(operand_stack);
	char * X = stack_pop(operand_stack);

	sprintf(buffer, "\t\t\tLD  %s\t\t; Comparacao X > Y\n", X);
	write_to_code(buffer);

	sprintf(buffer, "\t\t\t-   %s\t\t;\n", Y);
	write_to_code(buffer);

	char * temp = get_temp_label();
	sprintf(buffer, "\t\t\tMM  %s\t\t;\n", temp);
	write_to_code(buffer);

	stack_push(operand_stack, temp);

	addr_counter = addr_counter + 6;
}

void resolve_compare_less_than() {
	char * Y = stack_pop(operand_stack);
	char * X = stack_pop(operand_stack);

	sprintf(buffer, "\t\t\tLD  %s\t\t; Comparacao X < Y\n", Y);
	write_to_code(buffer);

	sprintf(buffer, "\t\t\t-  %s\t\t;\n", X);
	write_to_code(buffer);

	char * temp = get_temp_label();
	sprintf(buffer, "\t\t\tMM  %s\t\t;\n", temp);
	write_to_code(buffer);

	stack_push(operand_stack, temp);

	addr_counter = addr_counter + 6;
}

void resolve_compare_equal_equal() {
	char * Y = stack_pop(operand_stack);
	char * X = stack_pop(operand_stack);

	sprintf(buffer, "\t\t\tLD  %s\t\t; Comparacao X == Y\n", X);
	write_to_code(buffer);

	sprintf(buffer, "\t\t\t-  %s\t\t;\n", Y);
	write_to_code(buffer);

	char * is_equal = get_temp_label_label();
	sprintf(buffer, "\t\t\tJZ  %s\t\t;\n", is_equal);
	write_to_code(buffer);

	sprintf(buffer, "\t\t\tLD  zero\t; Nao e igual\n");
	write_to_code(buffer);

	char * is_not_equal = get_temp_label_label();
	sprintf(buffer, "\t\t\tJP  %s\t\t;\n", is_not_equal);
	write_to_code(buffer);

	sprintf(buffer, "%s\t\t\tLD  one\t\t; E igual\n", is_equal);
	write_to_code(buffer);

	char * temp = get_temp_label();
	sprintf(buffer, "%s\t\t\tMM  %s\t\t;\n", is_not_equal, temp);
	write_to_code(buffer);

	stack_push(operand_stack, temp);

	addr_counter = addr_counter + 14;
}

void resolve_logic_and() {
	char * X = stack_pop(operand_stack);

	sprintf(buffer, "\t\t\tLD  %s\t\t; Comeco do and logico\n", X);
	write_to_code(buffer);

	char * temp = get_temp_label_label();
	sprintf(buffer, "\t\t\tJZ  %s\t\t; Returns NO\n", temp);
	write_to_code(buffer);

	sprintf(buffer, "\t\t\tJN  %s\t\t; Returns NO\n", temp);
	write_to_code(buffer);

	stack_push(operand_stack, temp);
	stack_push(operator_stack, "_and");

	addr_counter = addr_counter + 6;
}

void end_logic_and() {
	char * Y = stack_pop(operand_stack);
	char * no_label = stack_pop(operand_stack);

	sprintf(buffer, "\t\t\tLD  %s\t\t;\n", Y);
	write_to_code(buffer);

	sprintf(buffer, "\t\t\tJZ  %s\t\t; Returns NO\n", no_label);
	write_to_code(buffer);

	sprintf(buffer, "\t\t\tJN  %s\t\t; Returns NO\n", no_label);
	write_to_code(buffer);

	sprintf(buffer, "\t\t\tLD  one\t\t;\n");
	write_to_code(buffer);

	char * yes_label = get_temp_label_label();
	sprintf(buffer, "\t\t\tJP  %s\t\t; Returns YES\n", yes_label);
	write_to_code(buffer);

	sprintf(buffer, "%s\t\t\tLD zero\t;\n", no_label);
	write_to_code(buffer);

	char * temp = get_temp_label();
	sprintf(buffer, "%s\t\t\tMM  %s\t\t; Fim do and logico\n", yes_label, temp);
	write_to_code(buffer);

	stack_push(operand_stack, temp);

	addr_counter = addr_counter + 14;
}

void resolve_logic_or() {
	char * X = stack_pop(operand_stack);

	sprintf(buffer, "\t\t\tLD  %s\t\t; Comeco do or logico\n", X);
	write_to_code(buffer);

	char * temp = get_temp_label_label();
	sprintf(buffer, "\t\t\tJZ  %s\t\t;\n", temp);
	write_to_code(buffer);

	sprintf(buffer, "\t\t\tJN  %s\t\t;\n", temp);
	write_to_code(buffer);

	char * yes_label = get_temp_label_label();
	sprintf(buffer, "\t\t\tJP  %s\t\t; Returns YES\n", yes_label);
	write_to_code(buffer);

	stack_push(operand_stack, yes_label);
	stack_push(operand_stack, temp);
	stack_push(operator_stack, "_or");

	addr_counter = addr_counter + 8;
}

void end_logic_or() {
	char * Y = stack_pop(operand_stack);
	char * temp = stack_pop(operand_stack);
	char * yes_label = stack_pop(operand_stack);

	sprintf(buffer, "%s\t\t\tLD  %s\t\t;\n", temp, Y);
	write_to_code(buffer);

	char * temp2 = get_temp_label_label();
	sprintf(buffer, "\t\t\tJZ  %s\t\t;\n", temp2);
	write_to_code(buffer);

	sprintf(buffer, "\t\t\tJN  %s\t\t;\n", temp2);
	write_to_code(buffer);

	sprintf(buffer, "\t\t\tJP  %s\t\t; Returns YES\n", yes_label);
	write_to_code(buffer);

	sprintf(buffer, "%s\t\t\tLD  zero\t;\n", temp2);
	write_to_code(buffer);

	char * no_label = get_temp_label_label();
	sprintf(buffer, "\t\t\tJP  %s\t\t; Returns NO\n", no_label);
	write_to_code(buffer);

	sprintf(buffer, "%s\t\t\tLD one\t\t;\n", yes_label);
	write_to_code(buffer);

	char * temp3 = get_temp_label();
	sprintf(buffer, "%s\t\t\tMM  %s\t\t; Fim do or logico\n", no_label, temp3);
	write_to_code(buffer);

	stack_push(operand_stack, temp3);

	addr_counter = addr_counter + 16;
}

void resolve_logic_not() {
	char * Y = stack_pop(operand_stack);

	sprintf(buffer, "\t\t\tLD  %s\t\t; Comeco do not logico\n", Y);
	write_to_code(buffer);

	char * temp = get_temp_label_label();
	sprintf(buffer, "\t\t\tJZ  %s\t\t; Returns NO\n", temp);
	write_to_code(buffer);

	sprintf(buffer, "\t\t\tJN  %s\t\t; Returns NO\n", temp);
	write_to_code(buffer);

	sprintf(buffer, "\t\t\tLD  one\t\t;\n");
	write_to_code(buffer);

	char * temp2 = get_temp_label_label();
	sprintf(buffer, "\t\t\tJP  %s\t\t; Returns YES\n", temp2);
	write_to_code(buffer);

	sprintf(buffer, "%s\t\t\tLD zero\t\t;\n", temp);
	write_to_code(buffer);

	char * temp3 = get_temp_label();
	sprintf(buffer,"%s\t\t\tMM  %s\t\t; Fim do not logico\n",temp2, temp3);
	write_to_code(buffer);

	stack_push(operand_stack, temp3);

	addr_counter = addr_counter + 14;
}

// X o Y
// o is the top of the operator stack
// X is the second on the operand stack
// Y is the top on the operand stack
void resolve_arithmetic(char * o) {
	char * Y = stack_pop(operand_stack);
	o = get_mvn_operator(o);
	char * X = stack_pop(operand_stack);

	sprintf(buffer, "\t\t\tLD  %s\t\t;\n", X);
	write_to_code(buffer);

	sprintf(buffer, "\t\t\t%s  %s\t\t;\n", o, Y);
	write_to_code(buffer);

	char * temp = get_temp_label();
	sprintf(buffer, "\t\t\tMM  %s\t\t;\n", temp);
	write_to_code(buffer);

	stack_push(operand_stack, temp);

	addr_counter = addr_counter + 6;
}

void resolve_expression() {
	char * o = stack_pop(operator_stack);
	if(strcmp(o, ">") == 0) resolve_compare_greater_than();
	else if(strcmp(o, "<") == 0) resolve_compare_less_than();
	else if(strcmp(o, "==") == 0) resolve_compare_equal_equal();
	else if(strcmp(o, "and") == 0) resolve_logic_and();
	else if(strcmp(o, "_and") == 0) end_logic_and();
	else if(strcmp(o, "or") == 0) resolve_logic_or();
	else if(strcmp(o, "_or") == 0) end_logic_or();
	else if(strcmp(o, "not") == 0) resolve_logic_not();
	else resolve_arithmetic(o);
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
		if (strcmp(token->lexeme, "and") == 0) resolve_expression();
		if (strcmp(token->lexeme, "or") == 0) resolve_expression();
	}
}

void push_identifier(Token *token) {
	Node *identifier = get_identifier_for_data(token->lexeme);
	if(identifier->wasDeclared) {
		if(identifier->label == NULL)
			identifier->label = get_variable_label();
		stack_push(operand_stack, identifier->label);
	} else {
		char err[200];
		sprintf(err, "trying to use undeclared variable \"%s\"", token->lexeme);
		throw_semantic_exception(ERR_VARIABLE_REDECLARED, err);
	}
}

void expression_end(Token *token) {
	if(!stack_is_empty(operator_stack)) {
		resolve_expression();
		expression_end(token);
	}
}

void function_call(Token *token) {
	Node * function = get_identifier_for_label_on_current_table(stack_pop(operand_stack));

	addr_counter = addr_counter + 12;

	char * paramNumber = get_constant_for_number(function->parameterNumber);
	sprintf(buffer, "\t\t\tLD  %s\t\t; Cria registro de ativacao\n", paramNumber);
	write_to_code(buffer);

	sprintf(buffer, "\t\t\tMM ra_tam\t\t; \n");
	write_to_code(buffer);

	char * addressNumber = get_constant_for_number(addr_counter);
	sprintf(buffer, "\t\t\tLD  %s\t\t;\n", addressNumber);
	write_to_code(buffer);

	sprintf(buffer, "\t\t\tMM  ra_end\t\t;\n");
    write_to_code(buffer);

    sprintf(buffer, "\t\t\tSC  cria_ra\t;\n");
    write_to_code(buffer);

    char * functionAddress = get_constant_for_number(function->functionAddress);
    sprintf(buffer, "\t\t\tSC  %s\t\t; Chama funcao\n", functionAddress);
    write_to_code(buffer);
}

void init_semantic_actions() {

	operand_stack = empty_stack();
	operator_stack = empty_stack();

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
	actions_on_state_transition[MTYPE_PROGRAM][3][MTTYPE_BOOLEAN] = throw_boolean_exception;
//
//	actions_on_state_transition[MTYPE_PROGRAM][4][MTTYPE_INT] = 34;
	actions_on_state_transition[MTYPE_PROGRAM][4][MTTYPE_BOOLEAN] = throw_boolean_exception;
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
	actions_on_state_transition[MTYPE_PROGRAM][9][MTTYPE_BOOLEAN] = throw_boolean_exception;
//
	actions_on_state_transition[MTYPE_PROGRAM][11][MTTYPE_RIGHT_CURLY_BRACKET] = end_program;
//
//	actions_on_state_transition[MTYPE_PROGRAM][12][MTTYPE_DECLARE] = 13;
//	actions_on_state_transition[MTYPE_PROGRAM][12][MTTYPE_RIGHT_CURLY_BRACKET] = 1;
//
//	actions_on_state_transition[MTYPE_PROGRAM][13][MTTYPE_STRUCT] = 16;
//	actions_on_state_transition[MTYPE_PROGRAM][13][MTTYPE_INT] = 17;
	actions_on_state_transition[MTYPE_PROGRAM][13][MTTYPE_BOOLEAN] = throw_boolean_exception;
//
//	actions_on_state_transition[MTYPE_PROGRAM][14][MTTYPE_IDENTIFIER] = 28;
//
	actions_on_state_transition[MTYPE_PROGRAM][15][MTTYPE_IDENTIFIER] = declare_variable;
//
//	actions_on_state_transition[MTYPE_PROGRAM][16][MTTYPE_IDENTIFIER] = 29;
//
	actions_on_state_transition[MTYPE_PROGRAM][17][MTTYPE_IDENTIFIER] = declare_variable;
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
	actions_on_state_transition[MTYPE_PROGRAM][21][MTTYPE_BOOLEAN] = throw_boolean_exception;
//
//	actions_on_state_transition[MTYPE_PROGRAM][22][MTTYPE_NUMBER] = 25;
//
//	actions_on_state_transition[MTYPE_PROGRAM][23][MTTYPE_INT] = 17;
	actions_on_state_transition[MTYPE_PROGRAM][23][MTTYPE_BOOLEAN] = throw_boolean_exception;
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
	actions_on_state_transition[MTYPE_PROGRAM][34][MTTYPE_IDENTIFIER] = declare_function;
//
//	actions_on_state_transition[MTYPE_PROGRAM][35][MTTYPE_IDENTIFIER] = 57;
//
	actions_on_state_transition[MTYPE_PROGRAM][36][MTTYPE_IDENTIFIER] = declare_variable;
//
//	actions_on_state_transition[MTYPE_PROGRAM][37][MTTYPE_LEFT_PARENTHESES] = 39;
//
//	actions_on_state_transition[MTYPE_PROGRAM][38][MTTYPE_LEFT_SQUARE_BRACKET] = 40;
//	actions_on_state_transition[MTYPE_PROGRAM][38][MTTYPE_COMMA] = 41;
//	actions_on_state_transition[MTYPE_PROGRAM][38][MTTYPE_SEMICOLON] = 42;
//
//	actions_on_state_transition[MTYPE_PROGRAM][39][MTTYPE_INT] = 43;
	actions_on_state_transition[MTYPE_PROGRAM][39][MTTYPE_BOOLEAN] = throw_boolean_exception;
//	actions_on_state_transition[MTYPE_PROGRAM][39][MTTYPE_RIGHT_PARENTHESES] = 44;
//
//	actions_on_state_transition[MTYPE_PROGRAM][40][MTTYPE_NUMBER] = 49;
//
//	actions_on_state_transition[MTYPE_PROGRAM][41][MTTYPE_INT] = 36;
	actions_on_state_transition[MTYPE_PROGRAM][41][MTTYPE_BOOLEAN] = throw_boolean_exception;
//
//	actions_on_state_transition[MTYPE_PROGRAM][42][MTTYPE_DECLARE] = 3;
//	actions_on_state_transition[MTYPE_PROGRAM][42][MTTYPE_FUNCTION] = 4;
	actions_on_state_transition[MTYPE_PROGRAM][42][MTTYPE_MAIN] = print_main;
//
	actions_on_state_transition[MTYPE_PROGRAM][43][MTTYPE_IDENTIFIER] = declare_parameter;
//
//	actions_on_state_transition[MTYPE_PROGRAM][44][MTTYPE_LEFT_CURLY_BRACKET] = 45;
//
//	actions_on_state_transition[MTYPE_PROGRAM][45][MTTYPE_DECLARE] = 46;
	actions_on_state_transition[MTYPE_PROGRAM][45][MTTYPE_RIGHT_CURLY_BRACKET] = end_function;
//
//	actions_on_state_transition[MTYPE_PROGRAM][46][MTTYPE_STRUCT] = 50;
//	actions_on_state_transition[MTYPE_PROGRAM][46][MTTYPE_INT] = 51;
	actions_on_state_transition[MTYPE_PROGRAM][46][MTTYPE_BOOLEAN] = throw_boolean_exception;
//
//	actions_on_state_transition[MTYPE_PROGRAM][47][MTTYPE_FUNCTION] = 4;
	actions_on_state_transition[MTYPE_PROGRAM][47][MTTYPE_MAIN] = print_main;
//
	actions_on_state_transition[MTYPE_PROGRAM][48][MTTYPE_RIGHT_CURLY_BRACKET] = end_function;
//
//	actions_on_state_transition[MTYPE_PROGRAM][49][MTTYPE_RIGHT_SQUARE_BRACKET] = 52;
//
//	actions_on_state_transition[MTYPE_PROGRAM][50][MTTYPE_IDENTIFIER] = 61;
//
	actions_on_state_transition[MTYPE_PROGRAM][51][MTTYPE_IDENTIFIER] = declare_function_variable;
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
	actions_on_state_transition[MTYPE_PROGRAM][55][MTTYPE_BOOLEAN] = throw_boolean_exception;
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
	actions_on_state_transition[MTYPE_PROGRAM][65][MTTYPE_BOOLEAN] = throw_boolean_exception;
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
	/* 						COMMAND							   */
	/***********************************************************/

	actions_on_state_transition[MTYPE_COMMAND][0][MTTYPE_IDENTIFIER] = push_command_operand;
	actions_on_state_transition[MTYPE_COMMAND][0][MTTYPE_IF] = push_control_command;
	actions_on_state_transition[MTYPE_COMMAND][0][MTTYPE_WHILE] = push_control_command;
	actions_on_state_transition[MTYPE_COMMAND][0][MTTYPE_INPUT] = push_command;
	actions_on_state_transition[MTYPE_COMMAND][0][MTTYPE_OUTPUT] = push_command;
	actions_on_state_transition[MTYPE_COMMAND][0][MTTYPE_RETURN] = push_command;
//
	actions_on_state_transition[MTYPE_COMMAND][1][MTTYPE_EQUAL] = push_command;
//	actions_on_state_transition[MTYPE_COMMAND][1][MTTYPE_LEFT_SQUARE_BRACKET] = 7;
//	actions_on_state_transition[MTYPE_COMMAND][1][MTTYPE_LEFT_PARENTHESES] = 8;
//
//	actions_on_state_transition[MTYPE_COMMAND][2][MTTYPE_LEFT_PARENTHESES] = 21;
//
//	actions_on_state_transition[MTYPE_COMMAND][3][MTTYPE_LEFT_PARENTHESES] = 15;
//
	actions_on_state_transition[MTYPE_COMMAND][4][MTTYPE_IDENTIFIER] = push_command_operand;
//
//	actions_on_state_transition[MTYPE_COMMAND][6][MTTYPE_LEFT_CURLY_BRACKET] = 18;
//
//	actions_on_state_transition[MTYPE_COMMAND][7][MTTYPE_NUMBER] = 13;
//
//	actions_on_state_transition[MTYPE_COMMAND][8][MTTYPE_RIGHT_PARENTHESES] = 9;
//
//	actions_on_state_transition[MTYPE_COMMAND][9][MTTYPE_SEMICOLON] = resolve_command;
//
//	actions_on_state_transition[MTYPE_COMMAND][10][MTTYPE_COMMA] = 12;
//	actions_on_state_transition[MTYPE_COMMAND][10][MTTYPE_RIGHT_PARENTHESES] = 9;
//
//	actions_on_state_transition[MTYPE_COMMAND][13][MTTYPE_RIGHT_SQUARE_BRACKET] = 14;
//
//	actions_on_state_transition[MTYPE_COMMAND][14][MTTYPE_EQUAL] = 5;
//
//	actions_on_state_transition[MTYPE_COMMAND][16][MTTYPE_RIGHT_PARENTHESES] = 17;

	actions_on_state_transition[MTYPE_COMMAND][17][MTTYPE_LEFT_CURLY_BRACKET] = resolve_command;

//	actions_on_state_transition[MTYPE_COMMAND][19][MTTYPE_RIGHT_CURLY_BRACKET] = 11;
//
//	actions_on_state_transition[MTYPE_COMMAND][20][MTTYPE_COMMA] = 18;
//	actions_on_state_transition[MTYPE_COMMAND][20][MTTYPE_RIGHT_CURLY_BRACKET] = 9;
//
//	actions_on_state_transition[MTYPE_COMMAND][22][MTTYPE_RIGHT_PARENTHESES] = 23;

	actions_on_state_transition[MTYPE_COMMAND][23][MTTYPE_LEFT_CURLY_BRACKET] = resolve_command;

//	actions_on_state_transition[MTYPE_COMMAND][24][MTTYPE_RIGHT_CURLY_BRACKET] = 25;
//
	actions_on_state_transition[MTYPE_COMMAND][25][MTTYPE_ELSE] = push_control_command;
//
//	actions_on_machine_transition[MTYPE_COMMAND][5][MTYPE_EXPRESSION] = 9;
//	actions_on_machine_transition[MTYPE_COMMAND][6][MTYPE_EXPRESSION] = 9;
//	actions_on_machine_transition[MTYPE_COMMAND][8][MTYPE_EXPRESSION] = 10;
//	actions_on_machine_transition[MTYPE_COMMAND][12][MTYPE_EXPRESSION] = 10;
//	actions_on_machine_transition[MTYPE_COMMAND][15][MTYPE_EXPRESSION] = 16;
//	actions_on_machine_transition[MTYPE_COMMAND][18][MTYPE_EXPRESSION] = 20;
//	actions_on_machine_transition[MTYPE_COMMAND][19][MTYPE_COMMAND] = 19;
//	actions_on_machine_transition[MTYPE_COMMAND][21][MTYPE_EXPRESSION] = 22;
//	actions_on_machine_transition[MTYPE_COMMAND][24][MTYPE_COMMAND] = 24;

	actions_on_machine_return[MTYPE_COMMAND][11] = resolve_command;
	actions_on_machine_return[MTYPE_COMMAND][25] = resolve_command;

	/***********************************************************/
	/* 						EXPRESSION						   */
	/***********************************************************/
	actions_on_state_transition[MTYPE_EXPRESSION][0][MTTYPE_TRUE]  = push_operand_true;
	actions_on_state_transition[MTYPE_EXPRESSION][0][MTTYPE_FALSE] = push_operand_false;
	actions_on_state_transition[MTYPE_EXPRESSION][0][MTTYPE_NOT] = push_operator;
	actions_on_state_transition[MTYPE_EXPRESSION][0][MTTYPE_NUMBER] = push_operand;
	actions_on_state_transition[MTYPE_EXPRESSION][0][MTTYPE_IDENTIFIER] = push_identifier;

	actions_on_state_transition[MTYPE_EXPRESSION][1][MTTYPE_GREATER_THAN] = push_operator;
	actions_on_state_transition[MTYPE_EXPRESSION][1][MTTYPE_LESS_THAN] = push_operator;
	actions_on_state_transition[MTYPE_EXPRESSION][1][MTTYPE_EQUAL_EQUAL] = push_operator;
	actions_on_state_transition[MTYPE_EXPRESSION][1][MTTYPE_AND] = push_operator;
	actions_on_state_transition[MTYPE_EXPRESSION][1][MTTYPE_OR] = push_operator;
	actions_on_state_transition[MTYPE_EXPRESSION][1][MTTYPE_PLUS] = push_operator;
	actions_on_state_transition[MTYPE_EXPRESSION][1][MTTYPE_MINUS] = push_operator;
	actions_on_state_transition[MTYPE_EXPRESSION][1][MTTYPE_MULTIPLICATION] = push_operator;
	actions_on_state_transition[MTYPE_EXPRESSION][1][MTTYPE_DIVISION] = push_operator;

//	actions_on_state_transition[MTYPE_EXPRESSION][3][MTTYPE_LEFT_SQUARE_BRACKET] = 5;
//	actions_on_state_transition[MTYPE_EXPRESSION][3][MTTYPE_DOT] = 6;
	actions_on_state_transition[MTYPE_EXPRESSION][3][MTTYPE_LEFT_PARENTHESES] = function_call;
	actions_on_state_transition[MTYPE_EXPRESSION][3][MTTYPE_GREATER_THAN] = push_operator;
	actions_on_state_transition[MTYPE_EXPRESSION][3][MTTYPE_LESS_THAN] = push_operator;
	actions_on_state_transition[MTYPE_EXPRESSION][3][MTTYPE_EQUAL_EQUAL] = push_operator;
	actions_on_state_transition[MTYPE_EXPRESSION][3][MTTYPE_AND] = push_operator;
	actions_on_state_transition[MTYPE_EXPRESSION][3][MTTYPE_OR] = push_operator;
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
