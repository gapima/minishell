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
	t_ast	*ast;

	ast = parse(line, shell);
	if (shell->parser.has_error)
	{
		ft_printf("%s near `%s`\n",
						shell->parser.error_msg,
						get_token_symbol(parser_peek_last(&shell->parser)));
		if (shell->parser.error_from_lexer)
			parser_free_token_list(&shell->parser);
	}
	else if (!parser_iseof(&shell->parser))
	{
		ft_printf("shellzin: syntax error: Unexpected token near `%s`\n",
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
		if (shell->last_status == 131)
			ft_putendl_fd("Quit", 2);
		line = readline("> ");
		if (!line)
		{
			ft_putendl_fd("exit", 1);
			break ;
		}
		if (*line)
		{
			shellzin_evaluate(line, shell);
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
	/*shellzin_evaluate("ls -al \"./libft\"", &shell);*/
	/*shellzin_evaluate("touch \"a\" 'b' c", &shell);*/
	/*shellzin_evaluate("\"ls\" \"-al\"", &shell);*/
	/*shellzin_evaluate("rm -f a b c", &shell);*/
	/*shellzin_evaluate("touch \"a\"\"b\"\"c\" ", &shell);*/
	/*shellzin_evaluate("'ls'", &shell);*/
	/*shellzin_evaluate("rm -f 'a'\"b\"\"c\" ", &shell);*/
	/*shellzin_evaluate("'ls'", &shell);*/
	/*shellzin_evaluate("touch ls \"libft\"\"a\"\"b\"\"", &shell);*/
	/*shellzin_evaluate("echo \'\"a\"\'b\'\'c\'\"d\"\'", &shell);*/
	/*shellzin_evaluate("echo \"$HOME $HOME $HOME '$HOME'\" '$HOME'", &shell);*/
	/*shellzin_evaluate("\"\"", &shell);*/
	/*shellzin_evaluate("''", &shell);*/
	/*shellzin_evaluate("\"\"\"\"\"\"\"\"", &shell);*/
	/*shellzin_evaluate("\"", &shell);*/
	/*shellzin_evaluate("echo 'ok' 'a' 'b", &shell);*/
	/*shellzin_evaluate("\"\"\"'$HOME '$HOME' $HOME $HOME'\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"", &shell);*/
	/*shellzin_evaluate("\"\"\"\"\"\"\"\"\"\"\"\"\"'$HOME '$HOME' $HOME $HOME'\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"", &shell);*/
	/*shellzin_evaluate("\"\"\"\"\"\"\"\"\"\"\"\"\"\"'$HOME '$HOME' $HOME $HOME'\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"", &shell);*/
	/*shellzin_evaluate("echo a > b c d", &shell);*/
	/*shellzin_evaluate("cat b", &shell);*/
	/*shellzin_evaluate("cat b", &shell);*/
	/*shellzin_evaluate("echo po>oi<oi>tst", &shell);*/
	/*shellzin_evaluate("cat tst", &shell);*/
	shellzin_repl(&shell);
	shellzin_deinit(&shell);
}
