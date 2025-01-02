/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glima <gapima7@gmail.com>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 18:18:19 by glima             #+#    #+#             */
/*   Updated: 2024/12/29 18:18:56 by glima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../include/minishell.h"

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

void ft_lst_destroy(t_list *list)
{
	t_list	*head;
	t_list	*next;

	head = list; 
	while (head)
	{
		next = head->next;
		free(head);
		head = next;
	}
}

void *ft_realloc(void *m, size_t prev_size, size_t new_size)
{
	void *ret;

	ret = NULL;
	if (new_size < prev_size) {
		free(m);
		return (ret);
	}
	ret = ft_calloc(new_size, 1);
	shellzin_assert(ret != NULL, "(ft_realloc) could not allocate memory");
	if (m != NULL) {
		ft_memcpy(ret, m, prev_size);
	}
	free(m);
	return (ret);
}
