#include "lex.h"
#include "ial.h"

Ttoken *token;

void lex_init(){
	token = malloc(sizeof(Ttoken));
}

Ttoken *get_token(){
	token->name = "if";
	token->type = TOKEN_IF;
	
	return token;
}