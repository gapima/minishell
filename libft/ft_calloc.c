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
