/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafaelheringer <rafaelheringer@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 15:19:09 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/07/07 13:16:22 by rafaelherin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

bool	execute_built_in(t_data *data, t_node *cur)
{
	t_type	type;

	if (!cur->cmd)
		return (true);
	type = ft_get_cmd_type(cur->cmd[0]);
	dup_fds(cur);
	if (type == ECHO)
		echo(cur->cmd);
	else if (type == CD)
		return (cd(data, cur->cmd));
	else if (type == PWD)
		pwd(data);
	else if (type == ENV)
		env(data, cur->cmd);
	else if (type == EXIT)
	{
		b_exit(data, cur->cmd);
		return (false);
	}
	else if (type == EXPORT)
		return (b_export(data, cur->cmd));
	else if (type == UNSET)
		return (b_unset(data, cur->cmd));
	else
		return (false);
	return (true);
}

bool	execute_external(t_data *data, t_node *cur)
{
	char	*full_path;
	char	**env_array;
	int		pid;

	env_array = get_env_array(data->env_list);
	pid = fork();
	if (cur->cmd[0][0] == '.' || cur->cmd[0][0] == '/')
		full_path = ft_strdup(cur->cmd[0]);
	else
		full_path = ft_get_external_path(cur->cmd[0]);
	if (pid < 0)
		return (free_program(data, "Fork failed"));
	else if (pid == 0)
	{
		if (execve(full_path, cur->cmd, env_array) == -1)
		{
			free(full_path);
			ft_free_matrix(env_array);
			exit(data->exit_status);
		}
	}
	waitpid(pid, NULL, 0);
	free(full_path);
	ft_free_matrix(env_array);
	return (true);
}
