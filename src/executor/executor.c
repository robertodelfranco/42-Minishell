/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 14:39:51 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/06/16 10:48:06 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

bool	fd_restore(t_data *data)
{
	dup2(data->fd[0], STDIN_FILENO);
	dup2(data->fd[1], STDOUT_FILENO);
	close(data->fd[0]);
	close(data->fd[1]);
	return (true);
}

bool	execute_one_command(t_data *data, t_node *cur)
{
	if (cur->redir)
		if (!identify_redirs(cur->redir, data))
			return (false);
	if (cur->node_type == BUILT_IN)
	{
		if (execute_built_in(data, cur))
		{
			data->exit_status = 0;
			return (fd_restore(data));
		}
	}
	else if (cur->node_type == EXTERNAL)
	{
		if (execute_external(data, cur))
		{
			data->exit_status = 0;
			return (fd_restore(data));
		}
	}
	else
	{
		data->exit_status = CMD_NOT_FOUND;
		printf("bash: %s: command not found\n", cur->cmd[0]);
	}
	return (false);
}

static bool	handle_child(t_data *data, t_node *cur, int fd[2], int prev_fd)
{
	if (cur->redir)
		if (!identify_redirs(cur->redir, data))
			return (false);
	if (cur->prev == NULL)
	{
		if (!cur->redir || cur->redir->type == IN_REDIR)
			ft_dup_and_close(fd[1], STDOUT_FILENO, fd[0]);
		execute_first_command(data, cur);
	}
	else if (cur->next == NULL)
	{
		if (!cur->redir)
			ft_dup_and_close(prev_fd, STDIN_FILENO, fd[0]);
		execute_last_command(data, cur);
	}
	else
	{
		if (!cur->redir)
		{
			ft_dup_and_close(prev_fd, STDIN_FILENO, fd[0]);
			ft_dup_and_close(fd[1], STDOUT_FILENO, -1);
		}
		execute_middle_command(data, cur);
	}
	return (true);
}

static bool	handle_parent(t_node *cur, int fd[2], int *prev_fd, pid_t pid)
{
	if (*prev_fd != -1)
		close(*prev_fd);
	if (cur->next != NULL)
		close(fd[1]);
	*prev_fd = fd[0];
	waitpid(pid, NULL, 0);
	return (true);
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
		if (cur->node_type == PIPE)
			cur = cur->next;
		if (cur->next && pipe(fd) == -1)
			return (free_program(data, "Pipe creation failed"));
		pid = fork();
		if (pid < 0)
			return (free_program(data, "Fork failed"));
		if (pid == 0)
			return (handle_child(data, cur, fd, prev_fd));
		handle_parent(cur, fd, &prev_fd, pid);
		cur = cur->next;
	}
	data->exit_status = 0;
	return (true);
}
