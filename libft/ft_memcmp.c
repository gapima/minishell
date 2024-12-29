/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gapima <gapima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 14:40:00 by gapima            #+#    #+#             */
/*   Updated: 2023/10/22 14:58:19 by gapima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	const char	*s1tmp;
	const char	*s2tmp;
	int			i;

	i = 0;
	s1tmp = s1;
	s2tmp = s2;
	while (n)
	{
		if (s1tmp[i] != s2tmp[i])
			return ((unsigned char)s1tmp[i] - (unsigned char)s2tmp[i]);
		i++;
		n--;
	}
	return (0);
}
