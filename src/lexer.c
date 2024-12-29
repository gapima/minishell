/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glima <gapima7@gmail.com>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 18:03:10 by glima             #+#    #+#             */
/*   Updated: 2024/12/29 18:27:41 by glima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/lexer.h"

char	lexer_consume_specific(t_lexer *lexer, char c)
{
	bool	r;

	r = false;
	if (lexer_peek(lexer) == c)
	{
		lexer_consume(lexer);
		r = true;
	}
	return (r);
}

void	lexer_skip_whitespaces(t_lexer *lexer)
{
	while (!lexer_iseof(lexer) && \
	ft_strchr(BLANKS, lexer_peek(lexer)) != NULL)
		lexer_consume(lexer);
}

char	*get_token_symbol(t_token token)
{
	if (token.kind == TokenKind_Eof)
		return ("<EOF>");
	else if (token.kind == TokenKind_Word)
		return ("<WORD>");
	else if (token.kind == TokenKind_StringLiteral)
		return ("<StringLiteral>");
	else if (token.kind == TokenKind_Pipe)
		return ("|");
	else if (token.kind == TokenKind_LArrow)
		return ("<");
	else if (token.kind == TokenKind_RArrow)
		return (">");
	else if (token.kind == TokenKind_DRArrow)
		return (">>");
	else if (token.kind == TokenKind_DLArrow)
		return ("<<");
	return (NULL);
}

void	lexer_print_state(t_lexer *lexer)
{
	t_token	token;

	token = lexer_next(lexer);
	while (token.kind != TokenKind_Eof)
	{
		if (token.kind == TokenKind_Skip) {
			token = lexer_next(lexer);
			continue;
		}
		printf("(%s)", get_token_symbol(token));

		if (token.kind == TokenKind_Word ||
			token.kind == TokenKind_StringLiteral)
			printf(": %s", token.content);
		printf("\n");
		token = lexer_next(lexer);
	}
}

t_token	lexer_next(t_lexer *lexer)
{
	t_token	token;
	char	c;

	lexer_skip_whitespaces(lexer);
	token.kind = TokenKind_Eof;
	token.content = NULL;
	token.start = lexer->cursor;
	token.end = lexer->size;
	if (lexer_iseof(lexer))
		return (token);
	c = lexer_peek(lexer);
	if (c == '|')
	{
		lexer_consume_specific(lexer, '|');
		token.kind = TokenKind_Pipe;
		token.end = lexer->cursor;
	}
	else if (c == '<')
	{
		lexer_consume_specific(lexer, '<');
		token.kind = TokenKind_LArrow;
		if (lexer_consume_specific(lexer, '<'))
			token.kind = TokenKind_DLArrow;
		token.end = lexer->cursor;
	}
	else if (c == '>')
	{
		lexer_consume_specific(lexer, '>');
		token.kind = TokenKind_RArrow;
		if (lexer_consume_specific(lexer, '>'))
			token.kind = TokenKind_DLArrow;
		token.end = lexer->cursor;
	}
	else if (c == '\'' || c == '"')
	{
		lexer_consume_specific(lexer, c);
		token.kind = TokenKind_StringLiteral;

		lexer_consume_until(lexer, c);
		if (!lexer_consume_specific(lexer, c)) {
			//ERROR unclosed quotes
			exit(1);
		}

		token.end = lexer->cursor;
		if (token.end - token.start <= 2) {
			token.kind = TokenKind_Skip;
		}

		token.content = ft_substr(lexer->content, \
		token.start, token.end - token.start);
	}
	else
	{
		token.kind = TokenKind_Word;
		while (!lexer_iseof(lexer) && \
		ft_strchr(SPECIAL, lexer_peek(lexer)) == NULL)
			lexer_consume(lexer);
		token.end = lexer->cursor;
		token.content = ft_substr(lexer->content, \
		token.start, token.end - token.start);
	}
	return (token);
}
