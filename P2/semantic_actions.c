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

static int constant_counter 	= 0;
static int temp_counter 		= 0;
static int temp_label_counter	= 0;
static int variable_counter 	= 0;
static int loop_counter 		= 0;
static int if_counter			= 0;
static int else_counter			= 0;
static int func_counter			= 0;

static int addr_counter			= 512;

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
	char * buf = malloc(5*sizeof(char));
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
char * get_func_label() {
	char * func = (char *)malloc(10*sizeof(char));
	sprintf(func, "F%d", func_counter);
	func_counter++;

	return func;
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

		char * label = get_func_label();
		sprintf(buffer, "%s\t\t\tJP  /0000\t\t;\n", label);
		write_to_code(buffer);

		function->wasDeclared = 1;
		function->label = label;
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

	sprintf(buffer, "\t\t\t+  return_inst\t\t;\n");
    write_to_code(buffer);

    sprintf(buffer, "\t\t\tMM  _%s\t\t;\n", current_function->label);
    write_to_code(buffer);

	sprintf(buffer, "_%s\t\t\tK  /0000\t\t; Guarda o endereço de retorno\n", current_function->label);
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

	stack_push(command_operator_stack, command, 0);
	stack_push(command_operand_stack, label, 0);
}

void push_command(Token *token) {
	stack_push(command_operator_stack, token->lexeme, 0);
}

void push_command_operand(Token *token) {
	Node *variable = get_identifier_for_data(token->lexeme);
	if(variable->wasDeclared) {
		if(variable->label == NULL)
			variable->label = get_variable_label();
		stack_push(command_operand_stack, variable->label, is_in_function());
	} else {
		char err[200];
		sprintf(err, "trying to use undeclared variable \"%s\"", variable->data);
		throw_semantic_exception(ERR_VARIABLE_REDECLARED, err);
	}
}

