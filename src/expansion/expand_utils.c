/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 14:44:39 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/07/02 15:36:38 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	process_variable(t_data *data, const char *str, int *i);

static int	handle_quotes_double(t_data *data, char *input, char *str_expand, int *j)
{
	char	quote;
	int		i;

	i = 0;
	quote = input[i];
	while (input[i] && input[i] != quote)
	{
		if (input[i] == '$' && input[i + 1] != '\0' && input[i + 1] != ' ')
			i += process_variable(data, &input[i], j);
		else
		{
			str_expand[*j] = input[i];
			(*j)++;
			i++;
		}
	}
	return (i + 1);
}

int	copy_quotes(t_data *data, char *input, char *str_expand, int *j)
{
	char	quote_type;
	int		start;
	int		i;

	i = 0;
	quote_type = input[i];
	if (quote_type == '\"')
		return (handle_quotes_double(data, input, str_expand, j));
	else
	{
		i++;
		start = *j;
		while (input[i] != quote_type)
		{
			str_expand[*j] = input[i];
			(*j)++;
			i++;
		}
	}
	return (i + 1);
}

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
