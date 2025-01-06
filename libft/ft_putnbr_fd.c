/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glima <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 18:42:10 by glima             #+#    #+#             */
/*   Updated: 2023/11/08 19:05:45 by glima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putnbr_fd(int n, int fd)
{
	char	c;
	size_t	count;

	count = 0;
	if (n == -2147483648)
		count += ft_putstr_fd("-2147483648", fd);
	else if (n > 9)
	{
		count += ft_putnbr_fd(n / 10, fd);
		c = (n % 10 + '0');
		count += ft_putchar_fd(c, fd);
	}
	else if (n >= 0 && n <= 9)
	{
		c = (n + '0');
		count += ft_putchar_fd(c, fd);
	}
	else
	{
		count += ft_putchar_fd('-', fd);
		count += ft_putnbr_fd(n * -1, fd);
	}
	return (count);
}
