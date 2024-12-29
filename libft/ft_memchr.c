/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gapima <gapima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 13:58:47 by gapima            #+#    #+#             */
/*   Updated: 2023/10/25 18:20:27 by glima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*stmp;
	int				itmp;

	itmp = -1;
	stmp = (unsigned char *)s;
	while (n)
	{
		if (stmp[n - 1] == (unsigned char)c)
			itmp = (n - 1);
		n--;
	}
	if (itmp > -1)
		return ((void *)&stmp[itmp]);
	return (NULL);
}
