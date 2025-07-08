/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 15:20:25 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/07/08 16:50:16 by rdel-fra         ###   ########.fr       */
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

static int	handle_parent_no_wait(t_node *cur, int fd[2], int *prev_fd)
{
	if (*prev_fd != -1)
		close(*prev_fd);
	if (cur->next != NULL)
		close(fd[1]);
	if (cur->fd_in != -1)
		close(cur->fd_in);
	if (cur->fd_out != -1)
		close(cur->fd_out);
	*prev_fd = fd[0];
	return (0);
}

static int	count_commands(t_node *cur)
{
	int	count;

	count = 0;
	while (cur)
	{
		if (cur->node_type != PIPE)
			count++;
		cur = cur->next;
	}
	return (count);
}

static int	wait_for_all_children(pid_t *pids, int count)
{
	int	i;
	int	status;
	int	exit_code;

	exit_code = 0;
	i = 0;
	while (i < count)
	{
		waitpid(pids[i], &status, 0);
		if (WIFEXITED(status))
		{
			if (i == count - 1)
				exit_code = WEXITSTATUS(status);
		}
		i++;
	}
	return (exit_code);
}

bool	exec_multiple_cmd(t_data *data, t_node *cur, int fd[2], int prev_fd)
{
	int		i;
	int		last_cmd_error;
	pid_t	pid;
	pid_t	*pids;

	pids = malloc(sizeof(pid_t) * count_commands(cur));
	if (!pids)
		return (free_program(data, "Memory allocation failed"));
	i = 0;
	last_cmd_error = 0;
	while (cur)
	{
		if (cur->node_type == PIPE)
			cur = cur->next;
		if (cur->next && pipe(fd) == -1)
			return (free_program(data, "Pipe creation failed"));
		if (!identify_redirs(cur->redir, cur, data))
		{
			last_cmd_error = handle_redir_pipe_error(data, fd, cur, &prev_fd);
			cur = cur->next;
			continue ;
		}
		pid = fork();
		if (pid < 0)
			return (free_program(data, "Fork failed"));
		if (pid == 0)
			handle_child(data, cur, fd, prev_fd);
		pids[i++] = pid;
		handle_parent_no_wait(cur, fd, &prev_fd);
		cur = cur->next;
	}
	if (last_cmd_error)
		data->exit_status = 1;
	else
		data->exit_status = wait_for_all_children(pids, i);
	free(pids);
	return (true);
}
