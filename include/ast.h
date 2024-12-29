#ifndef AST_H
# define AST_H

#include "../include/minishell.h"
#include "../include/lexer.h"

typedef enum e_ast_kind
{
	AstKind_Eof,
	AstKind_Word,
	AstKind_Redirect,
	AstKind_SimpleCommand,
	AstKind_Pipe,
} e_ast_kind;

typedef struct e_ast t_ast;

typedef struct e_ast_word
{
	char *word;
} t_ast_word;

typedef struct e_ast_redirect
{
	t_ast *children;
	t_list *lhs;
	t_list *rhs;
} t_ast_redirect;

typedef struct e_ast_simple_command
{
	t_list *body;
} t_ast_simple_command;

typedef struct e_ast_pipe
{
	t_list *body;
} t_ast_pipe;

struct e_ast 
{
	e_ast_kind kind;
	union {
		t_ast_word word;
		t_ast_redirect redirect;
		t_ast_simple_command simple_command;
		t_ast_pipe pipe;
	};
};

#endif
