/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 18:13:28 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/07/11 15:34:51 by rdel-fra         ###   ########.fr       */
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

int	jump_quotes(t_data *data, const char *str, int *size)
{
	char	quote;
	int		i;

	i = 1;
	quote = str[0];
	if (str[1] == quote)
	{
		*size += 2;
		return (2);
	}
	if (quote == '\"')
	{
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
	}
	else if (quote == '\'')
	{
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

static int	ft_double_quotes(t_data *data, char *input, char *expand, int *j)
{
	char	*key;
	char	*value;
	char	quote;
	int		i;

	i = 1;
	quote = input[0];
	while (input[i] && input[i] != quote)
	{
		if (input[i] == '$' && !is_variable_delimiter(input[i + 1]))
		{
			key = get_variable_key(&input[i + 1]);
			value = get_variable_value(data, key);
			copy_value(expand, value, j);
			i += ft_strlen(key) + 1;
			ft_free_key_and_value(key, value);
		}
		else
			expand[(*j)++] = input[i++];
	}
	if (input[i] == quote)
		i++;
	return (i);
}

int	copy_quotes(t_data *data, char *input, char *str_expand, int *j)
{
	char	quote_type;
	int		i;

	i = 0;
	quote_type = input[i];
	if (quote_type == input[i + 1])
		return (2);
	if (quote_type == '\"')
		return (ft_double_quotes(data, input, str_expand, j));
	else if (quote_type == '\'')
	{
		i++;
		while (input[i] && input[i] != quote_type)
		{
			str_expand[*j] = input[i];
			(*j)++;
			i++;
		}
		if (input[i] == quote_type)
			i++;
	}
	return (i);
}
