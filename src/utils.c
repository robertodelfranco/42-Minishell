/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 17:36:50 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/07/14 17:45:53 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*ft_get_external_path(t_data *data, char *token_name)
{
	int		i;
	char	*full_cmd;
	char	*full_path;
	char	**new_path;
	char	*value;

	value = get_variable_value(data, "PATH");
	new_path = ft_split(value, ':');
	free(value);
	i = 0;
	while (new_path[i])
	{
		full_path = ft_strjoin(new_path[i++], "/");
		full_cmd = ft_strjoin(full_path, token_name);
		free(full_path);
		if (access(full_cmd, X_OK) == 0)
		{
			ft_free_matrix(new_path);
			return (full_cmd);
		}
		free(full_cmd);
	}
	ft_free_matrix(new_path);
	return (NULL);
}

t_node	*get_last_command_node(t_node *cur)
{
	t_node	*last_cmd;

	last_cmd = NULL;
	while (cur)
	{
		if (cur->node_type != PIPE)
			last_cmd = cur;
		cur = cur->next;
	}
	return (last_cmd);
}

int	ft_listsize(t_env *list)
{
	t_env	*ptr;
	int		i;

	i = 0;
	ptr = list;
	while (ptr != NULL)
	{
		ptr = ptr->next;
		i++;
	}
	return (i);
}
