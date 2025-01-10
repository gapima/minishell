/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redirect.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glima <gapima7@gmail.com>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 23:04:17 by glima             #+#    #+#             */
/*   Updated: 2025/01/08 23:28:51 by glima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static t_ast	*while_redirect(t_ast *node, \
t_parser *parser, t_shellzin *shell)
{
	t_ast					*right;
	enum e_token_kind		kind;

	while (1)
	{
		kind = parser_peek(parser).kind;
		if (!is_redirection_symbol(kind))
			break ;
		parser->should_expand = (kind != TokenKind_DLArrow);
		parser_consume(parser);
		right = parse_word_list(parser, shell);
		if (right == NULL)
		{
			parser_set_error(parser, \
			"shellzin: syntax error: Unexpected token");
			break ;
		}
		node = ast_redirect_node_init(kind, node, right);
	}
	return (node);
}

t_ast	*ast_redirect_node_init(enum e_token_kind kind, \
t_ast *node, t_ast *right)
{
	t_ast	*redirect_node;

	redirect_node = ast_init(AstKind_Redirect);
	redirect_node->u_node.redirect_node.kind = kind;
	redirect_node->u_node.redirect_node.left = node;
	redirect_node->u_node.redirect_node.right = right;
	redirect_node->u_node.redirect_node.fd = 0;
	return (redirect_node);
}

t_ast	*parse_redirect(t_parser *parser, t_shellzin *shell)
{
	t_ast	*node;

	if (parser->has_error)
		return (NULL);
	node = parse_word_list(parser, shell);
	if (node == NULL)
		node = ast_init(AstKind_List);
	return (while_redirect(node, parser, shell));
}
