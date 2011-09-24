#include "transducer.h"

int type_for_state(int state) {
	switch (state) {
		case 2:
			return TTYPE_IDENT_OR_RESERV;
		case 3:
			return TTYPE_SPECIAL_CHARACTER;
		case 4:
			return TTYPE_NUM;
		case 5:
			return TTYPE_STRING;
		case 7:
			return TTYPE_COMMENT;
		case 8:
			return TTYPE_INVALID;
		case 6:
			return TTYPE_SPECIAL_CHARACTER;
		default:
			return -1;
	}
}

type_char get_type_char(char c) {
	if ((c >= 'a' && c <= 'z') ||
		(c >= 'A' && c <= 'Z'))
		return letter;
	else if (c >= '0' && c <= '9')
		return digit;
	else if (c == ' ' || c == '\t')
		return space;
	else if (c == '#')
		return sharp;
	else if (c == '=')
		return equal;
	else if (c == '"')
		return quote;
	else if (c == '\n')
		return linebreak;
	else if (c == '+' || c == '-' || c == '*' || c == '/' || 
			 c == ';' || c == '<' || c == '>' || c == '(' || 
			 c == ')' || c == '“' || c == '{' || c == '}')
		return special;
	else
		return invalid_char;
}

void init_transition_table() {

	transition_table[1][letter] = 2;
	transition_table[2][letter] = 2;
	transition_table[2][digit] = 2;
	transition_table[2][space] = 1;

	transition_table[1][digit] = 4;
	transition_table[4][digit] = 4;
	transition_table[4][space] = 1;

	transition_table[1][space] = 1;
	
	transition_table[1][sharp] = 7;
	transition_table[7][letter] = 7;
	transition_table[7][digit] = 7;
	transition_table[7][space] = 7;
	transition_table[7][sharp] = 7;
	transition_table[7][equal] = 7;
	transition_table[7][quote] = 7;
	transition_table[7][special] = 7;
	transition_table[7][invalid_char] = 7;
	transition_table[7][linebreak] = 1;

	transition_table[1][equal] = 6;
	transition_table[6][equal] = 1;

	transition_table[1][quote] = 5;
    transition_table[5][letter] = 5;
    transition_table[5][digit] = 5;
    transition_table[5][space] = 5;
    transition_table[5][sharp] = 5;
    transition_table[5][equal] = 5;
    transition_table[5][special] = 5;
    transition_table[5][linebreak] = 5;
	transition_table[5][invalid_char] = 5;
    transition_table[5][quote] = 1;

	transition_table[1][linebreak] = 1;

	transition_table[1][special] = 3;
	transition_table[3][space] = 1;

	transition_table[1][invalid_char] = 8;
	transition_table[8][space] = 1;
}

void transducer_get_next_token() {
	int state = 1;
	char *current_char = get_next_char();
	int type_char = get_type_char(*current_char);

	int next_state = transition_table[state][*current_char];
	while(next_state == 1) { //ignoring spaces, \n and \t
		current_char = get_next_char();
		type_char = get_type_char(*current_char);
		next_state = transition_table[state][type_char];
	}
	
	char *look_ahead_char = get_look_ahead();
	int type_look_ahead = get_type_char(*look_ahead_char);
	char lexeme[] = *current_char;

	int token_type;

	while(transition_table[next_state][type_look_ahead] != 1 &&
		  transition_table[next_state][type_look_ahead] != 0) {
			current_char = get_next_char();
			type_char = get_type_char(*current_char);
			look_ahead_char = get_look_ahead();
			type_look_ahead = get_type_char(*look_ahead_char);
			strcat (lexeme, current_char);
			next_state = transition_table[state][type_char];
	}
	token_type = type_for_state(next_state);
	
	token->type = token_type;
	token->lexeme = lexeme;

	//OBS: ACHO QUE PRECISA DE UMA TABELA DE TRANSIÇÕES (ESTADO_ATUAL, CHAR)->PROX_ESTADO
}

