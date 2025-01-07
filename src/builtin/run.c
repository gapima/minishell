/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glima <gapima7@gmail.com>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 19:55:29 by glima             #+#    #+#             */
/*   Updated: 2025/01/07 19:56:34 by glima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

bool	shellzin_try_run_builtin(char **argv, t_shellzin *shell)
{
	bool	is_builtin;
	size_t	len;

	len = ft_strlen(argv[0]);
	is_builtin = false;
	if (len <= 1)
		return (false);
	if (ft_strncmp(argv[0], "cd", len) == 0)
	{
		shellzin_cd(argv, shell);
		is_builtin = true;
	}
	else if (ft_strncmp(argv[0], "unset", len) == 0)
	{
		shellzin_unset(argv, shell);
		is_builtin = true;
	}
	else if (ft_strncmp(argv[0], "env", len) == 0)
	{
		shellzin_env(argv, shell);
		is_builtin = true;
	}
	else if (ft_strncmp(argv[0], "exit", len) == 0)
	{
		shellzin_exit(argv, shell);
		is_builtin = true;
	}
	else if (ft_strncmp(argv[0], "pwd", len) == 0)
	{
		shellzin_pwd(argv, shell);
		is_builtin = true;
	}
	else if (ft_strncmp(argv[0], "echo", len) == 0)
	{
		shellzin_echo(argv, shell);
		is_builtin = true;
	}
	else if (ft_strncmp(argv[0], "export", len) == 0)
	{
		shellzin_export(argv, shell);
		is_builtin = true;
	}
	return (is_builtin);
}
