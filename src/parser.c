#include "../include/minishell.h"

t_parser parser_init(t_lexer *lexer)
{
	t_parser parser;

	parser.lexer = lexer;
	parser.tokens = NULL;
	parser.cursor = 0;
	parser.size = 0;
	parser.has_error = false;
	parser.error_msg = NULL;
	return (parser);
}

void parser_deinit(t_parser *parser)
{
	free(parser->tokens);
}

void parser_free_token_list(t_parser *parser)
{
	size_t	idx;

	idx = 0;
	while (idx < parser->size)
	{
		if (parser->tokens[idx].kind == TokenKind_Word ||
				parser->tokens[idx].kind == TokenKind_StringLiteral)
		{
			free(parser->tokens[idx].content);
		}
		idx++;
	}
}

void parser_set_error(t_parser *parser, char *msg)
{
	parser->has_error = true;
	parser->error_msg = msg;
}

void parser_batch_tokens(t_parser *parser)
{
	t_lexer	*lexer;
	t_token	token;

	lexer = parser->lexer;
	token = lexer_next(lexer);
	while (token.kind != TokenKind_Eof) {
		if (token.kind == TokenKind_Error)
		{
			parser_set_error(parser, token.content);
			parser_free_token_list(parser);
			return ;
		}
		parser->tokens = ft_realloc(parser->tokens,
															sizeof(t_token) * parser->size,
															sizeof(t_token) * (parser->size + 1));
		parser->tokens[parser->size++] = token;
		token = lexer_next(lexer);
	}
	parser->tokens = ft_realloc(parser->tokens,
														 sizeof(t_token) * parser->size,
														 sizeof(t_token) * (parser->size + 1));
	parser->tokens[parser->size++] = token;
}

t_token parser_peek(t_parser *parser)
{
	return parser->tokens[parser->cursor];
}

bool parser_iseof(t_parser *parser)
{
	return parser->cursor >= parser->size || (parser_peek(parser).kind == TokenKind_Eof);
}

t_token parser_consume(t_parser *parser)
{
	t_token token;

	token = parser_peek(parser);
	if (!parser_iseof(parser)) {
		parser->cursor = ft_min(parser->cursor + 1, parser->size);
	}
	return (token);
}

bool is_redirection_symbol(e_token_kind kind)
{
	return (kind == TokenKind_LArrow ||
	kind == TokenKind_RArrow ||
	kind == TokenKind_DRArrow ||
	kind == TokenKind_DLArrow);
}

void string_trim_quotes(char **str, size_t size)
{
	(*str)[size-1] = '\0';
	*str = ft_memmove(*str, (*str)+1, size);
}

t_ast *parse_word(t_parser *parser)
{
	t_ast					*word_node;
	t_token				token;
	e_token_kind	kind;

	if (parser->has_error)
		return (NULL);
	kind = parser_peek(parser).kind;
	if (kind != TokenKind_Word &&
		kind != TokenKind_StringLiteral)
		return (NULL);

	token = parser_consume(parser);
	word_node = ast_init(AstKind_Word);
	/*if (kind == TokenKind_StringLiteral)*/
	/*	string_trim_quotes(&token.content, token.end-token.start);*/
	//TODO(rnoba) expand $
	word_node->u_node.word_node.content = token.content;
	return (word_node);
}

t_ast *parse_word_list(t_parser *parser)
{
	t_ast	*list_node;
	t_ast	*word_node;

	if (parser->has_error)
		return (NULL);
	word_node = parse_word(parser);
	if (!word_node)
		return (NULL);
	list_node = ast_init(AstKind_List);
	while (word_node) {
		ft_lstadd_back(&list_node->u_node.list_node.list, ft_lstnew(word_node));
		word_node = parse_word(parser);
	}
	return (list_node);
}

t_ast *parse_redirect(t_parser *parser)
{
	t_ast					*redirect_node;
	t_ast					*node;
	t_ast					*right;
	e_token_kind	kind;

	if (parser->has_error)
		return (NULL);
	node = parse_word_list(parser);
	if (node == NULL)
		return (NULL);
	while (1) {
		kind = parser_peek(parser).kind;
		if (!is_redirection_symbol(kind))
			break;
		parser_consume(parser);
		right = parse_word_list(parser);
		if (right == NULL)
		{
			parser_set_error(parser, "shellzin: syntax error");
			break;
		}
		redirect_node = ast_init(AstKind_Redirect);
		redirect_node->u_node.redirect_node.kind = kind;
		redirect_node->u_node.redirect_node.left = node;
		redirect_node->u_node.redirect_node.right = right;
		node = redirect_node;
	}
	return (node);
}

t_ast *parse_pipe(t_parser *parser)
{
	t_ast	*pipe_node;
	t_ast	*node;
	t_ast	*right;

	if (parser->has_error)
		return (NULL);
	node = parse_redirect(parser);
	if (node == NULL)
		return (NULL);
	while (1) {
		if (parser_peek(parser).kind != TokenKind_Pipe)
			break;
		parser_consume(parser);
		right = parse_redirect(parser);
		if (right == NULL)
		{
			parser_set_error(parser, "shellzin: syntax error");
			break;
		}
		pipe_node = ast_init(AstKind_Pipe);
		pipe_node->u_node.pipe_node.left = node;
		pipe_node->u_node.pipe_node.right = right;
		node = pipe_node;
	}
	return (node);
}

void print_indent(int lv)
{
	while (lv--) {
		ft_putchar(' ');
		ft_putchar(' ');
	}
}

void ast_print_state(t_ast *ast, int lv) {
	if (!ast)
		return ;

	print_indent(lv);
	if (ast->kind == AstKind_Word) {
		ft_putendl_fd(ast->u_node.word_node.content, 1);
	}
	else if (ast->kind == AstKind_Redirect)
	{
		ft_putendl_fd("(REDIRECT)", 1);
		ast_print_state(ast->u_node.redirect_node.left, lv + 1);
		ast_print_state(ast->u_node.redirect_node.right, lv + 1);
	}
	else if (ast->kind == AstKind_List)
	{
		t_list *head = ast->u_node.list_node.list;
		while (head) {
			ast_print_state((t_ast *)head->content, lv);
			head = head->next;
		}
	}
	else if (ast->kind == AstKind_Pipe)
	{
		ft_putendl_fd("<PIPE>", 1);
		ast_print_state(ast->u_node.pipe_node.left, lv + 1);
		ast_print_state(ast->u_node.pipe_node.right, lv + 1);
	}
}

t_ast *parse(char *line, t_shellzin *shell) {
	shell->lexer = lexer_init(line);
	shell->parser = parser_init(&shell->lexer);

	parser_batch_tokens(&shell->parser);
	return (parse_pipe(&shell->parser));
}
