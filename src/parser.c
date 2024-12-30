#include "../include/parser.h"
#include "../include/ast.h"

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

void parser_next(t_parser *parser)
{
	if (!parser_iseof(parser)) {
		parser->cursor = ft_min(parser->cursor + 1, parser->size);
	}
}

t_ast *parse_word(t_parser *parser)
{
	t_ast		*ast;
	t_token	token;

	if (parser_iseof(parser)) {
		return NULL;
	}
	token = parser_peek(parser);
	if (token.kind != TokenKind_Word &&
		token.kind != TokenKind_StringLiteral) {
			return NULL;
	}
	parser_next(parser);
	ast = ast_init(AstKind_Word);
	ast->word.word = token.content;
	return (ast);
}

bool is_redirection(e_token_kind kind)
{
	return (kind == TokenKind_LArrow ||
				kind == TokenKind_RArrow ||
				kind == TokenKind_DRArrow ||
				kind == TokenKind_DLArrow);
}

t_ast *parse_redirect(t_parser *parser)
{
	t_ast		*ast;
	t_token	token;
	t_ast		*lhs;
	t_ast		*rhs;

	if (parser_iseof(parser)) {
		return (NULL);
	}
	lhs = parse_word(parser);
	token = parser_peek(parser);
	if (!is_redirection(token.kind)) {
			return (lhs);
	}
	parser_next(parser);
	rhs = parse_word(parser);
	ast = ast_init(AstKind_Redirect);
	ast->redirect.kind = token.kind; 
	ast->redirect.lhs = lhs; 
	ast->redirect.rhs = rhs; 
	return (ast);
}

t_ast *parse_command(t_parser *parser)
{
	t_ast		*ast;
	t_ast		*red;

	if (parser_iseof(parser)) {
		return (NULL);
	}
	red = parse_redirect(parser);
	if (red == NULL) {
		return NULL;
	}
	ast = ast_init(AstKind_SimpleCommand);
	while (red) {
		ft_lstadd_front(&ast->simple_command.body, ft_lstnew(red));
		red = parse_redirect(parser);
	}
	return (ast);
}

t_ast *parse_pipe(t_parser *parser)
{
	t_ast		*ast;
	t_ast		*lhs;
	t_ast		*rhs;
	t_token	token;

	if (parser_iseof(parser)) {
		return (NULL);
	}
	lhs = parse_command(parser);
	token = parser_peek(parser);
	if (token.kind != TokenKind_Pipe) {
		return lhs; 
	}
	parser_next(parser);
	rhs = parse_command(parser);

	ast = ast_init(AstKind_Pipe);
	ast->pipe.lhs = lhs;
	ast->pipe.rhs = rhs;
	ast->pipe.children = NULL;

	//TODO(rnoba): make this recursive
	if (token.kind == TokenKind_Pipe) {
		ast->pipe.children = parse_pipe(parser);
	}
	return (ast);
}

void ast_print_state(t_ast *ast) {
	if (!ast) {
		return ; 
	}

	if (ast->kind == AstKind_Word) {
		t_ast_word word = ast->word;
		printf("%s\n", word.word);
	}
	else if (ast->kind == AstKind_Redirect)
	{
		printf("<Redirection>\n");
		t_ast_redirect redir = ast->redirect;
		ast_print_state(redir.lhs);
		ast_print_state(redir.rhs);
	}
	else if (ast->kind == AstKind_SimpleCommand)
	{
		printf("<SimpleCommand>\n");
		t_ast_simple_command scmd = ast->simple_command;
		t_list *l = scmd.body;
		while (l) {
			t_ast *red = l->content;
			ast_print_state(red);
			l = l->next;
		}
	}
	else if (ast->kind == AstKind_Pipe)
	{
		printf("<|>\n");
		t_ast_pipe pipe = ast->pipe;
		ast_print_state(pipe.lhs);
		ast_print_state(pipe.rhs);
		/*t_list *l = pipe.body;*/
		/*while (l) {*/
		/*	t_ast *cmd = l->content;*/
		/*	ast_print_state(cmd);*/
		/*	l = l->next;*/
		/*}*/
	}
}

void parse(char *line) {
	t_lexer lexer;
	t_parser parser;

	lexer = lexer_init(line);
	parser = parser_init(&lexer);
	parser_batch_tokens(&parser);

	t_ast *ast = parse_pipe(&parser);
	ast_print_state(ast);
}
