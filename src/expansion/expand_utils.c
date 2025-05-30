/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 14:44:39 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/05/30 15:11:38 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_ptr_len(char **str)
{
	int	i;
	int	j;
	int	total;

	i = 0;
	total = 0;
	while (str[i])
	{
		j = 0;
		while (str[i][j])
		{
			total++;
			j++;
		}
		i++;
	}
	return (total);
}

int	ft_strchr_count(char const *str, char c)
{
	int		i;
	int		count;

	i = 0;
	count = 0;
	while (str[i])
	{
		if (str[i] == c)
			count += 1;
		i++;
	}
	if (str[i] == c)
		count += 1;
	return (count);
}

void	ft_free_key_and_value(char *key, char *value)
{
	if (key)
		free(key);
	if (value)
		free(value);
}

void	copy_value(char *str_expand, char *value, int *j)
{
	int	i;
	int	c;

	i = 0;
	c = *j;
	while (value[i])
		str_expand[c++] = value[i++];
	*j = c;
}
