/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rheringe <rheringe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 12:21:38 by rheringe          #+#    #+#             */
/*   Updated: 2025/06/02 15:52:29 by rheringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int		ft_envdup(t_data *data);
void	print_export(t_data *data);

static t_env	*create_env_node(const char *key, const char *value)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->key = ft_strdup(key);
	if (value)
		new_node->value = ft_strdup(value);
	else
		new_node->value = NULL;
	new_node->next = NULL;
	if (!new_node->key || (value && !new_node->value))
	{
		free(new_node->key);
		free(new_node->value);
		free(new_node);
		return (NULL);
	}
	return (new_node);
}

static void	append_env_node(t_env **head, t_env *new_node)
{
	t_env	*last;

	if (!*head)
		*head = new_node;
	else
	{
		last = *head;
		while (last->next)
			last = last->next;
		last->next = new_node;
	}
}

int	ft_envdup(t_data *data)
{
	t_env	*current;
	t_env	*new_node;

	data->env_copy = NULL;
	current = data->env_list;
	while (current)
	{
		new_node = create_env_node(current->key, current->value);
		if (!new_node)
			return (1);
		append_env_node(&data->env_copy, new_node);
		current = current->next;
	}
	return (0);
}

void	b_export(t_data *data, char **argv)
{
	if (!argv[1])
	{
		print_export(data);
		return ;
	}
}
