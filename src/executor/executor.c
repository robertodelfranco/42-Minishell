/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 14:39:51 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/07/01 19:55:33 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static bool	execute_one_command(t_data *data, t_node *cur)
{
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
// if the command fails, i need to catch the error

bool	executor(t_data *data)
{
	t_node	*cur;
	int		fd[2];
	int		prev_fd;

	cur = data->exec_list;
	if (!open_redirs(data))
	{
		data->exit_status = 1;
		return (false);
	}
	while (cur && cur->cmd[0] && cur->cmd[1] == NULL)
	{
		cur->cmd[0] = ft_strtrim(cur->cmd[0], " \t\n");
		if (cur->cmd[0][0] == '\0')
		{
			cur = cur->next;
			if (!cur)
				return (true);
		}
		else
			break ;
	}
	if (cur->next == NULL)
		return (execute_one_command(data, cur));
	prev_fd = -1;
	if (!exec_multiple_cmd(data, cur, fd, prev_fd))
		return (false);
	return (true);
}
