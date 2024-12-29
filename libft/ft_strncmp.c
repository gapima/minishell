
#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	c;

	if (n == 0)
		return (0);
	c = 0;
	while ((s1[c] && s2[c]) != '\0' && s1[c] == s2[c] && c < n - 1)
		c++;
	return ((s1[c] - s2[c]));
}
