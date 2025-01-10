/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glima <gapima7@gmail.com>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 19:22:14 by glima             #+#    #+#             */
/*   Updated: 2025/01/08 23:30:41 by glima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_token	parser_peek(t_parser *parser)
{
	return (parser->tokens[parser->cursor]);
}

t_token	parser_peek_last(t_parser *parser)
{
	if (parser->size > 0)
		return (parser->tokens[parser->size - 1]);
	if (parser->size == 0)
		return ((t_token){0});
	return (parser->tokens[0]);
}

bool	parser_iseof(t_parser *parser)
{
	return (parser->cursor >= parser->size \
	|| (parser_peek(parser).kind == TokenKind_Eof));
}

size_t	variable_get_end(char *at_dollar)
{
	size_t	end;

	end = 1;
	while (at_dollar[end] && !ft_strchr(SPECIAL, at_dollar[end]) \
	&& (ft_isalnum(at_dollar[end]) || at_dollar[end] == '_'))
		end++;
	if (at_dollar[1] == '?' || (at_dollar[1] == '_' && at_dollar[2] == 0))
		end++;
	return (end);
}

char	*variable_from_key(char *var_key, t_shellzin *shell)
{
	char	*var;

	var = NULL;
	if (var_key[0] == '?')
		var = ft_itoa(shell->last_status);
	else
	{
		var = shellzin_env_search(shell, var_key);
		if (var)
			var = ft_strdup(var);
	}
	free(var_key);
	if (!var)
		var = ft_strdup("");
	return (var);
}
