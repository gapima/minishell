#include "../include/minishell.h"

//TODO(rnoba): remove
#include <assert.h>

t_parser parser_init(t_lexer *lexer)
{
	t_parser parser;

	parser.lexer = lexer;
	parser.tokens = NULL;
	parser.cursor = 0;
	parser.size = 0;
	return (parser);
}

void parser_deinit(t_parser *parser)
{
	free(parser->tokens);
}

void parser_batch_tokens(t_parser *parser)
{
	t_lexer *lexer;
	t_token token;

	lexer = parser->lexer;
	token = lexer_next(lexer);
	while (token.kind != TokenKind_Eof) {

		if (token.kind == TokenKind_Skip) {
			token = lexer_next(lexer);
			continue;
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

bool parser_iseof(t_parser *parser)
{
	return parser->cursor >= parser->size;
}

t_token parser_peek(t_parser *parser)
{
	return parser->tokens[parser->cursor];
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

t_ast *parse_word(t_parser *parser)
{
	t_ast	*word_node;
	e_token_kind	kind;

	kind = parser_peek(parser).kind;
	if (kind != TokenKind_Word &&
			kind != TokenKind_StringLiteral)
		return (NULL);

	word_node = ast_init(AstKind_Word);
	//TODO(rnoba) expand $
	word_node->u_node.word_node.content = parser_consume(parser).content;
	return (word_node);
}

t_ast *parse_word_list(t_parser *parser)
{
	t_ast	*list_node;
	t_ast	*word_node;

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
	t_ast	*redirect_node;
	t_ast	*node;
	t_ast	*right;
	e_token_kind	kind;

	node = parse_word_list(parser);
	if (node == NULL)
		return (NULL);

	while (1) {
		kind = parser_peek(parser).kind;
		if (!is_redirection_symbol(kind))
			break;
		parser_consume(parser);
		right = parse_word(parser);
		if (right == NULL)
			break;
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

	node = parse_redirect(parser);
	if (node == NULL)
		return (NULL);
	while (1) {
		if (parser_peek(parser).kind != TokenKind_Pipe)
			break;
		parser_consume(parser);
		right = parse_redirect(parser);
		if (right == NULL)
			break;
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
