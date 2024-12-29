/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_lexer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glima <gapima7@gmail.com>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 18:27:43 by glima             #+#    #+#             */
/*   Updated: 2024/12/29 18:28:12 by glima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/lexer.h"
#include "../include/utils.h"

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
