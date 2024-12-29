#include "../include/minishell.h"
#include "../include/lexer.h"
#include "../include/utils.h"

bool lexer_iseof(t_lexer *lexer) {
	return lexer->cursor >= lexer->size;
}

char lexer_peek(t_lexer *lexer) {
	return lexer_iseof(lexer) ? '\0' : lexer->content[lexer->cursor];
}

t_lexer lexer_init(char *str) {
	t_lexer lexer;

	lexer.content = str;
	lexer.cursor = 0;
	lexer.size = ft_strlen(str);
	return (lexer);
}

char lexer_consume(t_lexer *lexer) {
	char curr = lexer_peek(lexer);
	if (!lexer_iseof(lexer)) {
		lexer->cursor = ft_min(lexer->cursor + 1, lexer->size);
	}
	return curr;
}

char lexer_consume_specific(t_lexer *lexer, char c) {
	bool r = false;
	if (lexer_peek(lexer) == c) {
		lexer_consume(lexer);
		r = true;
	}
	return r;
}

void lexer_skip_whitespaces(t_lexer *lexer) {
	while (!lexer_iseof(lexer) && ft_strchr(BLANKS, lexer_peek(lexer)) != NULL) {
		lexer_consume(lexer);
	}
}

char *get_token_symbol(t_token token) {
	if (token.kind == TokenKind_Eof) {
		return "<EOF>";
	}
	else if (token.kind == TokenKind_Word) {
		return "<WORD>";
	}
	else if (token.kind == TokenKind_Pipe) {
		return "|";
	}
	else if (token.kind == TokenKind_LArrow) {
		return "<";
	}
	else if (token.kind == TokenKind_RArrow) {
		return ">";
	}
	else if (token.kind == TokenKind_DRArrow) {
		return ">>";
	}
	else if (token.kind == TokenKind_DLArrow) {
		return "<<";
	}
	return NULL;
}

void lexer_print_state(t_lexer *lexer) {
	t_token token = lexer_next(lexer);

	while (token.kind != TokenKind_Eof) {
		printf("(%s)", get_token_symbol(token));
		if (token.kind == TokenKind_Word) {
			printf(": %s", token.content);
		}
		printf("\n");
		token = lexer_next(lexer);
	}
}

t_token lexer_next(t_lexer *lexer) {
	lexer_skip_whitespaces(lexer);

	t_token token;

	token.kind = TokenKind_Eof;
	token.content = NULL;
	token.start = lexer->cursor;
	token.end = lexer->size;

	if (lexer_iseof(lexer)) {
		return token;
	}

	char c = lexer_peek(lexer);
	if (c == '|') {
		lexer_consume_specific(lexer, '|');
		token.kind = TokenKind_Pipe;
		token.end = lexer->cursor;
	}
	else if (c == '<') {
		lexer_consume_specific(lexer, '<');
		token.kind = TokenKind_LArrow;
		if (lexer_consume_specific(lexer, '<')) {
			token.kind = TokenKind_DLArrow;
		}
		token.end = lexer->cursor;
	}
	else if (c == '>') {
		lexer_consume_specific(lexer, '>');
		token.kind = TokenKind_RArrow;
		if (lexer_consume_specific(lexer, '>')) {
			token.kind = TokenKind_DLArrow;
		}
		token.end = lexer->cursor;
	}
	else {
		token.kind = TokenKind_Word;
		while (!lexer_iseof(lexer) && ft_strchr(SPECIAL, lexer_peek(lexer)) == NULL) {
			lexer_consume(lexer);
		}
		token.end = lexer->cursor;
		token.content = ft_substr(lexer->content, token.start, token.end-token.start);
	}
	return token;
}
