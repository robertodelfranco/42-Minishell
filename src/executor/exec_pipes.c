/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 15:20:25 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/07/14 18:22:46 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	handle_redir_pipe(t_data *data, int fd[2], t_node **cur, int *prev)
{
	t_node	*last_node;
	int		last;

	last_node = get_last_command_node(data->exec_list);
	last = 0;
	if (*cur == last_node)
		last = 1;
	if (*prev != -1)
		close(*prev);
	if ((*cur)->next != NULL)
		close(fd[1]);
	if ((*cur)->fd_in != -1)
		close((*cur)->fd_in);
	if ((*cur)->fd_out != -1)
		close((*cur)->fd_out);
	*prev = fd[0];
	*cur = (*cur)->next;
	return (last);
}

int	handle_parent_no_wait(t_node *cur, int fd[2], int *prev_fd)
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
	int	wait_result;

	i = 0;
	status = 0;
	exit_code = 0;
	while (i < count)
	{
		wait_result = waitpid(pids[i], &status, 0);
		if (wait_result != -1)
		{
			if (WIFEXITED(status))
			{
				if (i == count - 1)
					exit_code = WEXITSTATUS(status);
			}
			else if (WIFSIGNALED(status))
				if (i == count - 1)
					exit_code = 128 + WTERMSIG(status);
		}
		i++;
	}
	return (exit_code);
}

bool	exec_multiple_cmd(t_data *data, t_node *cur, int fd[2], int prev_fd)
{
	int	last_cmd_error;
	int	i;

	i = 0;
	last_cmd_error = 0;
	data->pids = malloc(sizeof(pid_t) * count_commands(cur));
	while (cur)
	{
		if (cur->node_type == PIPE)
			cur = cur->next;
		if (cur->next && pipe(fd) == -1)
			return (free_program(data, "Pipe creation failed"));
		if (!identify_redirs(cur->redir, cur))
			last_cmd_error = handle_redir_pipe(data, fd, &cur, &prev_fd);
		else
			data->pids[i++] = exec_child(data, &cur, fd, &prev_fd);
	}
	if (last_cmd_error)
		data->exit_status = 1;
	else
		data->exit_status = wait_for_all_children(data->pids, i);
	free(data->pids);
	data->pids = 0;
	return (true);
}
