/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glima <gapima7@gmail.com>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 18:00:15 by glima             #+#    #+#             */
/*   Updated: 2024/12/29 18:00:16 by glima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	char	*duplicate;
	size_t	len;

	len = ft_strlen(s) + 1;
	duplicate = (char *)malloc(len);
	if (duplicate == NULL)
		return (NULL);
	ft_memcpy(duplicate, s, len);
	return (duplicate);
}
