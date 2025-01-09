/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glima <gapima7@gmail.com>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 19:22:14 by glima             #+#    #+#             */
/*   Updated: 2025/01/08 22:47:18 by glima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_parser	parser_init(t_lexer *lexer)
{
	t_parser	parser;

	parser.lexer = lexer;
	parser.tokens = NULL;
	parser.cursor = 0;
	parser.size = 0;
	parser.has_error = false;
	parser.error_msg = NULL;
	parser.error_from_lexer = false;
	parser.should_expand = true;
	return (parser);
}

void	parser_deinit(t_parser *parser)
{
	free(parser->tokens);
}

void	parser_free_token_list(t_parser *parser)
{
	size_t	idx;

	idx = 0;
	while (idx < parser->size)
	{
		if (parser->tokens[idx].kind == TokenKind_Word \
		|| parser->tokens[idx].kind == TokenKind_StringLiteral)
			free(parser->tokens[idx].content);
		idx++;
	}
}

void	parser_set_error(t_parser *parser, char *msg)
{
	parser->has_error = true;
	parser->error_msg = msg;
}

void	parser_batch_tokens(t_parser *parser)
{
	t_lexer	*lexer;
	t_token	token;

	lexer = parser->lexer;
	token = lexer_next(lexer);
	while (token.kind != TokenKind_Eof)
	{
		if (token.kind == TokenKind_Error)
		{
			parser_set_error(parser, token.content);
			parser->error_from_lexer = true;
			return ;
		}
		parser->tokens = ft_realloc(parser->tokens, \
		sizeof(t_token) * parser->size, sizeof(t_token) * (parser->size + 1));
		parser->tokens[parser->size++] = token;
		token = lexer_next(lexer);
	}
	parser->tokens = ft_realloc(parser->tokens, \
	sizeof(t_token) * parser->size, sizeof(t_token) * (parser->size + 1));
	parser->tokens[parser->size++] = token;
}
