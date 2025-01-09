/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glima <gapima7@gmail.com>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 19:48:40 by glima             #+#    #+#             */
/*   Updated: 2025/01/07 19:49:35 by glima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	shellzin_echo(char **argv, t_shellzin *shell)
{
	size_t	idx;
	bool	nl;

	idx = 1;
	nl = true;
	while (argv[idx] &&
		argv[idx][0] == '-' &&
		argv[idx][1] == 'n')
	{
		if (argv[idx][2] != '\0' && argv[idx][2] != 'n')
			break;
		idx += 1;
		nl = false;
	}
	while (argv[idx])
	{
		ft_putstr(argv[idx]);
		if (argv[idx + 1] != NULL)
			ft_putchar_fd(' ', 1);
		idx++;
	}
	if (nl)
		ft_putchar_fd('\n', 1);
	shell->last_status = 0;
}
