/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_pipe.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glima <gapima7@gmail.com>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 23:02:08 by glima             #+#    #+#             */
/*   Updated: 2025/01/08 23:02:52 by glima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_ast	*ast_pipe_node_init(t_ast *node, t_ast *right)
{
	t_ast	*pipe_node;

	pipe_node = ast_init(AstKind_Pipe);
	pipe_node->u_node.pipe_node.left = node;
	pipe_node->u_node.pipe_node.right = right;
	return (pipe_node);
}

t_ast	*parse_pipe(t_parser *parser, t_shellzin *shell)
{
	t_ast	*node;
	t_ast	*right;

	if (parser->has_error)
		return (NULL);
	node = parse_redirect(parser, shell);
	if (node == NULL)
		return (NULL);
	while (1)
	{
		if (parser_peek(parser).kind != TokenKind_Pipe)
			break ;
		parser_consume(parser);
		right = parse_redirect(parser, shell);
		if (right == NULL)
		{
			parser_set_error(parser, \
			"shellzin: syntax error: Unexpected token");
			break ;
		}
		node = ast_pipe_node_init(node, right);
	}
	return (node);
}
