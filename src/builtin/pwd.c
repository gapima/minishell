/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glima <gapima7@gmail.com>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 19:57:18 by glima             #+#    #+#             */
/*   Updated: 2025/01/07 19:57:19 by glima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	shellzin_pwd(char **argv, t_shellzin *shell)
{
	if (argv[1] && argv[2] != NULL)
	{
		shell->last_status = 1;
		ft_printf_fd(2, "shellzin: pwd: too many arguments\n");
		return ;
	}
	ft_printf("%s\n", shell->cwd);
	shell->last_status = 0;
}
