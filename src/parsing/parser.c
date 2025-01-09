/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glima <gapima7@gmail.com>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 19:22:14 by glima             #+#    #+#             */
/*   Updated: 2025/01/08 23:06:00 by glima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_token	parser_consume(t_parser *parser)
{
	t_token	token;

	token = parser_peek(parser);
	if (!parser_iseof(parser))
		parser->cursor = ft_min(parser->cursor + 1, parser->size);
	return (token);
}

t_ast	*parse(char *line, t_shellzin *shell)
{
	t_ast	*ast;

	shell->lexer = lexer_init(line);
	shell->parser = parser_init(&shell->lexer);
	parser_batch_tokens(&shell->parser);
	shell->stop_evaluation = false;
	ast = parse_pipe(&shell->parser, shell);
	return (ast);
}
