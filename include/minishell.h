/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glima <gapima7@gmail.com>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 18:16:30 by glima             #+#    #+#             */
/*   Updated: 2025/01/07 19:46:56 by glima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"

# include <stdio.h>
# include <stdbool.h>
# include <unistd.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include <errno.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <stdlib.h>

# define BLANKS " \t\v\f\n\v\f\r"
# define SPECIAL " \v\f\n\v\f\r\t<>|\'\""

extern volatile int	g_last_signal;

typedef enum e_token_kind
{
	TokenKind_Eof,
	TokenKind_Word,
	TokenKind_Error,
	TokenKind_StringLiteral,
	TokenKind_LArrow,
	TokenKind_RArrow,
	TokenKind_DLArrow,
	TokenKind_DRArrow,
	TokenKind_Pipe
}	e_token_kind;

typedef struct s_token
{
	e_token_kind	kind;
	char			*content;
	size_t			start;
	size_t			end;
}	t_token;

typedef struct s_lexer
{
	size_t	cursor;
	size_t	size;
	char	*content;
}	t_lexer;

t_lexer		lexer_init(char *str);
t_token		lexer_next(t_lexer *lexer);
void		lexer_print_state(t_lexer *lexer);
bool		lexer_iseof(t_lexer *lexer);
char		lexer_peek(t_lexer *lexer);
char		lexer_consume(t_lexer *lexer);
void		lexer_consume_until(t_lexer *lexer, char c);
void		token_print_state(t_token token);
char		*get_token_symbol(t_token token);

typedef struct s_parser
{
	t_lexer *lexer;
	t_token *tokens;
	size_t 	size;
	size_t 	cursor;
	bool 	has_error;
	char 	*error_msg;
	bool 	error_from_lexer;
	bool 	should_expand;
}	t_parser;

t_parser	parser_init(t_lexer *lexer);
void		parser_deinit(t_parser *parser);
void		parser_batch_tokens(t_parser *parser);
bool		parser_iseof(t_parser *parser);
void		parser_free_token_list(t_parser *parser);
t_token		parser_peek(t_parser *parser);
t_token		parser_peek_last(t_parser *parser);
void		parser_set_error(t_parser *parser, char *msg);

typedef enum e_ast_kind
{
	AstKind_Eof,
	AstKind_List,
	AstKind_Word,
	AstKind_Redirect,
	AstKind_Pipe,
}	e_ast_kind;

typedef struct	e_ast t_ast;

typedef struct e_ast_word
{
	char	*content;
	bool	is_expanded;
	bool	is_string;
}	t_ast_word;

typedef struct e_ast_list
{
	t_list	*list;
}	t_ast_list;

typedef struct e_ast_redirect
{
	t_ast			*left;
	t_ast			*right;
	e_token_kind	kind;
	int				fd;
}	t_ast_redirect;

typedef struct e_ast_pipe
{
	t_ast	*left;
	t_ast	*right;
}	t_ast_pipe;

struct e_ast 
{
	e_ast_kind	kind;
	bool		is_error;
	union {
		t_ast_word word_node;
		t_ast_redirect redirect_node;
		t_ast_list list_node;
		t_ast_pipe pipe_node;
	} u_node;
};

typedef struct s_shellzin
{
	int			last_status;
	t_list		*env;
	t_lexer		lexer;
	t_parser	parser;
	t_ast		*ast;
	bool		stop_evaluation;
	char		cwd[PATH_MAX];
}	t_shellzin;

int		is_regular_file(const char *path);

void	ast_deinit(t_ast *ast);
t_ast	*ast_init(e_ast_kind kind);
void	ast_evaluate(t_shellzin *shell, t_ast *ast);

t_ast	*parse(char *line, t_shellzin *shell);

void	shellzin_handle_sigint(int sig);
void	shellzin_init(t_shellzin *shell, char *envp[]);
void	shellzin_deinit(t_shellzin *shell);
void	shellzin_assert(bool cond, char *msg);
char	*shellzin_env_search(t_shellzin *shell, const char *key);
t_list	*shellzin_env_search_node(t_shellzin *shell, const char *key, t_list **p_ptr, bool r_prev);

int		ft_min(int a, int b);
int		ft_max(int a, int b);
void	ft_lst_destroy(t_list *list);
void	*ft_realloc(void *m, size_t prev_size, size_t new_size);

void	string_list_destroy(char **list);
char	**join_string_list(t_list *list);
char	**join_word_list(t_list *list);
char	*search_path(t_shellzin *shell, char *str);

bool	shellzin_set_or_change_env(t_shellzin *shell, char *key, char *value);
bool	shellzin_try_run_builtin(char **argv, t_shellzin *shell);
void	shellzin_cd(char **argv, t_shellzin *shell);
void	shellzin_env(char **argv, t_shellzin *shell);
void	shellzin_unset(char **argv, t_shellzin *shell);
void	shellzin_pwd(char **argv, t_shellzin *shell);
void	shellzin_echo(char **argv, t_shellzin *shell);
void	shellzin_export(char **argv, t_shellzin *shell);
void	shellzin_exit(char **argv, t_shellzin *shell);

bool	string_try_expand(char **str, size_t size, t_shellzin *shell);
void	shellzin_heredoc(t_ast *ast, t_shellzin *shell);

bool	shellzin_redisplay(bool v, int s);
bool	shellzin_is_heredoc(bool v, int s);
#endif
