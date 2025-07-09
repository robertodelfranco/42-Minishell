/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rheringe <rheringe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 17:40:55 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/07/09 17:18:44 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	process_variable(t_data *data, const char *str, int *i)
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

int	get_exp_size(t_data *data, const char *str, bool heredoc)
{
	int		size;
	int		i;

	i = 0;
	size = 0;
	while (str[i])
	{
		if (heredoc == false && (str[i] == '\'' || str[i] == '\"'))
			i += jump_quotes(data, &str[i], &size);
		else if (str[i] == '$' && str[i + 1] != '\0' && str[i + 1] != ' ')
			size += process_variable(data, str, &i);
		else
		{
			size++;
			i++;
		}
	}
	return (size);
}

char	*get_str_expand(t_data *data, char *input, char *expand, bool heredoc)
{
	char	*key;
	char	*value;
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (input[i])
	{
		if (heredoc == false && (input[i] == '\'' || input[i] == '\"'))
			i += copy_quotes(data, &input[i], expand, &j);
		else if (input[i] == '$' && input[i + 1] != '\0' && input[i + 1] != ' ')
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
	char	*exp;

	cur = data->token_list;
	while (cur)
	{
		if (ft_strcmp(cur->value, "<<") == 0)
			cur = cur->next->next;
		if (cur && cur->value)
		{
			if (ft_strchr(cur->value, '$') || ft_strchr(cur->value, '\"')
				|| ft_strchr(cur->value, '\''))
			{
				exp = ft_calloc(get_exp_size(data, cur->value, false) + 1, 1);
				if (!exp)
					return (free_program(data, "calloc error"));
				new_str = get_str_expand(data, cur->value, exp, false);
				free(cur->value);
				cur->value = new_str;
			}
		}
		if (cur)
			cur = cur->next;
	}
	return (true);
}