void resolve_output() {
	StackNode * identifier = stack_pop(command_operand_stack);
	if (identifier->functionFlag) {
		print_load_variable_from_RA(get_identifier_for_label_on_current_table(identifier->content)->raPosition);
	} else {
		sprintf(buffer, "\t\t\tLD  %s\t\t\t\t; Comando de output\n", identifier->content);
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

	StackNode *identifier = stack_pop(command_operand_stack);
	if (identifier->functionFlag) print_store_variable_from_RA(get_identifier_for_label_on_current_table(identifier->content)->raPosition);
	else {
		sprintf (buffer, "\t\t\tMM  %s\t\t;\n", identifier->content);
		write_to_code(buffer);

		addr_counter = addr_counter + 2;
	}
}

void resolve_assign() {
	StackNode * identifier =  stack_pop(operand_stack);
	if (identifier->functionFlag) print_load_variable_from_RA(get_identifier_for_label_on_current_table(identifier->content)->raPosition);
	else {
		sprintf(buffer, "\t\t\tLD  %s\t\t; Atribuicao de variavel\n", identifier->content);
		write_to_code(buffer);
	}

	addr_counter = addr_counter + 2;

	identifier = stack_pop(command_operand_stack);
	if (identifier->functionFlag) print_store_variable_from_RA(get_identifier_for_label_on_current_table(identifier->content)->raPosition);
	else {
		sprintf(buffer, "\t\t\tMM  %s\t\t;\n", identifier->content);
		write_to_code(buffer);

		addr_counter = addr_counter + 2;
	}
}

void resolve_while() {
	StackNode * label = stack_check(command_operand_stack);

	StackNode * identifier = stack_pop(operand_stack);
	if(identifier->functionFlag) print_load_variable_from_RA(get_identifier_for_label_on_current_table(identifier->content)->raPosition);
	sprintf(buffer, "\t\t\tLD  %s\t\t;\n", identifier->content);
	write_to_code(buffer);

	sprintf(buffer, "\t\t\tJN  _%s\t\t;\n", label->content);
	write_to_code(buffer);

	sprintf(buffer, "\t\t\tJZ  _%s\t\t;\n", label->content);
	write_to_code(buffer);

	stack_push(command_operator_stack, "endwhile", 0);
	enter_new_scope();

	addr_counter = addr_counter + 6;
}

void resolve_end_while() {
	StackNode * label = stack_pop(command_operand_stack);

	sprintf(buffer, "\t\t\tJP  %s\t\t;\n", label->content);
	write_to_code(buffer);

	sprintf(buffer, "_%s\t\t\tLD  zero\t; End while loop\n", label->content);
	write_to_code(buffer);

	exit_current_scope();

	addr_counter = addr_counter + 4;
}

void resolve_if() {
	StackNode * label = stack_check(command_operand_stack);


	StackNode * identifier = stack_pop(operand_stack);
	if(identifier->functionFlag) print_load_variable_from_RA(get_identifier_for_label_on_current_table(identifier->content)->raPosition);
	sprintf(buffer, "\t\t\tLD  %s\t\t;\n", identifier->content);
	write_to_code(buffer);

	sprintf(buffer, "\t\t\tJN  _%s\t\t;\n", label->content);
	write_to_code(buffer);

	sprintf(buffer, "\t\t\tJZ  _%s\t\t;\n", label->content);
	write_to_code(buffer);

	stack_push(command_operator_stack, "endif", 0);
	enter_new_scope();

	addr_counter = addr_counter + 4;
}

void resolve_end_if() {
	StackNode * label = stack_pop(command_operand_stack);

	sprintf(buffer, "_%s\t\t\tLD  zero\t; End if case\n", label->content);
	write_to_code(buffer);

	exit_current_scope();

	addr_counter = addr_counter + 2;
}

void resolve_else() {
	StackNode * else_label = stack_pop(command_operand_stack);
	StackNode * end_if_label = stack_pop(command_operand_stack);
	stack_pop(command_operator_stack);

	sprintf(buffer, "\t\t\tJP  %s\t\t;\n", else_label->content);
	write_to_code(buffer);

	sprintf(buffer, "_%s\t\t\tLD  zero\t; End if case/Begin else case\n", end_if_label->content);
	write_to_code(buffer);

	stack_push(command_operand_stack, else_label->content, 0);
	stack_push(command_operator_stack, "endelse", 0);

	addr_counter = addr_counter + 4;
}

void resolve_end_else() {
	StackNode * label = stack_pop(command_operand_stack);

	sprintf(buffer, "%s\t\t\tLD  zero\t; End else case\n", label->content);
	write_to_code(buffer);

	addr_counter = addr_counter + 2;
}

void resolve_return() {
	StackNode * label = stack_pop(operand_stack);
	sprintf(buffer, "\t\t\tLD  %s\t\t; Returns value\n", label->content);
	write_to_code(buffer);

	addr_counter = addr_counter + 2;
}

void resolve_command(Token *token) {
	StackNode * command = stack_pop(command_operator_stack);

	if(strcmp(command->content, "=") == 0) resolve_assign();
	else if(strcmp(command->content, "output") == 0) resolve_output();
	else if(strcmp(command->content, "input") == 0) resolve_input();
	else if(strcmp(command->content, "while") == 0) resolve_while();
	else if(strcmp(command->content, "endwhile") == 0) resolve_end_while();
	else if(strcmp(command->content, "if") == 0) resolve_if();
	else if(strcmp(command->content, "endif") == 0) resolve_end_if();
	else if(strcmp(command->content, "else") == 0) resolve_else();
	else if(strcmp(command->content, "endelse") == 0) resolve_end_else();
	else if(strcmp(command->content, "return") == 0) resolve_return();
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
	StackNode * Y = stack_pop(operand_stack);
	StackNode * X = stack_pop(operand_stack);

	if(X->functionFlag) print_load_variable_from_RA(get_identifier_for_label_on_current_table(X->content)->raPosition);
	else {
		sprintf(buffer, "\t\t\tLD  %s\t\t; Comparacao X > Y\n", X->content);
		write_to_code(buffer);
	}

	if(Y->functionFlag) print_load_variable_from_RA(get_identifier_for_label_on_current_table(Y->content)->raPosition);
	else {
		sprintf(buffer, "\t\t\t-   %s\t\t;\n", Y->content);
		write_to_code(buffer);
	}

	char * temp = get_temp_label();
	sprintf(buffer, "\t\t\tMM  %s\t\t;\n", temp);
	write_to_code(buffer);

	stack_push(operand_stack, temp, is_in_function());

	addr_counter = addr_counter + 6;
}

void resolve_compare_less_than() {
	StackNode * Y = stack_pop(operand_stack);
	StackNode * X = stack_pop(operand_stack);

	if(Y->functionFlag) print_load_variable_from_RA(get_identifier_for_label_on_current_table(Y->content)->raPosition);
	else {
		sprintf(buffer, "\t\t\tLD  %s\t\t; Comparacao X < Y\n", Y->content);
		write_to_code(buffer);
	}

	if(X->functionFlag) print_load_variable_from_RA(get_identifier_for_label_on_current_table(X->content)->raPosition);
	else {
		sprintf(buffer, "\t\t\t-  %s\t\t;\n", X->content);
		write_to_code(buffer);
	}

	char * temp = get_temp_label();
	sprintf(buffer, "\t\t\tMM  %s\t\t;\n", temp);
	write_to_code(buffer);

	stack_push(operand_stack, temp, is_in_function());

	addr_counter = addr_counter + 6;
}

void resolve_compare_equal_equal() {
	StackNode * Y = stack_pop(operand_stack);
	StackNode * X = stack_pop(operand_stack);

	if(X->functionFlag) print_load_variable_from_RA(get_identifier_for_label_on_current_table(X->content)->raPosition);
	else {
		sprintf(buffer, "\t\t\tLD  %s\t\t; Comparacao X == Y\n", X->content);
		write_to_code(buffer);
	}

	if(Y->functionFlag) print_load_variable_from_RA(get_identifier_for_label_on_current_table(Y->content)->raPosition);
	else {
		sprintf(buffer, "\t\t\t-  %s\t\t;\n", Y->content);
		write_to_code(buffer);
	}

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

	stack_push(operand_stack, temp, is_in_function());

	addr_counter = addr_counter + 14;
}

void resolve_logic_and() {
	StackNode * X = stack_pop(operand_stack);

	if(X->functionFlag) print_load_variable_from_RA(get_identifier_for_label_on_current_table(X->content)->raPosition);
	else {
		sprintf(buffer, "\t\t\tLD  %s\t\t; Comeco do and logico\n", X->content);
		write_to_code(buffer);
	}

	char * temp = get_temp_label_label();
	sprintf(buffer, "\t\t\tJZ  %s\t\t; Returns NO\n", temp);
	write_to_code(buffer);

	sprintf(buffer, "\t\t\tJN  %s\t\t; Returns NO\n", temp);
	write_to_code(buffer);

	stack_push(operand_stack, temp, is_in_function());
	stack_push(operator_stack, "_and", 0);

	addr_counter = addr_counter + 6;
}

void end_logic_and() {
	StackNode * Y = stack_pop(operand_stack);
	StackNode * no_label = stack_pop(operand_stack);

	if(Y->functionFlag) print_load_variable_from_RA(get_identifier_for_label_on_current_table(Y->content)->raPosition);
	else {
		sprintf(buffer, "\t\t\tLD  %s\t\t;\n", Y->content);
		write_to_code(buffer);
	}

	sprintf(buffer, "\t\t\tJZ  %s\t\t; Returns NO\n", no_label->content);
	write_to_code(buffer);

	sprintf(buffer, "\t\t\tJN  %s\t\t; Returns NO\n", no_label->content);
	write_to_code(buffer);

	sprintf(buffer, "\t\t\tLD  one\t\t;\n");
	write_to_code(buffer);

	char * yes_label = get_temp_label_label();
	sprintf(buffer, "\t\t\tJP  %s\t\t; Returns YES\n", yes_label);
	write_to_code(buffer);

	sprintf(buffer, "%s\t\t\tLD zero\t;\n", no_label->content);
	write_to_code(buffer);

	char * temp = get_temp_label();
	sprintf(buffer, "%s\t\t\tMM  %s\t\t; Fim do and logico\n", yes_label, temp);
	write_to_code(buffer);

	stack_push(operand_stack, temp, is_in_function());

	addr_counter = addr_counter + 14;
}

void resolve_logic_or() {
	StackNode * X = stack_pop(operand_stack);

	sprintf(buffer, "\t\t\tLD  %s\t\t; Comeco do or logico\n", X->content);
	write_to_code(buffer);

	char * temp = get_temp_label_label();
	sprintf(buffer, "\t\t\tJZ  %s\t\t;\n", temp);
	write_to_code(buffer);

	sprintf(buffer, "\t\t\tJN  %s\t\t;\n", temp);
	write_to_code(buffer);

	char * yes_label = get_temp_label_label();
	sprintf(buffer, "\t\t\tJP  %s\t\t; Returns YES\n", yes_label);
	write_to_code(buffer);

	stack_push(operand_stack, yes_label, 0);
	stack_push(operand_stack, temp, 0);
	stack_push(operator_stack, "_or", 0);

	addr_counter = addr_counter + 8;
}

void end_logic_or() {
	StackNode * Y = stack_pop(operand_stack);
	StackNode* temp = stack_pop(operand_stack);
	StackNode * yes_label = stack_pop(operand_stack);

	sprintf(buffer, "%s\t\t\tLD  %s\t\t;\n", temp->content, Y->content);
	write_to_code(buffer);

	char * temp2 = get_temp_label_label();
	sprintf(buffer, "\t\t\tJZ  %s\t\t;\n", temp2);
	write_to_code(buffer);

	sprintf(buffer, "\t\t\tJN  %s\t\t;\n", temp2);
	write_to_code(buffer);

	sprintf(buffer, "\t\t\tJP  %s\t\t; Returns YES\n", yes_label->content);
	write_to_code(buffer);

	sprintf(buffer, "%s\t\t\tLD  zero\t;\n", temp2);
	write_to_code(buffer);

	char * no_label = get_temp_label_label();
	sprintf(buffer, "\t\t\tJP  %s\t\t; Returns NO\n", no_label);
	write_to_code(buffer);

	sprintf(buffer, "%s\t\t\tLD one\t\t;\n", yes_label->content);
	write_to_code(buffer);

	char * temp3 = get_temp_label();
	sprintf(buffer, "%s\t\t\tMM  %s\t\t; Fim do or logico\n", no_label, temp3);
	write_to_code(buffer);

	stack_push(operand_stack, temp3, is_in_function());

	addr_counter = addr_counter + 16;
}

void resolve_logic_not() {
	StackNode * Y = stack_pop(operand_stack);

	if(Y->functionFlag) print_load_variable_from_RA(get_identifier_for_label_on_current_table(Y->content)->raPosition);
	else {
		sprintf(buffer, "\t\t\tLD  %s\t\t; Comeco do not logico\n", Y->content);
		write_to_code(buffer);
	}

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

	stack_push(operand_stack, temp3, is_in_function());

	addr_counter = addr_counter + 14;
}

// X o Y
// o is the top of the operator stack
// X is the second on the operand stack
// Y is the top on the operand stack
void resolve_arithmetic(char * o) {
	StackNode * Y = stack_pop(operand_stack);
	o = get_mvn_operator(o);
	StackNode * X = stack_pop(operand_stack);

	if(X->functionFlag) print_load_variable_from_RA(get_identifier_for_label_on_current_table(X->content)->raPosition);
	else {
		sprintf(buffer, "\t\t\tLD  %s\t\t;\n", X->content);
		write_to_code(buffer);
	}
	if(Y->functionFlag) print_load_variable_from_RA(get_identifier_for_label_on_current_table(Y->content)->raPosition);
	else {
		sprintf(buffer, "\t\t\t%s  %s\t\t;\n", o, Y->content);
		write_to_code(buffer);
	}

	char * temp = get_temp_label();
	sprintf(buffer, "\t\t\tMM  %s\t\t;\n", temp);
	write_to_code(buffer);

	stack_push(operand_stack, temp, is_in_function());

	addr_counter = addr_counter + 6;
}

void resolve_expression() {
	char * o = stack_pop(operator_stack)->content;
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
	stack_push(operand_stack, "one", 0);
}

void push_operand_false(Token *token) {
	stack_push(operand_stack, "zero", 0);
}

void push_operand(Token * token) {
    char * label = get_constant_label(token->value);
	stack_push(operand_stack, label, is_in_function());
}

void push_operator(Token *token) {
	StackNode *operator = stack_check(operator_stack);
	if(operator != NULL && (operator_precedence(operator->content) > operator_precedence(token->lexeme))) {
		resolve_expression();
		push_operator(token);
	} else {
		stack_push(operator_stack, token->lexeme, 0);
		if (strcmp(token->lexeme, "and") == 0) resolve_expression();
		if (strcmp(token->lexeme, "or") == 0) resolve_expression();
	}
}

void push_identifier(Token *token) {
	Node *identifier = get_identifier_for_data(token->lexeme);
	if(identifier->wasDeclared) {
		if(identifier->label == NULL)
			identifier->label = get_variable_label();
		stack_push(operand_stack, identifier->label, is_in_function());
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
	Node * function = get_identifier_for_label_on_current_table(stack_pop(operand_stack)->content);
	if(function->wasDeclared) {
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

		sprintf(buffer, "\t\t\tSC  %s\t\t; Chama funcao\n", function->label);
		write_to_code(buffer);
	} else {
		char err[200];
		sprintf(err, "trying to use undeclared function %s", function->data);
		throw_semantic_exception(ERR_UNDECLARED_FUNCTION, err);
	}
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

	/***********************************************************/
	/* 					  OPERATION RIGHT					   */
	/***********************************************************/

	/***********************************************************/
	/* 					  OPERATION LEFT					   */
	/***********************************************************/

}
