/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 17:33:40 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/07/14 17:36:07 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	execute_command(t_data *data, t_node *cur, char **env_array)
{
	char	*full_path;
	int		exit_code;

	exit_code = 0;
	if (cur->node_type == BUILT_IN)
	{
		if (execute_built_in(data, cur))
		{
			ft_free_matrix(env_array);
			shutdown_program(data);
			exit(0);
		}
		exit(CMD_NOT_FOUND);
	}
	else if (cur->cmd[0][0] == '/' || ft_strncmp(cur->cmd[0], "./", 2) == 0
		|| ft_strncmp(cur->cmd[0], "../", 3) == 0)
		full_path = ft_strdup(cur->cmd[0]);
	else
		full_path = ft_get_external_path(data, cur->cmd[0]);
	if (execve(full_path, cur->cmd, env_array) == -1)
	{
		exit_code = get_execve_exit_code(cur->cmd[0], full_path);
		free(full_path);
		ft_free_matrix(env_array);
		shutdown_program(data);
		exit(exit_code);
	}
	else
		data->exit_status = 0;
	ft_printf_fd(2, "minishell: %s: command not found\n", cur->cmd[0]);
	free(full_path);
}

static bool	handle_child(t_data *data, t_node *cur, int fd[2], int prev_fd)
{
	char	**env_array;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	dup_fds(cur);
	env_array = get_env_array(data->env_list);
	if (cur->prev == NULL)
	{
		if (cur->fd_out == -1)
			ft_dup_and_close(fd[1], STDOUT_FILENO, fd[0]);
	}
	else if (cur->next == NULL)
	{
		if (cur->fd_in == -1)
			ft_dup_and_close(prev_fd, STDIN_FILENO, fd[0]);
	}
	else
	{
		if (cur->fd_in == -1)
			ft_dup_and_close(prev_fd, STDIN_FILENO, fd[0]);
		if (cur->fd_out == -1)
			ft_dup_and_close(fd[1], STDOUT_FILENO, -1);
	}
	execute_command(data, cur, env_array);
	return (false);
}

bool	execute_external(t_data *data, t_node *cur)
{
	char	*full_path;
	char	**env_array;
	int		pid;
	int		status;
	int		wait_result;
	int		exit_code;

	status = 0;
	env_array = get_env_array(data->env_list);
	if (cur->cmd[0][0] == '/' || ft_strncmp(cur->cmd[0], "./", 2) == 0
		|| ft_strncmp(cur->cmd[0], "../", 3) == 0)
		full_path = ft_strdup(cur->cmd[0]);
	else
		full_path = ft_get_external_path(data, cur->cmd[0]);
	pid = fork();
	if (pid < 0)
		return (free_program(data, "Fork failed"));
	else if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if (execve(full_path, cur->cmd, env_array) == -1)
		{
			exit_code = get_execve_exit_code(cur->cmd[0], full_path);
			free(full_path);
			ft_free_matrix(env_array);
			shutdown_program(data);
			exit(exit_code);
		}
	}
	wait_result = waitpid(pid, &status, 0);
	if (wait_result != -1)
	{
		if (WIFEXITED(status))
			data->exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			data->exit_status = 128 + WTERMSIG(status);
	}
	free(full_path);
	ft_free_matrix(env_array);
	return (true);
}
