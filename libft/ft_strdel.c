#include "libft.h"\

void	ft_memdel(void **ap)
{
	if (ap)
	{
		free(*ap);
		*ap = NULL;
	}
}

void	ft_strdel(char **as)
{
	ft_memdel((void **)as);
}