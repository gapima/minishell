
#include "libft.h"

static int	ft_convert(char c, va_list arg)
{
	size_t	len;

	len = 0;
	if (c == 'c')
		len += ft_putchar(va_arg(arg, int));
	else if (c == 's')
		len += ft_putstr(va_arg(arg, char *));
	else if (c == 'p')
		len += ft_print_point(va_arg(arg, unsigned long), "0123456789abcdef");
	else if (c == 'd')
		len += ft_putnbr(va_arg(arg, int));
	else if (c == 'i')
		len += ft_putnbr(va_arg(arg, int));
	else if (c == 'u')
		len += ft_itoa_base(va_arg(arg, unsigned int), "0123456789");
	else if (c == 'x')
		len += ft_itoa_base(va_arg(arg, unsigned int), "0123456789abcdef");
	else if (c == 'X')
		len += ft_itoa_base(va_arg(arg, unsigned int), "0123456789ABCDEF");
	else if (c == '%')
		len += ft_putchar('%');
	return (len);
}

int	ft_printf(const char *s, ...)
{
	size_t	count;
	va_list	arg_ptr;
	size_t	len;

	count = -1;
	len = 0;
	va_start(arg_ptr, s);
	while (s[++count] != '\0')
	{
		if (s[count] == '%')
		{
			count++;
			while (s[count] == ' ')
				count++;
			len += ft_convert(s[count], arg_ptr);
		}
		else
			len += ft_putchar(s[count]);
	}
	va_end(arg_ptr);
	return (len);
}
