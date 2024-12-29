/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glima <gapima7@gmail.com>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 17:35:44 by glima             #+#    #+#             */
/*   Updated: 2024/12/29 17:55:04 by glima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# define BLANKS " \t"
# define SPECIAL " \t<>|"

enum	e_token_kind
{
	TokenKind_Eof,
	TokenKind_Word,
	TokenKind_StringLiteral,
	TokenKind_LArrow,
	TokenKind_RArrow,
	TokenKind_DLArrow,
	TokenKind_DRArrow,
	TokenKind_Pipe,
};

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

t_lexer	lexer_init(char *str);
t_token	lexer_next(t_lexer *lexer);
char	*get_token_symbol(t_token token);
void	lexer_print_state(t_lexer *lexer);
#endif
