
#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int		len;

	if (!s)
		return (NULL);
	len = ft_strlen(s);
	while (len)
	{
		if (s[len] == (unsigned char)c)
		{
			return ((char *)&s[len]);
		}
		len--;
	}
	if (s[len] == (unsigned char)c)
		return ((char *)&s[len]);
	return (NULL);
}
