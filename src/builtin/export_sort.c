/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_sort.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rheringe <rheringe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 15:36:43 by rheringe          #+#    #+#             */
/*   Updated: 2025/06/30 17:59:07 by rheringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	count_env_vars(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		count++;
		env = env->next;
	}
	return (count);
}

static void	fill_env_array(t_env **array, t_env *env)
{
	int	i;

	i = 0;
	while (env)
	{
		array[i++] = env;
		env = env->next;
	}
	array[i] = NULL;
}

static void	sort_env_array(t_env **array, int len)
{
	int		i;
	int		j;
	t_env	*tmp;

	i = 0;
	while (i < len - 1)
	{
		j = 0;
		while (j < len - i - 1)
		{
			if (ft_strcmp(array[j]->key, array[j + 1]->key) > 0)
			{
				tmp = array[j];
				array[j] = array[j + 1];
				array[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
}

static void	print_sorted_env(t_env **array)
{
	int	i;

	i = 0;
	while (array[i])
	{
		if (array[i]->value)
			printf("declare -x %s=\"%s\"\n", array[i]->key, array[i]->value);
		else
			printf("declare -x %s\n", array[i]->key);
		i++;
	}
}

void	print_export(t_data *data)
{
	int			len;
	t_env		**sorted;
	t_env		*current;
	t_env		*next;

	if (ft_envdup(data))
		return ;
	len = count_env_vars(data->env_copy);
	sorted = malloc(sizeof(t_env *) * (len + 1));
	if (!sorted)
		return ;
	fill_env_array(sorted, data->env_copy);
	sort_env_array(sorted, len);
	print_sorted_env(sorted);
	free(sorted);
	current = data->env_copy;
	while (current)
	{
		next = current->next;
		free(current->key);
		free(current->value);
		free(current);
		current = next;
	}
	data->env_copy = NULL;
}
