/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glima <gapima7@gmail.com>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 18:16:36 by glima             #+#    #+#             */
/*   Updated: 2025/01/08 20:07:23 by glima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

volatile int	g_last_signal = 0;

void	shellzin_evaluate(char *line, t_shellzin *shell)
{
	t_ast	*ast;

	ast = parse(line, shell);
	if (shell->parser.has_error)
	{
		ft_printf_fd(2, "%s near `%s`\n", shell->parser.error_msg, \
		get_token_symbol(parser_peek_last(&shell->parser)));
		if (shell->parser.error_from_lexer)
			parser_free_token_list(&shell->parser);
	}
	else if (!parser_iseof(&shell->parser))
	{
		ft_printf_fd(2, "shellzin: syntax error: Unexpected token near `%s`\n", \
		get_token_symbol(parser_peek(&shell->parser)));
	}
	else if (ast)
	{
		shell->ast = ast;
		ast_evaluate(shell, ast);
	}
	parser_deinit(&shell->parser);
	ast_deinit(ast);
}

void	shellzin_repl(t_shellzin *shell)
{
	char	*line;

	signal(SIGINT, shellzin_handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		g_last_signal = 0;
		if (shell->last_status == 131)
			ft_putendl_fd("Quit", 2);
		shellzin_redisplay(true, 1);
		line = readline("shellzin$ ");
		shellzin_redisplay(false, 1);
		if (!line)
			shellzin_evaluate("exit", shell);
		shellzin_evaluate(line, shell);
		if (*line)
		{
			add_history(line);
		}
		free(line);
	}
}

int	main(int ac, char *av[], char *envp[])
{
	t_shellzin	shell;

	(void)ac;
	(void)av;
	shellzin_init(&shell, envp);
	shellzin_repl(&shell);
	shellzin_deinit(&shell);
}
