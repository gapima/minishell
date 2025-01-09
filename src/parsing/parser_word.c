/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_word.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glima <gapima7@gmail.com>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 23:05:44 by glima             #+#    #+#             */
/*   Updated: 2025/01/08 23:29:50 by glima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_ast	*parse_word(t_parser *parser, t_shellzin *shell)
{
	t_ast				*word_node;
	t_token				token;
	enum e_token_kind	kind;

	if (parser->has_error)
		return (NULL);
	kind = parser_peek(parser).kind;
	if (kind != TokenKind_Word \
	&& kind != TokenKind_StringLiteral)
		return (NULL);
	token = parser_consume(parser);
	word_node = ast_init(AstKind_Word);
	if (parser->should_expand)
		word_node->u_node.word_node.is_expanded = \
		string_try_expand(&token.content, token.end - token.start, shell);
	word_node->u_node.word_node.content = token.content;
	word_node->u_node.word_node.is_string = kind == TokenKind_StringLiteral;
	parser->should_expand = true;
	return (word_node);
}

t_ast	*parse_word_list(t_parser *parser, t_shellzin *shell)
{
	t_ast	*list_node;
	t_ast	*word_node;

	if (parser->has_error)
		return (NULL);
	word_node = parse_word(parser, shell);
	while (word_node \
	&& word_node->u_node.word_node.content[0] == '\0' \
	&& word_node->u_node.word_node.is_expanded)
	{
		free(word_node->u_node.word_node.content);
		free(word_node);
		word_node = parse_word(parser, shell);
	}
	if (!word_node)
		return (NULL);
	list_node = ast_init(AstKind_List);
	while (word_node)
	{
		ft_lstadd_back(&list_node->u_node.list_node.list, ft_lstnew(word_node));
		word_node = parse_word(parser, shell);
	}
	return (list_node);
}
