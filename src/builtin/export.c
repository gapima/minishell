/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glima <gapima7@gmail.com>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 19:49:47 by glima             #+#    #+#             */
/*   Updated: 2025/01/08 20:59:02 by glima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

bool	shellzin_export_is_valid(char *test)
{
	size_t	idx;
	bool	r;

	if (!test)
		return (false);
	r = true;
	idx = 0;
	while (test[idx])
	{
		if (ft_strchr(SPECIAL, test[idx]) != NULL \
		|| (!ft_isalnum(test[idx]) && test[idx] != '_'))
		{
			r = false;
			break ;
		}
		idx++;
	}
	return (r);
}

void	shellzin_export_print(t_shellzin *shell)
{
	t_list	*node;
	char	**tab;
	int		idx;

	tab = ft_calloc(sizeof(char *), ft_lstsize(shell->env) + 1);
	if (!tab)
		return ;
	idx = 0;
	node = shell->env;
	while (node)
	{
		tab[idx++] = (char *)node->content;
		node = node->next;
	}
	sort_tab(tab, idx);
	idx = 0;
	while (tab[idx])
		ft_printf("declare -x %s\n", tab[idx++]);
	free(tab);
}

bool	shellzin_check_export(char **split)
{
	bool	valid;

	valid = false;
	if (split[0] == NULL)
		ft_printf_fd(2, "shellzin: export: `=` not a valid identifier\n");
	if (split[0] != NULL)
	{
		if (shellzin_export_is_valid(split[0]) && !ft_isdigit(split[0][0]))
			valid = true;
		else
			ft_printf_fd(2, "shellzin: export: %s=%s not a valid identifier\n", split[0], split[1]);
	}
	return (valid);
}

void	shellzin_export_is_error(char **argv, size_t idx, t_shellzin *shell)
{
	if (!shellzin_export_is_valid(argv[idx]) || ft_isdigit(argv[idx][0]))
	{
		ft_printf_fd(2, "shellzin: export: %s \
		not a valid identifier\n", argv[idx]);
		shell->last_status = 1;
	}
	else if (argv[idx][0] == '\0')
	{
		ft_printf_fd(2, "shellzin: export: not a valid identifier\n");
		shell->last_status = 1;
	}
}

void	shellzin_export(char **argv, t_shellzin *shell)
{
	size_t	idx;
	char	**split;

	if (argv[1] == NULL)
		shellzin_export_print(shell);
	idx = 0;
	shell->last_status = 0;
	while (argv[++idx])
	{
		if (!ft_strchr(argv[idx], '='))
		{
			shellzin_export_is_error(argv, idx, shell);
			continue ;
		}
		split = ft_split(argv[idx], '=');
		if (shellzin_check_export(split))
			shellzin_set_or_change_env(shell, split[0], (split[1]));
		else
			shell->last_status = 1;
		string_list_destroy(split);
	}
}
