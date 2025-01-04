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

#include "../include/minishell.h"

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
		token_print_state(token);
		token = lexer_next(lexer);
	}
}

void lexer_consume_string_cleanup(void *a, void *b, t_token *token)
{
	free(a);
	free(b);
	token->kind = TokenKind_Error;
	token->content = "Unclosed quotes";
}

void lexer_consume_string_literal(char c, t_lexer *lexer, t_token *token)
{
	t_token	next;
	char		*str;

	token->kind = TokenKind_StringLiteral;
	str = NULL;
	next.content = NULL;
	while (c == '\'' || c == '"')
	{
		next.start = lexer->cursor + 1;
		lexer_consume_specific(lexer, c);
		lexer_consume_until(lexer, c);
		if (!lexer_consume_specific(lexer, c)) {
			lexer_consume_string_cleanup(next.content, str, token);
			return ;
		}
		next.end = lexer->cursor - 1;
		str = ft_substr(lexer->content, next.start, next.end-next.start);
		next.content = ft_strjoin(next.content, str);
		free(str);
		str = NULL;
		c = lexer_peek(lexer);
	}
	token->end = lexer->cursor;
	token->content = next.content;
	token->size = ft_strlen(token->content);
}

void lexer_consume_word(t_lexer *lexer, t_token *token)
{
	token->kind = TokenKind_Word;
	while (!lexer_iseof(lexer) &&
					ft_strchr(SPECIAL, lexer_peek(lexer)) == NULL)
	{
		lexer_consume(lexer);
	}
	token->end = lexer->cursor;
	token->content = ft_substr(lexer->content,
													 token->start,
													 token->end - token->start);
}

void lexer_try_join_tokens(t_lexer *lexer, t_token *token)
{
	t_token next;
	char		c;

	next.content = NULL;
	while (!lexer_iseof(lexer) &&
		ft_strchr(BLANKS, lexer_peek(lexer)) == NULL)
	{
		next.start = lexer->cursor;
		c = lexer_peek(lexer);
		if (c == '\'' || c == '\"')
		{
			lexer_consume_string_literal(c, lexer, &next);
			if (next.size == 0)
			{
				free(next.content);
				continue;
			}
		}
		else
			lexer_consume_word(lexer, &next);
		if (next.content)
		{
			token->content = ft_strjoin(token->content, next.content);
			free(next.content);
		}
	}
}

t_token	lexer_next(t_lexer *lexer)
{
	t_token	token;
	char		c;

	lexer_skip_whitespaces(lexer);
	token.kind = TokenKind_Eof;
	token.content = NULL;
	token.start = lexer->cursor;
	token.end = lexer->size;
	token.size = lexer->cursor;
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
			token.kind = TokenKind_DRArrow;
		token.end = lexer->cursor;
	}
	else if (c == '\'' || c == '"')
	{
		lexer_consume_string_literal(c, lexer, &token);
		lexer_try_join_tokens(lexer, &token);
	}
	else
	{
		lexer_consume_word(lexer, &token);
		lexer_try_join_tokens(lexer, &token);
	}
	return (token);
}
