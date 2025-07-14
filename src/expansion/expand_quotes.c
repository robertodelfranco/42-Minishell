/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 18:13:28 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/07/14 15:56:25 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*get_variable_value(t_data *data, char *str)
{
	t_env	*cur;

	cur = data->env_list;
	if (str[0] == '\0')
		return (ft_strdup("$"));
	if (str[0] == '?')
		return (ft_itoa(data->exit_status));
	while (cur)
	{
		if (ft_strcmp(str, cur->key) == 0)
			return (ft_strdup(cur->value));
		cur = cur->next;
	}
	return (ft_strdup(""));
}

char	*get_variable_key(const char *str)
{
	int	i;

	i = 0;
	if (str[i] == '?')
		return (ft_strdup("?"));
	while (str[i] && !is_variable_delimiter(str[i]) && str[i] != '?')
		i++;
	if (i == 0 && str[i] != '\0')
		i = 1;
	return (ft_substr(str, 0, i));
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
