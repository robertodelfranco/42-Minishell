/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 10:06:03 by rafaelherin       #+#    #+#             */
/*   Updated: 2025/07/11 16:24:36 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static bool	is_valid_arg(const char *args)
{
	int	i;

	if (!args || !*args)
		return (false);
	if (!(ft_isalpha(args[0]) || args[0] == '_'))
		return (false);
	i = 0;
	while (args[i])
	{
		if (!(ft_isalnum(args[i]) || args[i] == '_'))
			return (false);
		i++;
	}
	return (true);
}

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
		if (!is_valid_arg(args[i]))
		{
			ft_printf_fd(2, "unset: `%s': not a valid identifier\n", args[i]);
			data->exit_status = 1;
		}
		else
			remove_var_from_env_list(data, args[i]);
		i++;
	}
	return (true);
}
