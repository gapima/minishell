/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glima <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 17:10:18 by glima             #+#    #+#             */
/*   Updated: 2023/10/19 16:26:57 by glima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	char		*destmp;
	const char	*srctmp;

	destmp = dest;
	srctmp = src;
	if (dest > src)
	{
		while (n--)
			destmp[n] = srctmp[n];
	}
	else
		ft_memcpy(dest, src, n);
	return (dest);
}
