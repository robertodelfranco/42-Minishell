/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 15:20:25 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/07/01 19:53:12 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static bool	handle_child(t_data *data, t_node *cur, int fd[2], int prev_fd)
{
	char	**env_array;

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
	ft_free_matrix(env_array);
	return (false);
}

static int	handle_parent(t_node *cur, int fd[2], int *prev_fd, pid_t pid)
{
	int	status;
	int	exit_code;

	exit_code = 0;
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		exit_code = WEXITSTATUS(status);
	if (*prev_fd != -1)
		close(*prev_fd);
	if (cur->next != NULL)
		close(fd[1]);
	if (cur->fd_in != -1)
		close(cur->fd_in);
	if (cur->fd_out != -1)
		close(cur->fd_out);
	*prev_fd = fd[0];
	return (exit_code);
}

bool	exec_multiple_cmd(t_data *data, t_node *cur, int fd[2], int prev_fd)
{
	pid_t	pid;

	while (cur)
	{
		if (cur->node_type == PIPE)
			cur = cur->next;
		if (cur->next && pipe(fd) == -1)
			return (free_program(data, "Pipe creation failed"));
		pid = fork();
		if (pid < 0)
			return (free_program(data, "Fork failed"));
		if (pid == 0)
			return (handle_child(data, cur, fd, prev_fd));
		data->exit_status = handle_parent(cur, fd, &prev_fd, pid);
		if (data->exit_status != 0)
			return (false);
		cur = cur->next;
	}
	return (true);
}

bool	execute_command(t_data *data, t_node *cur, char **env_array)
{
	char	*full_path;
	int		exit_code;

	exit_code = 0;
	if (cur->node_type == BUILT_IN)
	{
		if (execute_built_in(data, cur))
			exit(0);
		return (false);
	}
	else if (cur->cmd[0][0] == '.' && cur->cmd[0][0] == '/')
		full_path = ft_strdup(cur->cmd[0]);
	else
		full_path = ft_get_external_path(cur->cmd[0]);
	if (execve(full_path, cur->cmd, env_array) == -1)
	{
		exit_code = get_execve_exit_code(cur->cmd[0], full_path);
		free(full_path);
		exit(exit_code);
	}
	else
		data->exit_status = 0;
	free(full_path);
	return (true);
}
