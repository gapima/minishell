
#include "libft.h"

char	*ft_strjoin(char *s1, char *s2)
{
	char	*result;
	int		i;
	int		count;

	i = 0;
	count = 0;
	result = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!result)
		return (NULL);
	while (s1 && s1[i])
	{
		result[i] = s1[i];
		i++;
	}
	while (s2 && s2[count])
	{
		result[i] = s2[count];
		count++;
		i++;
	}
	result[i] = '\0';
	free(s1);
	return (result);
}
