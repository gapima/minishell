/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glima <gapima7@gmail.com>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 18:54:38 by glima             #+#    #+#             */
/*   Updated: 2025/01/07 18:55:48 by glima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	**join_string_list(t_list *list)
{
	char	**str_list;
	size_t	idx;

	str_list = ft_calloc(ft_lstsize(list) + 1, sizeof(char *));
	shellzin_assert(str_list != NULL, "(ft_realloc) could not allocate memory");
	idx = 0;
	while (list)
	{
		str_list[idx++] = (char *)list->content;
		list = list->next;
	}
	str_list[idx] = NULL;
	return (str_list);
}

char	**join_word_list(t_list *list)
{
	char	**str_list;
	t_ast	*node;
	size_t	idx;

	str_list = ft_calloc(ft_lstsize(list) + 1, sizeof(char *));
	shellzin_assert(str_list != NULL, "(ft_realloc) could not allocate memory");
	idx = 0;
	while (list)
	{
		node = list->content;
		str_list[idx++] = node->u_node.word_node.content;
		list = list->next;
	}
	str_list[idx] = NULL;
	return (str_list);
}

void	string_list_destroy(char **list)
{
	char	**ptr;

	ptr = list;
	while (*ptr)
	{
		free(*ptr);
		ptr++;
	}
	free(list);
}
