/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 14:39:51 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/05/26 17:20:25 by rdel-fra         ###   ########.fr       */
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
		data->exit = true;
	else
	{
		printf("Unknown built-in command: %s\n", cur->cmd[0]);
		return (false);
	}
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
		if (execute_external(data, cur)) // -> to be implemented
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
			printf("External\n");
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
