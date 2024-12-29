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

#include "../include/lexer.h"
#include "../include/minishell.h"

void	shellzin_repl(t_shellzin *shell)
{
	char	*line;
	t_lexer	lexer;

	signal(SIGINT, shellzin_handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	(void)shell;
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			ft_putendl_fd("exit", 1);
			break ;
		}
		lexer = lexer_init(line);
		lexer_print_state(&lexer);
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
