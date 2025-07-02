/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 14:44:39 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/07/02 18:32:04 by rdel-fra         ###   ########.fr       */
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
	while (str[i] && str[i] != 32 && str[i] != '$'
		&& str[i] != '\'' && str[i] != '\"')
		i++;
	if (i == 0 && str[i] != '\0')
		i = 1;
	return (ft_substr(str, 0, i));
}
