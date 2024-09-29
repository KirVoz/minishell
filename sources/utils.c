#include "minishell.h"

void	*ft_realloc_exe(void *ptr, size_t size)
{
	void	*new_ptr;

	if (ptr == NULL)
		return (malloc(size));
	if (size == 0)
	{
		free(ptr);
		return (NULL);
	}
	new_ptr = malloc(size);
	if (new_ptr == NULL)
		return (NULL);
	ft_memcpy(new_ptr, ptr, size);
	free(ptr);
	return (new_ptr);
}

int count_str(char **str)
{
	int i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

char    *str_to_lower(char *str)
{
	char	*lower_str;
    int 	i;

	lower_str = ft_strdup(str);
    i = 0;
    while (lower_str[i])
    {
        lower_str[i] = ft_tolower(lower_str[i]);
        i++;
    }
    return (lower_str);
}