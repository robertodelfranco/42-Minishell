/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rheringe <rheringe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 14:39:51 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/06/02 15:51:16 by rheringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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

static bool	handle_child(t_data *data, t_node *cur, int fd[2], int prev_fd)
{
	if (cur->prev == NULL)
		execute_first_command(data, cur, fd);
	else if (cur->next == NULL)
		execute_last_command(data, cur, fd, prev_fd);
	else
		execute_middle_command(data, cur, fd, prev_fd);
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
	return (true);
}
