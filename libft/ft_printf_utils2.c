/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glima <gapima7@gmail.com>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 18:01:08 by glima             #+#    #+#             */
/*   Updated: 2024/12/29 18:01:11 by glima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_write_str(char *s, const char *base, size_t len, long nbr)
{
	while (len--)
	{
		s[len] = base[nbr % ft_strlen(base)];
		nbr /= ft_strlen(base);
	}
}

size_t	ft_itoa_base(long nbr, const char *base, int fd)
{
	long	nbr_l;
	char	*tab;
	size_t	len;
	size_t	blen;
	int		lenc;

	len = 0;
	lenc = 0;
	if (nbr == 0)
		len = 1;
	nbr_l = nbr;
	blen = ft_strlen(base);
	while (nbr_l)
	{
		len++;
		nbr_l /= blen;
	}
	tab = malloc(sizeof(char) * (len + 1));
	if (!tab)
		return (0);
	tab[len] = '\0';
	ft_write_str(tab, base, len, nbr);
	lenc = ft_putstr_fd(tab, fd);
	free(tab);
	return (lenc);
}

int	ft_print_point(long nbr, const char *base, int fd)
{
	int	count;

	count = 0;
	if (nbr == 0)
		return (ft_putstr("(nil)"));
	count += write(fd, "0x", 2);
	count += ft_itoa_base(nbr, base, fd);
	return (count);
}
