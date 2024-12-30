#include "../include/ast.h"
#include "../include/minishell.h"

t_ast *ast_init(e_ast_kind kind) {
	t_ast	*ast;

	ast = ft_calloc(sizeof(t_ast), 1);
	shellzin_assert(ast != NULL, "(ast_init) could not allocate memory");
	ast->kind = kind;
	return (ast);
}
