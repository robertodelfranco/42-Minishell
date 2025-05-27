/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 14:39:51 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/05/27 16:23:20 by rdel-fra         ###   ########.fr       */
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
	{
		perror("Fork failed");
		free(full_path);
		ft_free_matrix(env_array);
		return (false);
	}
	else if (pid == 0)
	{
		char *full = ft_strjoin_three(full_path, " ", cur->cmd[0]);
		printf("fullpathcmd: %s\n", full);
		free(full);
		if (execve(full_path, cur->cmd, env_array) == -1)
		{
			free(full_path);
			ft_free_matrix(env_array);
			free_program(data, NULL);
			exit(1);
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

	cur = data->exec_list;
	if (cur->next == NULL)
		return (execute_one_command(data, cur));
	while (cur)
	{
		if (cur->node_type == PIPE)
			printf("Pipe\n");
		else if (cur->node_type == EXTERNAL)
			execute_external(data, cur);
		else if (cur->node_type == EXPAND)
		{
			printf("Expand\n");
			printf("%s\n", getenv(&cur->cmd[0][1]));
		}
		else
		{
			printf("Built-in\n");
			execute_built_in(data, cur);
		}
		cur = cur->next;
	}
	return (true);
}
