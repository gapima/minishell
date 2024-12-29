
#include "libft.h"

static char	*ft_get_line(int fd, char *str)
{
	char	*temp;
	int		temp_len;

	temp = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!temp)
		return (NULL);
	temp_len = 1;
	while (ft_strrchr(str, '\n') == NULL && temp_len != 0)
	{
		temp_len = read(fd, temp, BUFFER_SIZE);
		if (temp_len < 0)
		{
			free(temp);
			return (NULL);
		}
		temp[temp_len] = '\0';
		str = ft_strjoin(str, temp);
	}
	free(temp);
	return (str);
}

static char	*ft_current_line(char *str, char *line)
{
	int	count;

	count = 0;
	if (str[count] == '\0')
		return (NULL);
	while (str[count] != '\n' && str[count] != '\0')
		count++;
	if (str[count] == '\n')
		line = malloc((count + 2) * sizeof(char));
	else if (str[count] == '\0')
		line = malloc((count + 1) * sizeof(char));
	if (!line)
		return (NULL);
	count = -1;
	while (str[++count] != '\n' && str[count] != '\0')
		line[count] = str[count];
	if (str[count] == '\n')
		line[count++] = '\n';
	line[count] = '\0';
	return (line);
}

static char	*ft_get_line_rest(char *str)
{
	size_t		count;
	size_t		str_len;
	size_t		rest_count;
	char		*rest;

	count = 0;
	while (str[count] != '\0' && str[count] != '\n')
		count++;
	if (str[count] == '\0')
	{
		free(str);
		return (NULL);
	}
	str_len = ft_strlen(str);
	rest = malloc(sizeof(char) * (str_len - count));
	if (!rest)
		return (NULL);
	count++;
	rest_count = 0;
	while (str[count] != '\0')
		rest[rest_count++] = str[count++];
	rest[rest_count] = '\0';
	free(str);
	return (rest);
}

char	*get_next_line(int fd)
{
	static char		*next_line;
	char			*current_line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	next_line = ft_get_line(fd, next_line);
	if (!next_line)
		return (NULL);
	current_line = NULL;
	current_line = ft_current_line(next_line, current_line);
	next_line = ft_get_line_rest(next_line);
	return (current_line);
}
