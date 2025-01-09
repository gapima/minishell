/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glima <gapima7@gmail.com>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 18:12:31 by glima             #+#    #+#             */
/*   Updated: 2025/01/08 20:53:45 by glima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_ast	*ast_init(enum e_ast_kind kind)
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

void	ast_evaluate(t_shellzin *shell, t_ast *ast)
{
	if (ast->kind == AstKind_List)
		list_evaluate(shell, ast);
	else if (ast->kind == AstKind_Redirect)
		redirect_evaluate(shell, ast);
	else if (ast->kind == AstKind_Pipe)
		pipe_evaluate(shell, ast);
}
