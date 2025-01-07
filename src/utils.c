/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glima <gapima7@gmail.com>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 18:18:19 by glima             #+#    #+#             */
/*   Updated: 2025/01/07 19:41:49 by glima            ###   ########.fr       */
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

void	*ft_realloc(void *m, size_t prev_size, size_t new_size)
{
	void	*ret;

	ret = NULL;
	if (new_size < prev_size)
	{
		free(m);
		return (ret);
	}
	ret = ft_calloc(new_size, 1);
	shellzin_assert(ret != NULL, "(ft_realloc) could not allocate memory");
	if (m != NULL)
		ft_memcpy(ret, m, prev_size);
	free(m);
	return (ret);
}

char	*search_directory(char *path, char *str)
{
	DIR				*dir;
	struct dirent	*ent;
	char			*ret;

	if (!path)
		return (NULL);
	ret = NULL;
	dir = opendir(path);
	if (dir)
	{
		ent = readdir(dir);
		while (ent)
		{
			if (ft_strncmp(ent->d_name, str, \
			ft_max(ft_strlen(ent->d_name), ft_strlen(str))) == 0)
			{
				ret = ft_strjoin(ft_strdup(path), "/");
				ret = ft_strjoin(ret, str);
				break ;
			}
			ent = readdir(dir);
		}
		closedir(dir);
	}
	return (ret);
}

char	*search_path(t_shellzin *shell, char *str)
{
	char	**split;
	char	*path_;
	char	*ret;
	char	**ptr;

	path_ = shellzin_env_search(shell, "PATH");
	split = ft_split(path_, ':');
	if (!split)
		return (NULL);
	ptr = split;
	while (*ptr)
	{
		ret = search_directory(*ptr++, str);
		if (ret)
			break ;
	}
	string_list_destroy(split);
	return (ret);
}

int	is_regular_file(const char *path)
{
	struct stat	path_stat;

	stat(path, &path_stat);
	if (errno == ENOENT)
		return (-1);
	else if (errno == EACCES)
		return (-2);
	return (S_ISREG(path_stat.st_mode));
}
