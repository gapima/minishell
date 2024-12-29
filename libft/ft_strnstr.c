/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gapima <gapima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 15:36:02 by gapima            #+#    #+#             */
/*   Updated: 2023/10/25 18:40:11 by glima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	b;
	size_t	l;

	b = 0;
	l = 0;
	if (!little[l])
		return ((char *)big);
	while (big[b] && b < len)
	{
		l = 0;
		while ((little[l] == big[b + l] && little[l]) && l + b < len)
			l++;
		if (!little[l])
			return ((char *)&big[b]);
		b++;
	}
	return (NULL);
}
