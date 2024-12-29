/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glima <gapima7@gmail.com>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 18:16:36 by glima             #+#    #+#             */
/*   Updated: 2024/12/27 18:16:37 by glima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../include/minishell.h"
#include "../include/lexer.h"

void shellzin_handle_sigint(int sig) {
	(void) sig;
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	ft_putstr("\n> ");
}

void shellzin_init(t_shellzin *shell, char *envp[]) {
	while (*envp) {
		ft_lstadd_back(&shell->env, ft_lstnew(*envp++));
	}
}

char *shellzin_env_search(t_shellzin *shell, const char *key) {
	t_list *head = shell->env;

	while (head) {
		char *ctt = head->content;
		if (ft_strncmp(key, ctt, ft_strlen(key)) == 0) {
			return (ft_strchr(ctt, '='));
		}
		head = head->next;
	}
	return NULL;
}

void shellzin_deinit(t_shellzin *shell) {
	t_list *head = shell->env;

	while (head) {
		t_list *next = head->next;
		free(head);
		head = next;
	}
}

void shellzin_repl(t_shellzin *shell) {
	signal(SIGINT, shellzin_handle_sigint);
	signal(SIGQUIT, SIG_IGN);

	(void)shell;
	while (1) {
		char *line = readline("> ");
		if (!line) {
			ft_putendl_fd("exit", 1);
			break;
		}
		t_lexer lexer = lexer_init(line);
		lexer_print_state(&lexer);

		free(line);
	}
}

int main(int ac, char *av[], char *envp[]) {
	(void)ac;
	(void)av;
	t_shellzin shell = {0};

	shellzin_init(&shell, envp);
	shellzin_repl(&shell);
	shellzin_deinit(&shell);
}
