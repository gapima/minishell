/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glima <gapima7@gmail.com>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 18:00:07 by glima             #+#    #+#             */
/*   Updated: 2024/12/29 18:00:08 by glima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	char	*array;
	size_t	full_size;

	full_size = size * nmemb;
	array = malloc(full_size);
	if (array == NULL)
		return (NULL);
	ft_bzero(array, full_size);
	return (array);
}
