/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 14:39:51 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/07/09 17:39:01 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	handle_redir_pipe(t_data *data, int fd[2], t_node *cur, int *prev)
{
	t_node	*last_node;
	int		last;

	last_node = get_last_command_node(data->exec_list);
	last = 0;
	if (cur == last_node)
		last = 1;
	if (*prev != -1)
		close(*prev);
	if (cur->next != NULL)
		close(fd[1]);
	if (cur->fd_in != -1)
		close(cur->fd_in);
	if (cur->fd_out != -1)
		close(cur->fd_out);
	*prev = fd[0];
	return (last);
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
		exit(CMD_NOT_FOUND);
	}
	else if (cur->cmd[0][0] == '/' || ft_strncmp(cur->cmd[0], "./", 2) == 0
		|| ft_strncmp(cur->cmd[0], "../", 3) == 0)
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

static bool	execute_one_command(t_data *data, t_node *cur)
{
	if (!identify_redirs(cur->redir, cur, data))
	{
		data->exit_status = 1;
		fd_restore(data, cur);
		return (false);
	}
	dup_fds(cur);
	if (cur->node_type == BUILT_IN)
	{
		if (execute_built_in(data, cur))
			return (fd_restore(data, cur));
	}
	else if (cur->node_type == EXTERNAL)
	{
		if (execute_external(data, cur))
			return (fd_restore(data, cur));
	}
	else
	{
		data->exit_status = CMD_NOT_FOUND;
		ft_printf_fd(2, "minishell: %s: command not found\n", cur->cmd[0]);
	}
	fd_restore(data, cur);
	return (false);
}

bool	executor(t_data *data)
{
	t_node	*cur;
	int		fd[2];
	int		prev_fd;
	char	*temp;

	cur = data->exec_list;
	if (cur->cmd != NULL)
	{
		while (cur && cur->cmd[0] && cur->cmd[1] == NULL)
		{
			temp = ft_strtrim(cur->cmd[0], " \t\n");
			free(cur->cmd[0]);
			cur->cmd[0] = temp;
			if (cur->cmd[0][0] == '\0')
			{
				cur = cur->next;
				if (!cur)
				{
					data->exit_status = 0;
					return (true);
				}
			}
			else
				break ;
		}
	}
	if (cur->next == NULL)
		return (execute_one_command(data, cur));
	prev_fd = -1;
	if (!exec_multiple_cmd(data, cur, fd, prev_fd))
		return (false);
	return (true);
}
