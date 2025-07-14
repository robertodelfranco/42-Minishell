/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_quotes_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 15:49:15 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/07/14 16:10:35 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	get_variable_size(t_data *data, const char *str, int start_pos)
{
	char	*key;
	char	*value;
	int		var_size;

	key = get_variable_key(&str[start_pos + 1]);
	value = get_variable_value(data, key);
	var_size = ft_strlen(value);
	ft_free_key_and_value(key, value);
	return (var_size);
}

static int	get_variable_key_length(const char *str)
{
	int	i;

	i = 0;
	if (str[i] == '?')
		return (1);
	while (str[i] && !is_variable_delimiter(str[i]) && str[i] != '?')
		i++;
	if (i == 0 && str[i] != '\0')
		i = 1;
	return (i);
}

static	int	copy_double_quotes(t_data *data, char *str, int *size)
{
	char	quote;
	int		i;

	quote = str[0];
	i = 1;
	while (str[i] && str[i] != quote)
	{
		if (str[i] == '$' && !is_variable_delimiter(str[i + 1]))
		{
			*size += get_variable_size(data, str, i);
			i += get_variable_key_length(&str[i + 1]) + 1;
		}
		else
		{
			(*size)++;
			i++;
		}
	}
	if (str[i] == quote)
	{
		(*size)++;
		i++;
	}
	return (i);
}

int	jump_quotes(t_data *data, char *str, int *size)
{
	char	quote;
	int		i;

	i = 0;
	quote = str[0];
	if (str[1] == quote)
	{
		*size += 2;
		return (2);
	}
	if (quote == '\"')
		i += copy_double_quotes(data, str, size);
	else if (quote == '\'')
	{
		i++;
		while (str[i] && str[i] != quote)
		{
			(*size)++;
			i++;
		}
	}
	if (str[i] == quote)
		i++;
	return (i);
}
