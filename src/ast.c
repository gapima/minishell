/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glima <gapima7@gmail.com>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 18:12:31 by glima             #+#    #+#             */
/*   Updated: 2025/01/07 18:30:23 by glima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_ast	*ast_init(e_ast_kind kind)
{
	t_ast	*ast;

	ast = ft_calloc(sizeof(t_ast), 1);
	shellzin_assert(ast != NULL, "(ast_init) could not allocate memory");
	ast->kind = kind;
	return (ast);
}

void	ast_list_deinit(t_ast *ast)
{
	t_list	*head;
	t_list	*next;

	head = ast->u_node.list_node.list;
	while (head)
	{
		next = head->next;
		ast_deinit((t_ast *)head->content);
		free(head);
		head = next;
	}
}

void	ast_deinit(t_ast *ast)
{
	if (!ast)
		return ;
	if (ast->kind == AstKind_Word)
	{
		free(ast->u_node.word_node.content);
	}
	else if (ast->kind == AstKind_Redirect)
	{
		ast_deinit(ast->u_node.redirect_node.left);
		ast_deinit(ast->u_node.redirect_node.right);
	}
	else if (ast->kind == AstKind_List)
	{
		ast_list_deinit(ast);
	}
	else if (ast->kind == AstKind_Pipe)
	{
		ast_deinit(ast->u_node.pipe_node.left);
		ast_deinit(ast->u_node.pipe_node.right);
	}
	free(ast);
}

int	wait_children(pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status))
		status = WTERMSIG(status) + 128;
	else if (WIFEXITED(status))
		status = WEXITSTATUS(status);
	return (status);
}


static void	stat_path_eaccess(char *cmd, char **msg, int *status, struct stat st)
{
	*msg = "Command not found";
	if (S_ISREG(st.st_mode))
	{
		if (cmd[0] == '/' || (cmd[0] == '.' && cmd[1] == '/'))
		{
			*msg = "Permission denied";
			*status = 126;
		}
	}
	else
	{
		if (cmd[0] == '/' || (cmd[0] == '.' && cmd[1] == '/'))
		{
			*msg = "Is a directory";
			*status = 126;
		}
	}
}

static int	stat_path(char *cmd, bool is_cmd)
{
	struct stat	st;
	int					status;
	char				*msg;

	stat(cmd, &st);
	status = 127;
	msg = "error";
	if (errno == EACCES)
		stat_path_eaccess(cmd, &msg, &status, st);
	else if (errno == ENOENT)
	{
		if (ft_strchr(cmd, '/') == NULL)
			msg = "Command not found";
		else
			msg = "No such file or directory";
		if (!is_cmd)
			msg = "No such file or directory";
	}
	ft_printf_fd(2, "shellzin: %s ", msg);
	ft_putendl_fd(cmd, 2);
	return (status);
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

t_ast	*ast_word_node_init(char *content)
{
	t_ast	*node;

	node = ast_init(AstKind_Word);
	node->u_node.word_node.content = ft_strdup(content);
	free(content);
	return (node);
}

void	redirection_trim_path(t_ast *ast, t_ast *command_node)
{
	t_list	*word_list;
	t_list	*next;
	t_ast	*node;
	char	*content;

	if (ast->u_node.redirect_node.left->kind == AstKind_Redirect)
		redirection_trim_path(ast->u_node.redirect_node.left, command_node);
	word_list = ast->u_node.redirect_node.right->u_node.list_node.list->next;
	while (word_list)
	{
		content = ((t_ast *)word_list->content)->u_node.word_node.content;
		node = ast_word_node_init(content);
		ft_lstadd_back(&command_node->u_node.list_node.list, ft_lstnew(node));
		free(word_list->content);
		next = word_list->next;
		free(word_list);
		word_list = next;
	}
	ast->u_node.redirect_node.right->u_node.list_node.list->next = NULL;
}

t_ast	*redirection_get_command_node(t_ast *ast)
{
	if (ast->kind == AstKind_Redirect)
		return (redirection_get_command_node(ast->u_node.redirect_node.left));
	return (ast);
}

int	open_dup2_close(char *path, int flags, int bflags, int d, int *fd)
{
	*fd = open(path, flags, bflags);
	if (*fd == -1)
	{
		stat_path(path, false);
		return (1);
	}
	if (dup2(*fd, d) == -1)
		return (1);
	close(*fd);
	return (0);
}

static int	redirection_handle_heredoc(char *path, int *fd)
{
	int	status;

	status = open_dup2_close(path, O_RDONLY, 0666, STDIN_FILENO, fd);
	unlink(path);
	return (status);
}

int	redirection_prepare(t_ast *ast, t_shellzin *shell)
{
	t_ast	*right;
	char	*path;
	int		*fd;

	if (shell->stop_evaluation)
		return (1);
	if (ast->u_node.redirect_node.left->kind == AstKind_Redirect)
		if (redirection_prepare(ast->u_node.redirect_node.left, shell) != 0)
			return (1);
	right = ast->u_node.redirect_node.right->u_node.list_node.list->content;
	path = right->u_node.word_node.content;
	fd = &ast->u_node.redirect_node.fd;
	if (ast->u_node.redirect_node.left->u_node.redirect_node.fd == -1)
		return (1);
	if (ast->u_node.redirect_node.kind == TokenKind_LArrow)
		return (open_dup2_close(path, O_RDONLY, 0666, STDIN_FILENO, fd));
	else if (ast->u_node.redirect_node.kind == TokenKind_RArrow)
		return (open_dup2_close(path, O_CREAT | O_TRUNC \
													| O_WRONLY, 0666, STDOUT_FILENO, fd));
	else if (ast->u_node.redirect_node.kind == TokenKind_DRArrow)
		return (open_dup2_close(path, O_CREAT | O_APPEND \
													| O_WRONLY, 0666, STDOUT_FILENO, fd));
	else if (ast->u_node.redirect_node.kind == TokenKind_DLArrow)
		return redirection_handle_heredoc(path, fd);
	return (1);
}

void	redirect_evaluate(t_shellzin *shell, t_ast *ast)
{
	t_ast	*command_node;
	int		status;
	int		restore[2];

	if (shell->stop_evaluation)
		return ;
	command_node = redirection_get_command_node(ast);
	redirection_trim_path(ast, command_node);
	restore[0] = dup(STDIN_FILENO);
	restore[1] = dup(STDOUT_FILENO);
	if (ast->u_node.redirect_node.fd == 0)
	{
		status = redirection_prepare(ast, shell);
		if (status != 0)
		{
			shell->stop_evaluation = true;
			shell->last_status = 1;
		}
	}
	ast_evaluate(shell, ast->u_node.redirect_node.left);
	dup2(restore[0], STDIN_FILENO);
	dup2(restore[1], STDOUT_FILENO);
	close(restore[0]);
	close(restore[1]);
}

void	pipe_evaluate_run_lhs(t_shellzin *shell, t_ast *ast, int pipes[2])
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

void	pipe_evaluate_run_rhs(t_shellzin *shell, t_ast *ast, int pipes[2], int restore)
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

void	ast_evaluate(t_shellzin *shell, t_ast *ast)
{
	if (ast->kind == AstKind_List)
		list_evaluate(shell, ast);
	else if (ast->kind == AstKind_Redirect)
		redirect_evaluate(shell, ast);
	else if (ast->kind == AstKind_Pipe)
		pipe_evaluate(shell, ast);
}
