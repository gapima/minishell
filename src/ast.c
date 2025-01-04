#include "../include/minishell.h"
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

t_ast *ast_init(e_ast_kind kind) {
	t_ast	*ast;

	ast = ft_calloc(sizeof(t_ast), 1);
	shellzin_assert(ast != NULL, "(ast_init) could not allocate memory");
	ast->kind = kind;
	return (ast);
}

void ast_deinit(t_ast *ast) {
	t_list	*head;
	t_list	*next;

	if (!ast)
		return ;
	if (ast->kind == AstKind_Word) {
		free(ast->u_node.word_node.content);
		free(ast);
	}
	else if (ast->kind == AstKind_Redirect)
	{
		ast_deinit(ast->u_node.redirect_node.left);
		ast_deinit(ast->u_node.redirect_node.right);
		free(ast);
	}
	else if (ast->kind == AstKind_List)
	{
		head = ast->u_node.list_node.list;
		while (head) {
			next = head->next;
			ast_deinit((t_ast *)head->content);
			free(head);
			head = next; 
		}
		free(ast);
	}
	else if (ast->kind == AstKind_Pipe)
	{
		ast_deinit(ast->u_node.pipe_node.left);
		ast_deinit(ast->u_node.pipe_node.right);
		free(ast);
	}
}

int wait_children(pid_t pid)
{
	int status;

	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status))
		status = WTERMSIG(status) + 128;
	else if (WIFEXITED(status))
		status = WEXITSTATUS(status);
	return (status);
}

int stat_cmd(char *cmd)
{
	struct stat	st;
	int					status;

	stat(cmd, &st);
	status = 127;
	if (errno == EACCES)
	{
		ft_printf("shellzin: `%s` Permission denied\n", cmd);
		status = 126;
	}
	else if (errno == ENOENT)
		ft_printf("shellzin: Command not found `%s`\n", cmd);
	else
		ft_printf("shellzin: Error\n");
	return (status);
}

int command_spawn(char **argv, char **env, t_shellzin *shell)
{
	pid_t pid;
	int status;
	
	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		signal(SIGQUIT, SIG_DFL);
		execve(argv[0], argv, env);
		status = stat_cmd(argv[0]);
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

void list_evaluate(t_shellzin *shell, t_ast *ast)
{
	char	**argv;
	char	**env;
	char	*cmd;
	int		status;

	env = join_string_list(shell->env);
	argv = join_word_list(ast->u_node.list_node.list);
	cmd = search_path(shell, argv[0]);
	if (cmd)
		argv[0] = cmd;
	status = command_spawn(argv, env, shell);
	if (status != -1)
		shell->last_status = status;
	free(cmd);
	free(env);
	free(argv);
}

t_ast *ast_word_node_init(char *content)
{
	t_ast *node;

	node = ast_init(AstKind_Word);
	node->u_node.word_node.content = ft_strdup(content);
	free(content);
	return (node);
}

void redirection_trim_path(t_ast *ast, t_ast *command_node)
{
	t_list	*word_list;
	t_list	*next;
	t_ast		*node;
	char		*content;

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

t_ast *redirection_get_command_node(t_ast *ast)
{
	if (ast->kind == AstKind_Redirect)
	{
		return redirection_get_command_node(ast->u_node.redirect_node.left);
	}
	return (ast);
}

int open_safe(char *path, int flags, int bflags)
{
	int fd;

	fd = open(path, flags, bflags);
	shellzin_assert(fd != -1, "todo");
	return (fd);
}

void redirection_prepare(t_ast *ast)
{
	t_ast *right;

	if (ast->u_node.redirect_node.left->kind == AstKind_Redirect)
		redirection_prepare(ast->u_node.redirect_node.left);
	right = ast->u_node.redirect_node.right->u_node.list_node.list->content;
	if (ast->u_node.redirect_node.kind == TokenKind_LArrow)
	{
		ast->u_node.redirect_node.fd = open_safe(right->u_node.word_node.content, O_RDONLY, 0666);
		dup2(ast->u_node.redirect_node.fd, STDIN_FILENO);
		close(ast->u_node.redirect_node.fd);
	}
	else if (ast->u_node.redirect_node.kind == TokenKind_RArrow)
	{
		ast->u_node.redirect_node.fd = open_safe(right->u_node.word_node.content, O_CREAT|O_TRUNC|O_WRONLY, 0666);
		dup2(ast->u_node.redirect_node.fd, STDOUT_FILENO);
		close(ast->u_node.redirect_node.fd);
	}
	else if (ast->u_node.redirect_node.kind == TokenKind_DRArrow)
	{
		ast->u_node.redirect_node.fd = open_safe(right->u_node.word_node.content, O_CREAT|O_APPEND|O_WRONLY, 0666);
		dup2(ast->u_node.redirect_node.fd, STDOUT_FILENO);
		close(ast->u_node.redirect_node.fd);
	}
	else if (ast->u_node.redirect_node.kind == TokenKind_DLArrow)
		shellzin_assert(0, "heredoc not implemented");
}

void redirect_evaluate(t_shellzin *shell, t_ast *ast)
{
	t_ast *command_node;
	int		restore[2];

	command_node = redirection_get_command_node(ast);
	redirection_trim_path(ast, command_node);
	restore[0] = dup(STDIN_FILENO);
	restore[1] = dup(STDOUT_FILENO);
	if (ast->u_node.redirect_node.fd == 0)
		redirection_prepare(ast);
	ast_evaluate(shell, ast->u_node.redirect_node.left);
	dup2(restore[0], STDIN_FILENO);
	dup2(restore[1], STDOUT_FILENO);
	close(restore[0]);
	close(restore[1]);
}

void pipe_evaluate(t_shellzin *shell, t_ast *ast)
{
	pid_t	pid;
	int		restore;
	int		pipes[2];

	if (pipe(pipes) == -1)
	{
		printf("shellzin: pipe: Unexpected error\n");
		shell->last_status = 1;
		return ;
	}
	pid = fork();
	if (pid == -1)
	{
		printf("shellzin: pipe: Unexpected error\n");
		shell->last_status = 1;
		return ;
	}
	restore = dup(STDIN_FILENO);
	if (pid == 0)
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
	close(pipes[1]);
	dup2(pipes[0], STDIN_FILENO);
	close(pipes[0]);
	ast_evaluate(shell, ast->u_node.pipe_node.right);
	dup2(restore, STDIN_FILENO);
	close(restore);
	waitpid(pid, 0, 0);
}

void ast_evaluate(t_shellzin *shell, t_ast *ast)
{
	if (ast->kind == AstKind_List)
		list_evaluate(shell, ast);
	else if (ast->kind == AstKind_Redirect)
		redirect_evaluate(shell, ast);
	else if (ast->kind == AstKind_Pipe)
		pipe_evaluate(shell, ast);
}

