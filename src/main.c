/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glima <gapima7@gmail.com>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 18:16:36 by glima             #+#    #+#             */
/*   Updated: 2024/12/29 18:36:53 by glima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	shellzin_evaluate(char *line, t_shellzin *shell)
{
	t_ast *ast;

	ast = parse(line, shell);
	ast_print_state(ast, 0);
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
		line = readline("> ");
		if (!line)
		{
			ft_putendl_fd("exit", 1);
			break ;
		}
		shellzin_evaluate(line, shell);
		if (*line)
			add_history(line);
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
