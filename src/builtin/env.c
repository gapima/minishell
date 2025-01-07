/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glima <gapima7@gmail.com>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 19:56:48 by glima             #+#    #+#             */
/*   Updated: 2025/01/07 19:56:57 by glima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	shellzin_env(char **argv, t_shellzin *shell)
{
	t_list	*node;

	if (argv[1] != NULL)
	{
		shell->last_status = 1;
		ft_printf_fd(2, "shellzin: env: too many arguments\n");
		return ;
	}
	node = shell->env;
	while (node)
	{
		if (node->content)
			ft_printf("%s\n", (char *)node->content);
		node = node->next;
	}
	shell->last_status = 0;
}
