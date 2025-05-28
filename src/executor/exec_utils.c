/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 15:22:19 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/05/28 15:22:23 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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

char	**get_env_array(t_env *env_list)
{
	char	**env_array;
	int		i;

	env_array = ft_calloc((ft_listsize(env_list) + 1), sizeof(char *));
	if (!env_array)
		return (NULL);
	i = 0;
	while (i < ft_listsize(env_list))
	{
		if (env_list->key && env_list->value)
			env_array[i] = ft_strjoin_three(env_list->key, "=",
					env_list->value);
		else if (env_list->key)
			env_array[i] = ft_strjoin(env_list->key, "=");
		else
			env_array[i] = NULL;
		env_list = env_list->next;
		i++;
	}
	return (env_array);
}

char	*ft_get_external_path(char *token_name)
{
	int		i;
	char	*full_cmd;
	char	*full_path;
	char	**new_path;

	new_path = ft_split(getenv("PATH"), ':');
	if (new_path == NULL)
		return (0);
	i = 0;
	while (new_path[i])
	{
		full_path = ft_strjoin(new_path[i++], "/");
		full_cmd = ft_strjoin(full_path, token_name);
		if (access(full_cmd, X_OK) == 0)
		{
			free(full_path);
			ft_free_matrix(new_path);
			return (full_cmd);
		}
		free(full_cmd);
		free(full_path);
	}
	ft_free_matrix(new_path);
	return (NULL);
}
