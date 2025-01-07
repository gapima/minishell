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
	if (argv[1] && *argv[1] \
	&& ft_strncmp(argv[1], "-n", ft_strlen(argv[1])) == 0)
	{
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
