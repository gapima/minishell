/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shellzin_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glima <gapima7@gmail.com>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 23:38:33 by glima             #+#    #+#             */
/*   Updated: 2025/01/08 23:39:06 by glima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	shellzin_handle_sigint(int sig)
{
	g_last_signal = sig;
	if (shellzin_is_heredoc(0, 0))
		close(STDIN_FILENO);
	rl_replace_line("", 0);
	rl_on_new_line();
	ft_putstr("\n");
	if (shellzin_redisplay(0, 0))
		rl_redisplay();
}

void	shellzin_init(t_shellzin *shell, char *envp[])
{
	ft_bzero(shell, sizeof(t_shellzin));
	shell->seed = 12345;
	getcwd(shell->cwd, PATH_MAX);
	while (*envp)
		ft_lstadd_back(&shell->env, ft_lstnew(ft_strdup(*envp++)));
}

t_list	*shellzin_env_search_node(t_shellzin *shell, \
const char *key, t_list **p_ptr, bool r_prev)
{
	t_list	*head;
	t_list	*prev;

	prev = NULL;
	head = shell->env;
	while (head)
	{
		if (ft_strncmp(key, head->content, ft_strlen(key)) == 0)
			break ;
		prev = head;
		head = head->next;
	}
	if (prev && r_prev)
		*p_ptr = prev;
	return (head);
}

bool	shellzin_set_or_change_env(t_shellzin *shell, char *key, char *value)
{
	t_list	*node;
	char	*new;

	new = ft_strjoin(ft_strdup(key), "=");
	new = ft_strjoin(new, value);
	node = shellzin_env_search_node(shell, key, NULL, false);
	if (node)
	{
		free(node->content);
		node->content = new;
		return (true);
	}
	ft_lstadd_back(&shell->env, ft_lstnew(new));
	return (false);
}

char	*shellzin_env_search(t_shellzin *shell, const char *key)
{
	char	*ctt;
	char	*env;
	t_list	*head;

	head = shell->env;
	env = NULL;
	while (head)
	{
		ctt = head->content;
		if (ft_strncmp(key, ctt, ft_strlen(key)) == 0)
		{
			env = ft_strchr(ctt, '=');
			if (env)
				env = env + 1;
		}
		head = head->next;
	}
	return (env);
}
