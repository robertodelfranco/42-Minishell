/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 14:39:51 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/05/23 16:07:45 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	execute_built_in(t_data *data, t_node *cur)
{
	if (cur->node_type == ECHO)
		echo(cur->cmd);
	if (cur->node_type == ENV)
		env(data, cur->cmd);
	if (cur->node_type == PWD)
		pwd(data);
	if (cur->node_type == EXIT)
		data->exit = true;
}

bool	executor(t_data *data)
{
	t_node	*cur;

	cur = data->exec_list;
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
