/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 14:39:51 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/05/28 13:58:22 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

bool	execute_built_in(t_data *data, t_node *cur)
{
	t_type	type;

	type = ft_get_cmd_type(cur->cmd[0]);
	if (type == ECHO)
		echo(cur->cmd);
	else if (type == ENV)
		env(data, cur->cmd);
	else if (type == CD)
		cd(data, cur->cmd);
	else if (type == PWD)
		pwd(data);
	else if (type == EXIT)
		b_exit(data, cur->cmd);
	else
	{
		printf("Unknown built-in command: %s\n", cur->cmd[0]);
		return (false);
	}
	return (true);
}

bool	execute_external(t_data *data, t_node *cur)
{
	char	*full_path;
	char	**env_array;
	int		pid;

	env_array = get_env_array(data->env_list);
	pid = fork();
	if (cur->cmd[0][0] == '.' && cur->cmd[0][1] == '/')
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
			b_exit(data, cur->cmd);
		}
	}
	waitpid(pid, NULL, 0);
	free(full_path);
	ft_free_matrix(env_array);
	return (true);
}

bool	execute_one_command(t_data *data, t_node *cur)
{
	if (cur->node_type == BUILT_IN)
	{
		if (execute_built_in(data, cur))
			return (true);
	}
	else if (cur->node_type == EXTERNAL)
	{
		if (execute_external(data, cur))
			return (true);
	}
	else if (cur->node_type == EXPAND)
	{
		ft_printf_fd(2, "bash: %s: No such file or directory\n",
			getenv(&cur->cmd[0][1]));
		return (false);
	}
	else
	{
		printf("Unknown command type: %d\n", cur->node_type);
		return (false);
	}
	return (false);
}

bool	executor(t_data *data)
{
	t_node	*cur;
	pid_t	pid;
	int		fd[2];
	int		prev_fd;

	cur = data->exec_list;
	if (cur->next == NULL)
		return (execute_one_command(data, cur));
	prev_fd = -1;
	while (cur)
	{
		// if (cur->node_type == PIPE)
		// 	cur = cur->next;
		if (cur->next != NULL)
			if (pipe(fd) == -1)
				return (free_program(data, "Pipe creation failed"));
		pid = fork();
		if (pid < 0)
			return (free_program(data, "Fork failed"));
		if (pid == 0)
		{
			if (cur->prev == NULL)
			{
				perror("here first ");
				execute_first_command(data, cur, fd);
			}
			else if (cur->next == NULL)
			{
				perror("here last ");
				execute_last_command(data, cur, fd, prev_fd);
			}
			else
			{
				perror("here middle ");
				execute_middle_command(data, cur, fd, prev_fd);
			}
		}
		else
		{
			if (prev_fd != -1)
				close(prev_fd);
			if (cur->next != NULL)
				close(fd[1]);
			prev_fd = fd[0];
			waitpid(pid, NULL, 0);
		}
		cur = cur->next;
	}
	return (true);
}
