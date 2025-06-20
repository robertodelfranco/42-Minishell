/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_findchar.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 16:01:29 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/06/20 16:26:03 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

char	ft_findchar(const char *s, char *find)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
	{
		if (ft_strchr(find, s[i]))
			if (ft_strchr(s, s[i]))
				return (s[i]);
		i++;
	}
	if (ft_strchr(find, '\0'))
		return (s[i]);
	return (0);
}
