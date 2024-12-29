/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glima <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 17:27:09 by glima             #+#    #+#             */
/*   Updated: 2023/10/31 11:02:19 by glima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*array;
	size_t	i;

	i = 0;
	if (start > ft_strlen((char *)s))
		return (ft_strdup(""));
	if (start + len > ft_strlen((char *)s))
		len = ft_strlen((char *)s) - start;
	array = malloc(sizeof(char) * (len + 1));
	if (array == NULL)
		return (NULL);
	while (i < len)
	{
		array[i] = s[start];
		i++;
		start++;
	}
	array[i] = '\0';
	return (array);
}
