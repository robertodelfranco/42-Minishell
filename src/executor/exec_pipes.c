/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 15:20:25 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/05/30 15:45:49 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_dup_and_close(int fd, int dup, int clos)
{
	dup2(fd, dup);
	close(fd);
	if (clos != -1)
		close(clos);
}

bool	execute_first_command(t_data *data, t_node *cur)
{
	char	**env_array;
	char	*full_path;

	env_array = get_env_array(data->env_list);
	if (cur->node_type == BUILT_IN)
	{
		if (execute_built_in(data, cur))
		{
			ft_free_matrix(env_array);
			b_exit(data, NULL);
		}
		return (false);
	}
	else if (cur->cmd[0][0] == '.' && cur->cmd[0][1] == '/')
		full_path = ft_strdup(cur->cmd[0]);
	else
		full_path = ft_get_external_path(cur->cmd[0]);
	if (execve(full_path, cur->cmd, env_array) == -1)
	{
		perror("execve");
		exit(free_program(data, "Command execution failed"));
	}
	return (true);
}

bool	execute_middle_command(t_data *data, t_node *cur)
{
	char	**env_array;
	char	*full_path;

	env_array = get_env_array(data->env_list);
	if (cur->node_type == BUILT_IN)
	{
		if (execute_built_in(data, cur))
		{
			ft_free_matrix(env_array);
			b_exit(data, NULL);
		}
		return (false);
	}
	else if (cur->cmd[0][0] == '.' && cur->cmd[0][1] == '/')
		full_path = ft_strdup(cur->cmd[0]);
	else
		full_path = ft_get_external_path(cur->cmd[0]);
	if (execve(full_path, cur->cmd, env_array) == -1)
	{
		perror("execve");
		exit(free_program(data, "Command execution failed"));
	}
	return (true);
}

bool	execute_last_command(t_data *data, t_node *cur)
{
	char	**env_array;
	char	*full_path;

	env_array = get_env_array(data->env_list);
	if (cur->node_type == BUILT_IN)
	{
		if (execute_built_in(data, cur))
		{
			ft_free_matrix(env_array);
			b_exit(data, NULL);
		}
		return (false);
	}
	else if (cur->cmd[0][0] == '.' && cur->cmd[0][1] == '/')
		full_path = ft_strdup(cur->cmd[0]);
	else
		full_path = ft_get_external_path(cur->cmd[0]);
	if (execve(full_path, cur->cmd, env_array) == -1)
	{
		perror("execve");
		exit(free_program(data, "Command execution failed"));
	}
	return (true);
}
