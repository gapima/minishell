#include "../include/minishell.h"
#include <stdlib.h>

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
	if (WIFEXITED(status))
		status = WEXITSTATUS(status);
	return (status);
}

int command_spawn(char **argv, char **env)
{
	pid_t pid;
	int status;
	
	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		execve(argv[0], argv, env);
		perror(strerror(errno));
		exit(-1);
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
	(void)shell;

	env = join_string_list(shell->env);
	argv = join_word_list(ast->u_node.list_node.list);
	cmd = search_path(shell, argv[0]);
	if (cmd)
		argv[0] = cmd;
	status = command_spawn(argv, env);
	if (status != -1)
		shell->last_status = status;
	free(cmd);
	free(env);
	free(argv);
}

void redirect_evaluate(t_shellzin *shell, t_ast *ast)
{
	(void)shell;
	ast_print_state(ast, 0);
}

void pipe_evaluate(t_shellzin *shell, t_ast *ast)
{
	(void)shell;
	ast_print_state(ast, 0);
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
