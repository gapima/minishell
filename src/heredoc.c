/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glima <gapima7@gmail.com>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 18:12:36 by glima             #+#    #+#             */
/*   Updated: 2025/01/08 21:29:01 by glima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

unsigned int	random_number(t_shellzin *shell)
{
	unsigned int	ptr;

	ptr = (unsigned int)((unsigned long)&random_number & 0x7fffffff);
	shell->seed = (shell->seed * 1103515245 + 12345 + ptr) & 0x7fffffff;
	return (shell->seed);
}

int	heredoc_process_line(char *cmp, int heredoc_fd, \
int _stdin, t_shellzin *shell)
{
	char	*line;

	shellzin_is_heredoc(true, 1);
	line = readline("> ");
	shellzin_is_heredoc(false, 1);
	if (!line)
		shell->heredoc_error = true;
	if (g_last_signal == SIGINT)
	{
		shell->last_status = 130;
		dup2(_stdin, STDIN_FILENO);
		free(line);
		line = NULL;
	}
	if (!line || ft_strncmp(line, cmp, ft_strlen(cmp) + 1) == 0)
		return (0);
	string_try_expand(&line, ft_strlen(line), shell);
	ft_putstr_fd(line, heredoc_fd);
	ft_putchar_fd('\n', heredoc_fd);
	free(line);
	return (1);
}

char	*open_heredoc(char *cmp, int _stdin, t_shellzin *shell)
{
	int		fd;
	char	*seed;
	char	*name;

	seed = ft_itoa(random_number(shell));
	name = ft_strjoin(ft_strdup("/tmp/heredoc"), seed);
	free(seed);
	if (!name)
		return (NULL);
	fd = open(name, O_CREAT | O_WRONLY | O_TRUNC, 0666);
	if (fd == -1)
		return (NULL);
	shell->heredoc_error = false;
	while (heredoc_process_line(cmp, fd, _stdin, shell))
		;
	if (g_last_signal != SIGINT && shell->heredoc_error)
		ft_putendl_fd("shellzin: heredoc: line delimited by end-of-file", 2);
	close(fd);
	close(_stdin);
	if (g_last_signal == SIGINT)
	{
		unlink(name);
		name = NULL;
	}
	return (name);
}

void	handle_heredoc(t_ast *ast, t_shellzin *shell, int _stdin)
{
	t_list	*cmd_list;
	t_ast	*rnode;
	char	**content;
	char	*heredoc_name;

	rnode = ast->u_node.redirect_node.right->u_node.list_node.list->content;
	cmd_list = ast->u_node.redirect_node.left->u_node.list_node.list;
	while (cmd_list)
	{
		content = &((t_ast *)cmd_list->content)->u_node.word_node.content;
		string_try_expand(content, ft_strlen(*content), shell);
		cmd_list = cmd_list->next;
	}
	heredoc_name = open_heredoc(rnode->u_node.word_node.content, _stdin, shell);
	if (heredoc_name)
	{
		free(rnode->u_node.word_node.content);
		rnode->u_node.word_node.content = heredoc_name;
	}
	else
	{
		shell->stop_evaluation = true;
		ast->u_node.redirect_node.fd = -1;
	}
}

void	shellzin_heredoc(t_ast *ast, t_shellzin *shell)
{
	if (!ast)
		return ;
	if (ast->kind == AstKind_Redirect \
	&& ast->u_node.redirect_node.kind == TokenKind_DLArrow)
		handle_heredoc(ast, shell, dup(STDIN_FILENO));
	else if (ast->kind == AstKind_Pipe)
	{
		shellzin_heredoc(ast->u_node.pipe_node.left, shell);
		shellzin_heredoc(ast->u_node.pipe_node.right, shell);
	}
}
