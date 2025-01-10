/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glima <gapima7@gmail.com>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 20:42:31 by glima             #+#    #+#             */
/*   Updated: 2025/01/08 22:11:50 by glima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	stat_path_eaccess(char *cmd, char **msg, \
int *status, struct stat st)
{
	*msg = "Command not found";
	if (S_ISREG(st.st_mode))
	{
		if (cmd[0] == '/' || (cmd[0] == '.' && cmd[1] == '/'))
		{
			*msg = "Permission denied";
			*status = 126;
		}
	}
	else
	{
		if (cmd[0] == '/' || (cmd[0] == '.' && cmd[1] == '/'))
		{
			*msg = "Is a directory";
			*status = 126;
		}
	}
}

int	stat_path(char *cmd, bool is_cmd)
{
	struct stat	st;
	int			status;
	char		*msg;

	stat(cmd, &st);
	status = 127;
	msg = "error";
	if (errno == EACCES)
		stat_path_eaccess(cmd, &msg, &status, st);
	else if (errno == ENOENT)
	{
		if (ft_strchr(cmd, '/') == NULL)
			msg = "Command not found";
		else
			msg = "No such file or directory";
		if (!is_cmd)
			msg = "No such file or directory";
	}
	ft_printf_fd(2, "shellzin: %s ", msg);
	ft_putendl_fd(cmd, 2);
	return (status);
}

static void	swap(char **a, char **b)
{
	char	*tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void	sort_tab(char **tab, int sz)
{
	int		idx;
	bool	sw;

	sw = true;
	while (sw)
	{
		idx = -1;
		sw = false;
		while (++idx < sz - 1)
		{
			if (ft_strncmp(tab[idx], tab[idx + 1], \
			ft_max(ft_strlen(tab[idx]), ft_strlen(tab[idx + 1]))) > 0)
			{
				swap(&tab[idx], &tab[idx + 1]);
				sw = true;
			}
		}
		sz--;
	}
}

void	token_print_state(t_token token)
{
	printf("(%s)", get_token_symbol(token));
	if (token.kind == TokenKind_Word || token.kind == TokenKind_StringLiteral)
		printf(": %s", token.content);
	printf("\n");
}
