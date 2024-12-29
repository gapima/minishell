/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glima <gapima7@gmail.com>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 17:59:57 by glima             #+#    #+#             */
/*   Updated: 2024/12/29 17:59:58 by glima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t		idx;
	char const	*srctmp;
	char		*destmp;

	destmp = dest;
	srctmp = src;
	idx = -1;
	if (destmp == NULL && srctmp == NULL)
		return (NULL);
	while (++idx < n)
		destmp[idx] = srctmp[idx];
	return (dest);
}
