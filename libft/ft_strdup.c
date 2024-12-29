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
