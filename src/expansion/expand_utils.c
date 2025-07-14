/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 14:44:39 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/07/14 15:56:32 by rdel-fra         ###   ########.fr       */
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

void	ft_free_key_and_value(char *key, char *value)
{
	if (key)
		free(key);
	if (value)
		free(value);
}

int	is_variable_delimiter(char c)
{
	return ((c >= '\t' && c <= '\r') || (c >= ' ' && c <= '/')
		|| (c >= ':' && c <= '>') || (c >= '[' && c <= '^')
		|| (c >= '{' && c <= '~') || c == '$' || c == '@');
}
