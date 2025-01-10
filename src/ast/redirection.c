/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glima <gapima7@gmail.com>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 20:16:58 by glima             #+#    #+#             */
/*   Updated: 2025/01/08 20:32:49 by glima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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

static int	redirection_handle_heredoc(char *path, int *fd)
{
	int	status;

	status = open_dup2_close(path, O_RDONLY, STDIN_FILENO, fd);
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
		return (open_dup2_close(path, O_RDONLY, STDIN_FILENO, fd));
	else if (ast->u_node.redirect_node.kind == TokenKind_RArrow)
		return (open_dup2_close(path, O_CREAT | O_TRUNC \
		| O_WRONLY, STDOUT_FILENO, fd));
	else if (ast->u_node.redirect_node.kind == TokenKind_DRArrow)
		return (open_dup2_close(path, O_CREAT | O_APPEND \
		| O_WRONLY, STDOUT_FILENO, fd));
	else if (ast->u_node.redirect_node.kind == TokenKind_DLArrow)
		return (redirection_handle_heredoc(path, fd));
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
