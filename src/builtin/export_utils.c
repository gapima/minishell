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

void	export_set_shell_variable(char *var, t_shellzin *shell)
{
	t_list *node;

	node = shellzin_env_search_node(shell->shell_variables, var, NULL, false);
	if (!node)
	{
		ft_lstadd_back(&shell->shell_variables, ft_lstnew(ft_strdup(var)));
	}
}

void	export_print_shell_variable(t_shellzin *shell)
{
	t_list	*node;

	node = shell->shell_variables;
	while (node)
	{
		ft_printf("declare -x %s\n", (char *)node->content);
		node = node->next;
	}
}


void	shellzin_export_print_var(char *var)
{
	char *dup;
	char	*eq;
	char	*right;

	right = NULL;
	dup = ft_strdup(var);
	eq = ft_strchr(dup, '=');
	if (eq)
	{
		*eq = '\0';
		right = eq + 1;
	}
	if (!right || !*right)
	{
		ft_printf("declare -x %s\n", dup);
	}
	else
		ft_printf("declare -x %s=\"%s\"\n", dup, right);
	free(dup);
}
