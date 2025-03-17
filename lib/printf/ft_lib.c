/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lib.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 15:57:21 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/03/14 14:56:28 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_printf_bonus.h"

void	ft_putchar(char c, int fd)
{
	write (fd, &c, 1);
}

void	ft_putstr(char *s, int fd)
{
	if (s == NULL)
		write(1, s, 6);
	write(fd, s, ft_strlen(s));
}

void	ft_putnbr(long n, int fd)
{
	long	nb;

	nb = n;
	if (n < 0)
	{
		nb = n * (-1);
		write(fd, "-", 1);
	}
	if (nb >= 10)
		ft_putnbr(nb / 10, fd);
	ft_putchar((nb % 10) + '0', fd);
}
