/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glima <gapima7@gmail.com>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 20:44:29 by glima             #+#    #+#             */
/*   Updated: 2025/01/08 20:47:58 by glima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	pipe_evaluate_run_lhs(t_shellzin *shell, t_ast *ast, int pipes[2])
{
	close(pipes[0]);
	dup2(pipes[1], STDOUT_FILENO);
	close(pipes[1]);
	ast_evaluate(shell, ast->u_node.pipe_node.left);
	ast_deinit(shell->ast);
	parser_deinit(&shell->parser);
	shellzin_deinit(shell);
	exit(shell->last_status);
}

static void	pipe_evaluate_run_rhs(t_shellzin *shell, t_ast *ast, \
int pipes[2], int restore)
{
	close(pipes[1]);
	dup2(pipes[0], STDIN_FILENO);
	close(pipes[0]);
	ast_evaluate(shell, ast->u_node.pipe_node.right);
	dup2(restore, STDIN_FILENO);
	close(restore);
}

void	pipe_evaluate(t_shellzin *shell, t_ast *ast)
{
	pid_t	pid;
	int		restore;
	int		pipes[2];

	if (shell->stop_evaluation)
		return ;
	if (pipe(pipes) == -1)
	{
		ft_printf_fd(2, "shellzin: pipe: Unexpected error\n");
		shell->last_status = 1;
		return ;
	}
	pid = fork();
	if (pid == -1)
	{
		ft_printf_fd(2, "shellzin: pipe: Unexpected error\n");
		shell->last_status = 1;
		return ;
	}
	restore = dup(STDIN_FILENO);
	if (pid == 0)
		pipe_evaluate_run_lhs(shell, ast, pipes);
	pipe_evaluate_run_rhs(shell, ast, pipes, restore);
	waitpid(pid, 0, 0);
}
