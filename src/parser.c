/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glima <gapima7@gmail.com>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 19:22:14 by glima             #+#    #+#             */
/*   Updated: 2025/01/07 19:34:38 by glima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_parser	parser_init(t_lexer *lexer)
{
	t_parser	parser;

	parser.lexer = lexer;
	parser.tokens = NULL;
	parser.cursor = 0;
	parser.size = 0;
	parser.has_error = false;
	parser.error_msg = NULL;
	parser.error_from_lexer = false;
	parser.should_expand = true;
	return (parser);
}

void	parser_deinit(t_parser *parser)
{
	free(parser->tokens);
}

void	parser_free_token_list(t_parser *parser)
{
	size_t	idx;

	idx = 0;
	while (idx < parser->size)
	{
		if (parser->tokens[idx].kind == TokenKind_Word \
		|| parser->tokens[idx].kind == TokenKind_StringLiteral)
			free(parser->tokens[idx].content);
		idx++;
	}
}

void	parser_set_error(t_parser *parser, char *msg)
{
	parser->has_error = true;
	parser->error_msg = msg;
}

void	parser_batch_tokens(t_parser *parser)
{
	t_lexer	*lexer;
	t_token	token;

	lexer = parser->lexer;
	token = lexer_next(lexer);
	while (token.kind != TokenKind_Eof)
	{
		if (token.kind == TokenKind_Error)
		{
			parser_set_error(parser, token.content);
			parser->error_from_lexer = true;
			return ;
		}
		parser->tokens = ft_realloc(parser->tokens, \
		sizeof(t_token) * parser->size, sizeof(t_token) * (parser->size + 1));
		parser->tokens[parser->size++] = token;
		token = lexer_next(lexer);
	}
	parser->tokens = ft_realloc(parser->tokens, \
	sizeof(t_token) * parser->size, sizeof(t_token) * (parser->size + 1));
	parser->tokens[parser->size++] = token;
}

t_token	parser_peek(t_parser *parser)
{
	return (parser->tokens[parser->cursor]);
}

t_token	parser_peek_last(t_parser *parser)
{
	if (parser->size > 0)
		return (parser->tokens[parser->size - 1]);
	if (parser->size == 0)
		return ((t_token){0});
	return (parser->tokens[0]);
}

bool	parser_iseof(t_parser *parser)
{
	return (parser->cursor >= parser->size \
	|| (parser_peek(parser).kind == TokenKind_Eof));
}

t_token	parser_consume(t_parser *parser)
{
	t_token	token;

	token = parser_peek(parser);
	if (!parser_iseof(parser))
		parser->cursor = ft_min(parser->cursor + 1, parser->size);
	return (token);
}

static bool	is_redirection_symbol(enum e_token_kind kind)
{
	return (kind == TokenKind_LArrow \
	|| kind == TokenKind_RArrow \
	|| kind == TokenKind_DRArrow \
	|| kind == TokenKind_DLArrow);
}

static size_t variable_get_end(char *at_dollar)
{
	size_t	end;

	end = 1;
	while (at_dollar[end] &&
				!ft_strchr(SPECIAL, at_dollar[end]) &&
				(ft_isalnum(at_dollar[end]) || at_dollar[end] == '_'))
		end++;
	if (at_dollar[1] == '?' || (at_dollar[1] == '_' && at_dollar[2] == 0))
		end++;
	return (end);
}

static char	*variable_from_key(char *var_key, t_shellzin *shell)
{
	char	*var;

	var = NULL;
	if (var_key[0] == '?')
		var = ft_itoa(shell->last_status);
	else
	{
		var = shellzin_env_search(shell, var_key);
		if (var)
			var = ft_strdup(var);
	}
	free(var_key);
	if (!var)
		var = ft_strdup("");
	return (var);
}

static char	*variable_expand(char **src, char *at_dollar, size_t start, t_shellzin *shell)
{
	char		*var_key;
	char		*var;
	char		*result;
	size_t	end;
	size_t	size;

	end = variable_get_end(at_dollar);
	if (end == 1)
		return (NULL);
	var_key = ft_substr(*src, start + 1, end - 1);
	if (!var_key)
		return (NULL);
	var = variable_from_key(var_key, shell);
	size = start + ft_strlen(var) + ft_strlen(at_dollar + end) + 1;
	result = ft_calloc(sizeof(char), size);
	if (!result)
		return (NULL);
	ft_memcpy(result, *src, start);
	ft_memcpy(result + start, var, ft_strlen(var));
	ft_memcpy(result + start + ft_strlen(var), at_dollar + end, ft_strlen(at_dollar + end));
	free(var);
	return (result);
}

static void	try_expand_variable(char **str, t_shellzin *shell)
{
	char	*at_dollar;
	char	*ret;
	size_t	start;

	if (!str)
		return ;
	at_dollar = ft_strchr(*str, '$');
	if (!at_dollar)
		return ;
	start = at_dollar - (*str);
	ret = variable_expand(str, at_dollar, start, shell);
	if (ret == NULL)
		return ;
	free(*str);
	*str = ret;
	try_expand_variable(str, shell);
}

static char	*collect_next_substring(char *str, size_t *start, size_t size, bool *expand)
{
	char	quote;
	size_t	st;
	size_t	end;
	char	*ret;

	quote = '\0';
	if (str[*start] == '\'' || str[*start] == '\"')
		quote = str[*start];
	*expand = quote != '\'';
	st = *start;
	if (quote != '\0')
		st += 1;
	end = st;
	while (end < size && quote != '\0' && str[end] != quote)
		end += 1;
	while (end < size && quote == '\0' \
	&& !(str[end] == '\'' || str[end] == '\"'))
		end += 1;
	ret = ft_substr(str, st, end - st);
	if (quote != '\0')
		end += 1;
	*start = end;
	return (ret);
}

bool	string_try_expand(char **str, size_t size, t_shellzin *shell)
{
	size_t	start;
	char	*curr;
	char	*joined;
	bool	should_expand;
	bool	expanded;

	start = 0;
	joined = NULL;
	should_expand = false;
	expanded = false;
	while (start < size)
	{
		curr = collect_next_substring(*str, &start, size, &should_expand);
		if (should_expand)
		{
			expanded = true;
			try_expand_variable(&curr, shell);
		}
		joined = ft_strjoin(joined, curr);
		free(curr);
	}
	free(*str);
	*str = joined;
	return (expanded);
}

static t_ast	*parse_word(t_parser *parser, t_shellzin *shell)
{
	t_ast			*word_node;
	t_token			token;
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

static t_ast	*parse_word_list(t_parser *parser, t_shellzin *shell)
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

static t_ast	*ast_redirect_node_init(enum e_token_kind kind, t_ast *node, t_ast *right)
{
	t_ast	*redirect_node;

	redirect_node = ast_init(AstKind_Redirect);
	redirect_node->u_node.redirect_node.kind = kind;
	redirect_node->u_node.redirect_node.left = node;
	redirect_node->u_node.redirect_node.right = right;
	redirect_node->u_node.redirect_node.fd = 0;
	return (redirect_node);
}

static t_ast	*parse_redirect(t_parser *parser, t_shellzin *shell)
{
	t_ast			*node;
	t_ast			*right;
	enum e_token_kind	kind;

	if (parser->has_error)
		return (NULL);
	node = parse_word_list(parser, shell);
	if (node == NULL)
		node = ast_init(AstKind_List);
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

static t_ast	*ast_pipe_node_init(t_ast *node, t_ast *right)
{
	t_ast	*pipe_node;

	pipe_node = ast_init(AstKind_Pipe);
	pipe_node->u_node.pipe_node.left = node;
	pipe_node->u_node.pipe_node.right = right;
	return (pipe_node);
}

static t_ast	*parse_pipe(t_parser *parser, t_shellzin *shell)
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

t_ast	*parse(char *line, t_shellzin *shell)
{
	t_ast	*ast;

	shell->lexer = lexer_init(line);
	shell->parser = parser_init(&shell->lexer);
	parser_batch_tokens(&shell->parser);
	shell->stop_evaluation = false;
	ast = parse_pipe(&shell->parser, shell);
	shellzin_heredoc(ast, shell);
	return (ast);
}
