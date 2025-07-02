/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 17:40:55 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/07/02 15:36:30 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	copy_quotes(t_data *data, char *input, char *str_expand, int *j);

static char	*get_variable_value(t_data *data, char *str)
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

static char	*get_variable_key(const char *str)
{
	int	i;

	i = 0;
	if (str[i] == '?')
		return (ft_strdup("?"));
	while (str[i] && str[i] != 32 && str[i] != '$')
		i++;
	if (i == 0 && str[i] != '\0')
		i = 1;
	return (ft_substr(str, 0, i));
}

int	process_variable(t_data *data, const char *str, int *i)
{
	char	*key;
	char	*value;
	int		var_size;

	key = get_variable_key(&str[*i + 1]);
	value = get_variable_value(data, key);
	var_size = ft_strlen(value);
	*i += ft_strlen(key) + 1;
	ft_free_key_and_value(key, value);
	return (var_size);
}

int	jump_quotes(t_data *data, const char *str, int *size)
{
	char	quote;
	int		i;

	i = 0;
	quote = str[i];
	if (quote == '\"')
	{
		i++;
		while (str[i] && str[i] != quote)
		{
			if (str[i] == '$' && str[i + 1] != '\0' && str[i + 1] != ' ')
				*size += process_variable(data, str, &i);
			else
			{
				(*size)++;
				i++;
			}
		}
	}
	else
	{
		while (str[++i] && str[i] != quote)
			(*size)++;
	}
	return (i + 1);
}

int	get_expand_size(t_data *data, const char *str)
{
	int		size;
	int		i;

	i = 0;
	size = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
			i += jump_quotes(data, &str[i], &size);
		if (str[i] == '$' && str[i + 1] != '\0' && str[i + 1] != ' ')
			size += process_variable(data, str, &i);
		else
		{
			size++;
			i++;
		}
	}
	return (size);
}

char	*get_str_expanded(t_data *data, char *input, char *expand)
{
	char	*key;
	char	*value;
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (input[i])
	{
		if (input[i] == '\'' || input[i] == '\"')
			i += copy_quotes(data, &input[i], expand, &j);
		if (input[i] == '$' && input[i + 1] != '\0' && input[i + 1] != ' ')
		{
			key = get_variable_key(&input[i + 1]);
			value = get_variable_value(data, key);
			copy_value(expand, value, &j);
			i += ft_strlen(key) + 1;
			ft_free_key_and_value(key, value);
		}
		else
			expand[j++] = input[i++];
	}
	return (expand);
}

bool	ft_expand(t_data *data)
{
	t_token	*cur;
	char	*new_str;
	char	*expand;

	cur = data->token_list;
	while (cur)
	{
		if (ft_strcmp(cur->value, "<<") == 0)
			cur = cur->next->next;
		if (cur && cur->value)
		{
			if (ft_strchr(cur->value, '$'))
			{
				expand = ft_calloc(get_expand_size(data, cur->value) + 1, 1);
				if (!expand)
					return (free_program(data, "calloc error"));
				new_str = get_str_expanded(data, cur->value, expand);
				free(cur->value);
				cur->value = new_str;
			}
		}
		if (cur)
			cur = cur->next;
	}
	return (true);
}
