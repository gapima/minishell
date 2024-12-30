/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shellzin.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glima <gapima7@gmail.com>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 18:34:29 by glima             #+#    #+#             */
/*   Updated: 2024/12/29 18:38:09 by glima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
# include "../include/utils.h"

void	shellzin_handle_sigint(int sig)
{
	(void) sig;
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	ft_putstr("\n> ");
}

void	shellzin_init(t_shellzin *shell, char *envp[])
{
	ft_bzero(shell, sizeof(t_shellzin));
	while (*envp)
		ft_lstadd_back(&shell->env, ft_lstnew(*envp++));
}

char	*shellzin_env_search(t_shellzin *shell, const char *key)
{
	char	*ctt;
	t_list	*head;

	head = shell->env;
	while (head)
	{
		ctt = head->content;
		if (ft_strncmp(key, ctt, ft_strlen(key)) == 0)
			return (ft_strchr(ctt, '='));
		head = head->next;
	}
	return (NULL);
}

void	shellzin_deinit(t_shellzin *shell)
{
	ft_lst_destroy(shell->env);
}

void shellzin_assert(bool cond, char *msg)
{
	if (!cond) {
		ft_putendl_fd(msg, 2);
		exit(1);
	}
}

