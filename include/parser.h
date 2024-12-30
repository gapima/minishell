#ifndef PARSER_H
# define PARSER_H

#include "../include/lexer.h"
#include "../include/minishell.h"
#include "../include/utils.h"

typedef struct s_parser
{
	t_lexer *lexer;
	t_token *tokens;
	size_t size;
	size_t cursor;
} t_parser;

t_parser parser_init(t_lexer *lexer);
void parser_batch_tokens(t_parser *parser);
void parse(char *line);

#endif
