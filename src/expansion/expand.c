/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 17:40:55 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/06/04 19:39:41 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*get_variable_value(t_data *data, char *str)
{
	t_env	*cur;

	cur = data->env_list;
	if (str[0] == '\0')
		return (ft_strdup("$"));
	if (str[0] == '$')
		return (ft_itoa(getpid()));
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

char	*get_variable_key(const char *str, int *len)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != 32 && str[i] != '$')
		i++;
	if (i == 0 && str[i] != '\0')
		i = 1;
	*len = i;
	return (ft_substr(str, 0, i));
}

int	get_expand_size(t_data *data, const char *str)
{
	int		key_len;
	char	*value;
	char	*key;
	int		size;
	int		j;

	size = 0;
	j = 0;
	while (str[j])
	{
		if (str[j] == '$')
		{
			key = get_variable_key(&str[j + 1], &key_len);
			value = get_variable_value(data, key);
			size += ft_strlen(value);
			ft_free_key_and_value(key, value);
			j += key_len + 1;
		}
		else
		{
			size++;
			j++;
		}
	}
	return (size);
}

char	*get_str_expanded(t_data *data, t_token *cur, char *expand)
{
	char	*key;
	char	*value;
	int		key_len;
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (cur->value[i])
	{
		if (cur->value[i] == '$')
		{
			key = get_variable_key(&cur->value[i + 1], &key_len);
			value = get_variable_value(data, key);
			copy_value(expand, value, &j);
			free(key);
			free(value);
			i += key_len + 1;
		}
		else
			expand[j++] = cur->value[i++];
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
		if (cur->value && (cur->type == EXPAND || cur->type == WORD
				|| cur->type == DOUB_QUOTE))
		{
			if (ft_strchr(cur->value, '$'))
			{
				expand = ft_calloc(get_expand_size(data, cur->value) + 1, 1);
				if (!expand)
					return (free_program(data, "calloc error"));
				new_str = get_str_expanded(data, cur, expand);
				free(cur->value);
				cur->value = new_str;
			}
		}
		cur = cur->next;
	}
	return (true);
}
