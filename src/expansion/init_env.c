/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 17:41:55 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/05/23 14:29:51 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	add_env_list(t_data *data, t_env *new_node)
{
	t_env	*last;

	if (!data->env_list)
		data->env_list = new_node;
	else
	{
		last = data->env_list;
		while (last->next)
			last = last->next;
		last->next = new_node;
	}
}

void	ft_init_env(t_data *data, char **env)
{
	char	**temp;
	t_env	*new_node;
	int		i;

	i = 0;
	(void)data;
	while (env[i])
	{
		temp = ft_split(env[i], '=');
		if (temp)
		{
			new_node = ft_calloc(1, sizeof(t_env));
			if (!new_node)
				return ;
			new_node->key = ft_strdup(temp[0]);
			new_node->value = ft_strdup(temp[1]);
			add_env_list(data, new_node);
			ft_free_matrix(temp);
		}
		i++;
	}
}
