/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 10:06:03 by rafaelherin       #+#    #+#             */
/*   Updated: 2025/07/17 16:04:46 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static bool	remove_var_from_env_list(t_data *data, const char *arg)
{
	t_env	*current;
	t_env	*prev;

	current = data->env_list;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->key, arg) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				data->env_list = current->next;
			free(current->key);
			if (current->value)
				free(current->value);
			free(current);
			return (true);
		}
		prev = current;
		current = current->next;
	}
	return (false);
}

bool	b_unset(t_data *data, char **args)
{
	int	i;

	if (!args[1] || !args)
	{
		data->exit_status = 0;
		return (true);
	}
	i = 1;
	while (args[i])
	{
		remove_var_from_env_list(data, args[i]);
		i++;
	}
	return (true);
}
