/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glima <gapima7@gmail.com>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 20:49:35 by glima             #+#    #+#             */
/*   Updated: 2025/01/08 20:53:56 by glima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	wait_children(pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status))
		status = WTERMSIG(status) + 128;
	else if (WIFEXITED(status))
		status = WEXITSTATUS(status);
	return (status);
}

t_ast	*ast_word_node_init(char *content)
{
	t_ast	*node;

	node = ast_init(AstKind_Word);
	node->u_node.word_node.content = ft_strdup(content);
	free(content);
	return (node);
}

int	command_spawn(char **argv, char **env, t_shellzin *shell)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		signal(SIGQUIT, SIG_DFL);
		execve(argv[0], argv, env);
		status = stat_path(argv[0], true);
		free(argv);
		free(env);
		ast_deinit(shell->ast);
		parser_deinit(&shell->parser);
		shellzin_deinit(shell);
		exit(status);
	}
	status = wait_children(pid);
	return (status);
}
