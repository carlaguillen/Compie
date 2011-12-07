/* =======================================================
 *	 		PCS 2056 - Linguagens e Compiladores
 * =======================================================
 *
 * transducer.c - Transducer
 *
 * Created on: 15/09/2011
 * 		Authors:
 *     		Bruno Pezzolo dos Santos, 5948816
 *      	Carla Guillen Gomes, 5691366
 */

#include "transducer.h"

Token *token;

int type_for_state(int state) {
	switch (state) {
		case 2:
			return TTYPE_ID;
		case 3:
			return TTYPE_OPERATOR;
		case 4:
			return TTYPE_NUM;
		case 5:
			return TTYPE_SPECIAL;
		case 6:
			return TTYPE_INVALID;
		case 7:
			return TTYPE_COMMENT;
		case 8:
			return TTYPE_END_OF_FILE;
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
	else if (c == '\n')
		return linebreak;
	else if (c == '@')
			return at;
	else if (c == '+' || c == '-' || c == '<' || c == '>' || c == '?')
		return operator;
	else if (c == '(' || c == ')')
		return special;
	else if (c == EOF) 
		return eof;
	else
		return invalid_char;
}

void init_transition_table() {
	int i, j;
	for (i = 0; i < NUM_STATES; i++)
		for (j = 0; j < NUM_TYPE_CHARS; j++)
			transition_table[i][j] = 0;

	/* TTYPE_ID */
	transition_table[1][letter] = 2;
	transition_table[1][at] = 2;

	/* TTYPE_OPERATOR */
	transition_table[1][operator] = 3;

	/* TTYPE_NUM */
	transition_table[1][digit] = 4;
	transition_table[4][digit] = 4;

	/* TTYPE_SPCEIAL */
	transition_table[1][special] = 5;

	/* space, \t and \n */
	transition_table[1][space] = 1;
	transition_table[1][linebreak] = 1;

	/* TTYPE_INVALID */
	transition_table[1][invalid_char] = 6;

	/* TTYPE_COMMENT */
	transition_table[1][sharp] = 7;
	transition_table[7][letter] = 7;
	transition_table[7][digit] = 7;
	transition_table[7][space] = 7;
	transition_table[7][sharp] = 7;
	transition_table[7][at] = 7;
	transition_table[7][operator] = 7;
	transition_table[7][special] = 7;
	transition_table[7][invalid_char] = 7;
	transition_table[7][linebreak] = 1;

	/* TTYPE_END_OF_FILE */
	transition_table[1][eof] = 8;
}

void transducer_get_next_token() {
	char current_char = get_next_char();
	int type_char = get_type_char(current_char);

	int next_state = transition_table[1][type_char];
	int token_type;
	char look_ahead_char; 
	int type_look_ahead;
	List * lexeme;

	/*eof?*/
	if (next_state == 8) {
		token_type = TTYPE_END_OF_FILE;
		lexeme = empty_list();
	} else {
		/* ignoring spaces, \n and \t */
		while(next_state == 1) {
			current_char = get_next_char();
			type_char = get_type_char(current_char);
			next_state = transition_table[next_state][type_char];
		}

		look_ahead_char = get_look_ahead();
		type_look_ahead = get_type_char(look_ahead_char);
		lexeme = empty_list();
		alloc_add_list(current_char, lexeme);

		/* while token is incomplete -- using the lookahead, next state isn't invalid (0) */
		while(transition_table[next_state][type_look_ahead] != 0) {
			current_char = get_next_char();
			type_char = get_type_char(current_char);
			look_ahead_char = get_look_ahead();
			type_look_ahead = get_type_char(look_ahead_char);
			alloc_add_list(current_char, lexeme);
			if(transition_table[next_state][type_char] == 1) {
				/* token is complete now */
				break;
			}
			next_state = transition_table[next_state][type_char];

		}
		token_type = type_for_state(next_state);
		/* fill typeof global variable (the value will be filled by lexico.c) */
	}
	alloc_add_list('\0', lexeme);
	/* fill lexeme of global variable (the value will be filled by lexico.c) */
	token->type = token_type;
	token->lexeme = get_string_array(lexeme);
}

