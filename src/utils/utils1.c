/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glima <gapima7@gmail.com>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 20:15:18 by glima             #+#    #+#             */
/*   Updated: 2025/01/08 20:42:29 by glima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_min(int a, int b)
{
	if (a > b)
		return (b);
	return (a);
}

int	ft_max(int a, int b)
{
	if (a > b)
		return (a);
	return (b);
}

void	ft_lst_destroy(t_list *list)
{
	t_list	*head;
	t_list	*next;

	head = list;
	while (head)
	{
		next = head->next;
		free(head->content);
		free(head);
		head = next;
	}
}

int	open_dup2_close(char *path, int flags, int d, int *fd)
{
	*fd = open(path, flags, 0666);
	if (*fd == -1)
	{
		stat_path(path, false);
		return (1);
	}
	if (dup2(*fd, d) == -1)
		return (1);
	close(*fd);
	return (0);
}
