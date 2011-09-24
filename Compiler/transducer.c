#include <stdlib.h>
#include <string.h>

void transducer_get_next_token() {
	char *current_char = get_next_char();
	char *look_ahead_char = get_look_ahead();
	char lexeme[] = current_char;

	int token_type;
	if (is_letter(*current_char)) {
		while (*look_ahead_char != ' ') {
			char *current_char = next_char();
			char *look_ahead_char = look_ahead();
			strcat (lexeme, current_char);
		}
		token_type = TTYPE_IDENT_OR_RESERV;
	} else if (true) {
		//todo..
		;
	}
	
	new_token->type = token_type;
	new_token->lexeme = lexeme;

	//OBS: ACHO QUE PRECISA DE UMA TABELA DE TRANSIÇÕES (ESTADO_ATUAL, CHAR)->PROX_ESTADO
}

