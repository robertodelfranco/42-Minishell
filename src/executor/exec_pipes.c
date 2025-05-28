/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 15:20:25 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/05/28 15:20:30 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

bool	execute_first_command(t_data *data, t_node *cur, int fd[2])
{
	char	**env_array;
	char	*full_path;

	dup2(fd[1], STDOUT_FILENO);
	close(fd[0]);
	close(fd[1]);
	env_array = get_env_array(data->env_list);
	if (cur->node_type == BUILT_IN)
	{
		if (execute_built_in(data, cur))
		{
			ft_free_matrix(env_array);
			b_exit(data, cur->cmd);
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
	free(full_path);
	free(env_array);
	return (true);
}

bool	execute_middle_command(t_data *data, t_node *cur, int fd[2], int prev)
{
	char	**env_array;
	char	*full_path;

	dup2(prev, STDIN_FILENO);
	dup2(fd[1], STDOUT_FILENO);
	close(prev);
	close(fd[0]);
	close(fd[1]);
	env_array = get_env_array(data->env_list);
	if (cur->node_type == BUILT_IN)
	{
		if (execute_built_in(data, cur))
		{
			ft_free_matrix(env_array);
			b_exit(data, cur->cmd);
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

bool	execute_last_command(t_data *data, t_node *cur, int fd[2], int prev_fd)
{
	char	**env_array;
	char	*full_path;

	dup2(prev_fd, STDIN_FILENO);
	close(prev_fd);
	close(fd[0]);
	env_array = get_env_array(data->env_list);
	if (cur->node_type == BUILT_IN)
	{
		if (execute_built_in(data, cur))
		{
			ft_free_matrix(env_array);
			b_exit(data, cur->cmd);
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
