/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_flags.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 12:13:52 by marvin            #+#    #+#             */
/*   Updated: 2025/03/14 14:56:28 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_printf_bonus.h"

void	ft_initialize_flags(t_flags *flags)
{
	(*flags).sign = 0;
	(*flags).space = 0;
	(*flags).hash = 0;
}

size_t	ft_check_flags(char *s, t_flags *flags)
{
	size_t	j;

	j = 0;
	while (s[j] == '#' || s[j] == ' ' || s[j] == '+')
	{
		if (s[j] == ' ')
			(*flags).space = 1;
		else if (s[j] == '+')
			(*flags).sign = 1;
		else if (s[j] == '#')
			(*flags).hash = 1;
		j++;
	}
	return (j + 2);
}
