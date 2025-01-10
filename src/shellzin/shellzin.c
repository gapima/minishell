/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shellzin.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glima <gapima7@gmail.com>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 18:34:29 by glima             #+#    #+#             */
/*   Updated: 2025/01/08 23:38:59 by glima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	shellzin_deinit(t_shellzin *shell)
{
	rl_clear_history();
	ft_lst_destroy(shell->env);
	ft_lst_destroy(shell->shell_variables);
}

bool	shellzin_redisplay(bool v, int s)
{
	static bool	redisplay;

	if (s == 0)
		return (redisplay);
	redisplay = v;
	return (redisplay);
}

bool	shellzin_is_heredoc(bool v, int s)
{
	static bool	heredoc;

	if (s == 0)
		return (heredoc);
	heredoc = v;
	return (heredoc);
}

void	shellzin_assert(bool cond, char *msg)
{
	if (!cond)
	{
		ft_putendl_fd(msg, 2);
		exit(1);
	}
}
