/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glima <gapima7@gmail.com>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 22:13:07 by glima             #+#    #+#             */
/*   Updated: 2025/01/08 22:13:33 by glima            ###   ########.fr       */
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
		return ("\\n");
	else if (token.kind == TokenKind_Word)
		return (token.content);
	else if (token.kind == TokenKind_StringLiteral)
		return (token.content);
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
