/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_list.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glima <gapima7@gmail.com>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 20:50:53 by glima             #+#    #+#             */
/*   Updated: 2025/01/08 20:51:39 by glima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	list_evaluate(t_shellzin *shell, t_ast *ast)
{
	char	**argv;
	char	**env;
	char	*cmd;
	int		status;

	if (shell->stop_evaluation)
		return ;
	argv = join_word_list(ast->u_node.list_node.list);
	if (argv[0])
	{
		if (!shellzin_try_run_builtin(argv, shell))
		{
			env = join_string_list(shell->env);
			cmd = search_path(shell, argv[0]);
			if (cmd)
				argv[0] = cmd;
			status = command_spawn(argv, env, shell);
			if (status != -1)
				shell->last_status = status;
			free(cmd);
			free(env);
		}
	}
	free(argv);
}
