/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zcherrad <zcherrad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 16:27:03 by zcherrad          #+#    #+#             */
/*   Updated: 2021/11/27 16:40:23 by zcherrad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>
#include <stdarg.h>

int	ft_printf(char	*str, ...)
{
	va_list		ap;
	int			i;

	va_start(ap, str);
	i = 0;
	while (str[i])
	{
		if (str[i] == '%')
		{
			i++;
			if (str[i] == '%')
				ft_putchar_fd('%', 1);
			else if (str[i] == 'd' || str[i] == 'i')
				ft_putnbr_fd(va_arg(ap, int), 1);
			else if (str[i] == 's')
				ft_putstr_fd(va_arg(ap, char *), 1);
			else if (str[i] == 'c')
				ft_putchar_fd(va_arg(ap, int), 1);
			i++;
		}
		else
		{
			ft_putchar_fd(str[i], 1);
			i++;
		}
	}
	va_end(ap);
	return (0);
}

int	main(void)
{
	ft_printf("%d hey %s %c", 192, "hello", 'c');
	return (0);
}
