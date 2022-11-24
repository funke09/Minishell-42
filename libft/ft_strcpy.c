#include "libft.h"

char	*ft_strcpy(char *dst, const char *src)
{
	char	*dest;
	size_t	i;

	dest = dst;
	i = 0;
	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dst);
}