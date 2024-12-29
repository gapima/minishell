#include "libft.h"

static size_t	ft_count(int n)
{
	size_t	count;

	count = 0;
	if (n == 0)
		count++;
	if (n < 0)
	{
		n = -n;
		count++;
	}
	while (n > 0)
	{
		n = n / 10;
		count++;
	}
	return (count);
}

static char	*ft_return_str(int count, int n, char *s)
{
	int		nb;

	while (n > 0)
	{
		nb = '0' + (n % 10);
		n = n / 10;
		s[count] = nb;
		count--;
	}
	return (s);
}

char	*ft_itoa(int n)
{
	char	*str;
	int		count;

	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	count = ft_count(n);
	str = malloc(sizeof(char) * (count + 1));
	if (str == NULL)
		return (NULL);
	str[count] = '\0';
	if (n == 0)
		str[0] = '0';
	if (n < 0)
	{
		n = -n;
		str[0] = '-';
	}
	count--;
	return (ft_return_str(count, n, str));
}
