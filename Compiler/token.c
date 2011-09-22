void init_token() {
	token = (Token *) malloc(sizeof(Token));
}

void release_token() {
	free(token);
}