#include "../include/minishell.h"

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
