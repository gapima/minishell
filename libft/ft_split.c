/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glima <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 13:02:26 by glima             #+#    #+#             */
/*   Updated: 2023/11/09 18:09:38 by glima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	**ft_div_words(char **str, size_t size, char *s, char c)
{
	size_t	count;
	size_t	cw;
	size_t	temp;

	count = 0;
	cw = 0;
	while (cw < size)
	{
		while (s[count] == c)
			count++;
		temp = count;
		while (s[temp] != c && s[temp] != '\0')
			temp++;
		str[cw] = malloc(sizeof(char) * (temp - count + 1));
		ft_memcpy((void *)str[cw], (const void *)&s[count], temp - count);
		str[cw][temp - count] = '\0';
		count = temp;
		cw++;
	}
	str[cw] = NULL;
	return (str);
}

char	**ft_split(char const *s, char c)
{
	char	**str;
	size_t	i;
	size_t	size;

	if (s == NULL)
		return (NULL);
	i = -1;
	size = 0;
	while (s[++i] != '\0')
	{
		if (s[i] != c && (s[i + 1] == c || s[i + 1] == '\0'))
			size++;
	}
	str = malloc(sizeof(char *) * (size + 1));
	if (str == NULL)
		return (NULL);
	str = ft_div_words(str, size, (char *)s, c);
	return (str);
}
