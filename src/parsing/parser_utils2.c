/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glima <gapima7@gmail.com>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 19:22:14 by glima             #+#    #+#             */
/*   Updated: 2025/01/08 23:31:47 by glima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

bool	is_redirection_symbol(enum e_token_kind kind)
{
	return (kind == TokenKind_LArrow \
	|| kind == TokenKind_RArrow \
	|| kind == TokenKind_DRArrow \
	|| kind == TokenKind_DLArrow);
}

static char	*variable_expand(char **src, char *at_dollar, \
size_t start, t_shellzin *shell)
{
	char	*var_key;
	char	*var;
	char	*result;
	size_t	end;
	size_t	size;

	end = variable_get_end(at_dollar);
	if (end == 1)
		return (NULL);
	var_key = ft_substr(*src, start + 1, end - 1);
	if (!var_key)
		return (NULL);
	var = variable_from_key(var_key, shell);
	size = start + ft_strlen(var) + ft_strlen(at_dollar + end) + 1;
	result = ft_calloc(sizeof(char), size);
	if (!result)
		return (NULL);
	ft_memcpy(result, *src, start);
	ft_memcpy(result + start, var, ft_strlen(var));
	ft_memcpy(result + start + ft_strlen(var), at_dollar + end, \
	ft_strlen(at_dollar + end));
	free(var);
	return (result);
}

static void	try_expand_variable(char **str, t_shellzin *shell)
{
	char	*at_dollar;
	char	*ret;
	size_t	start;

	if (!str)
		return ;
	at_dollar = ft_strchr(*str, '$');
	if (!at_dollar)
		return ;
	start = at_dollar - (*str);
	ret = variable_expand(str, at_dollar, start, shell);
	if (ret == NULL)
		return ;
	free(*str);
	*str = ret;
	try_expand_variable(str, shell);
}

static char	*collect_next_substring(char *str, size_t *start, \
size_t size, bool *expand)
{
	char	quote;
	size_t	st;
	size_t	end;
	char	*ret;

	quote = '\0';
	if (str[*start] == '\'' || str[*start] == '\"')
		quote = str[*start];
	*expand = quote != '\'';
	st = *start;
	if (quote != '\0')
		st += 1;
	end = st;
	while (end < size && quote != '\0' && str[end] != quote)
		end += 1;
	while (end < size && quote == '\0' \
	&& !(str[end] == '\'' || str[end] == '\"'))
		end += 1;
	ret = ft_substr(str, st, end - st);
	if (quote != '\0')
		end += 1;
	*start = end;
	return (ret);
}

bool	string_try_expand(char **str, size_t size, t_shellzin *shell)
{
	size_t	start;
	char	*curr;
	char	*joined;
	bool	should_expand;
	bool	expanded;

	start = 0;
	joined = NULL;
	should_expand = false;
	expanded = false;
	while (start < size)
	{
		curr = collect_next_substring(*str, &start, size, &should_expand);
		if (should_expand)
		{
			expanded = true;
			try_expand_variable(&curr, shell);
		}
		joined = ft_strjoin(joined, curr);
		free(curr);
	}
	free(*str);
	*str = joined;
	return (expanded);
}
