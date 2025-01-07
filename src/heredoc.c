#include "../include/minishell.h"
#include <readline/history.h>
#include <unistd.h>

unsigned int random_number()
{
	static unsigned int seed = 12345;
	seed = (seed * 1103515245 + 12345) & 0x7fffffff;
	return (seed);
}

int heredoc_process_line(char *cmp, int heredoc_fd, int _stdin, bool *err, t_shellzin *shell)
{
	char	*line;

	shellzin_is_heredoc(true, 1);
	line = readline("> ");
	shellzin_is_heredoc(false, 1);
	if (!line)
		*err = true;
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
	bool	err;

	seed = ft_itoa(random_number());
	name = ft_strjoin(ft_strdup("/tmp/heredoc"), seed);
	if (!name)
		return (NULL);
	free(seed);
	fd = open(name, O_CREAT|O_WRONLY|O_TRUNC, 0666);
	if (fd == -1)
		return (NULL);
	err = false;
	while (heredoc_process_line(cmp, fd, _stdin, &err, shell))
	{}
	if (g_last_signal != SIGINT && err)
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

void handle_heredoc(t_ast *ast, t_shellzin *shell)
{
	t_list	*cmd_list;
	t_ast		*rnode;
	char		**content;
	char		*heredoc_name;
	int			_stdin;

	rnode = ast->u_node.redirect_node.right->u_node.list_node.list->content;
	cmd_list = ast->u_node.redirect_node.left->u_node.list_node.list;
	while (cmd_list)
	{
		content = &((t_ast *)cmd_list->content)->u_node.word_node.content;
		string_try_expand(content, ft_strlen(*content), shell);
		cmd_list = cmd_list->next;
	}
	_stdin = dup(STDIN_FILENO);
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

void shellzin_heredoc(t_ast *ast, t_shellzin *shell)
{
	if (!ast) return;
	if (ast->kind == AstKind_Redirect && ast->u_node.redirect_node.kind == TokenKind_DLArrow)
		handle_heredoc(ast, shell);
	else if (ast->kind == AstKind_Pipe)
	{
		shellzin_heredoc(ast->u_node.pipe_node.left, shell);
		shellzin_heredoc(ast->u_node.pipe_node.right, shell);
	}
}
