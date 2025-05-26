/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rheringe <rheringe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 13:55:37 by rheringe          #+#    #+#             */
/*   Updated: 2025/05/26 15:09:24 by rheringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

static int	convertions_fd(int fd, const char *str, int i, va_list arg)
{
	int	count;

	count = 0;
	if (str[i] == 'c')
		count += ft_putchar_fd(va_arg(arg, int), fd);
	else if (str[i] == 's')
		count += ft_putstr_fd(va_arg(arg, char *), fd);
	return (count);
}

int	ft_printf_fd(int fd, const char *str, ...)
{
	va_list	t_args;
	int		str_len;
	int		count;
	int		i;

	if (!str)
		return (0);
	i = -1;
	count = 0;
	str_len = ft_strlen((char *)str);
	va_start(t_args, str);
	while (++i < str_len)
	{
		if (str[i] == '%')
		{
			i += 1;
			count += convertions_fd(fd, str, i, t_args);
		}
		else
			count += ft_putchar_fd(str[i], fd);
	}
	va_end(t_args);
	return (count);
}
