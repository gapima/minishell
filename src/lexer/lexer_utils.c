/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glima <gapima7@gmail.com>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 18:27:43 by glima             #+#    #+#             */
/*   Updated: 2025/01/08 22:11:42 by glima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

bool	lexer_iseof(t_lexer *lexer)
{
	return (lexer->cursor >= lexer->size);
}

char	lexer_peek(t_lexer *lexer)
{
	if (lexer_iseof(lexer))
		return ('\0');
	return (lexer->content[lexer->cursor]);
}

t_lexer	lexer_init(char *str)
{
	t_lexer	lexer;

	lexer.content = str;
	lexer.cursor = 0;
	lexer.size = ft_strlen(str);
	return (lexer);
}

char	lexer_consume(t_lexer *lexer)
{
	char	curr;

	curr = lexer_peek(lexer);
	if (!lexer_iseof(lexer))
		lexer->cursor = ft_min(lexer->cursor + 1, lexer->size);
	return (curr);
}

void	lexer_consume_until(t_lexer *lexer, char c)
{
	while (!lexer_iseof(lexer) && lexer_peek(lexer) != c)
		lexer_consume(lexer);
}
