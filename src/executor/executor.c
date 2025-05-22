/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 14:39:51 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/05/22 15:14:48 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

bool	executor(t_data *data)
{
	t_node	*cur;

	cur = data->exec_list;
	while (cur)
	{
		if (cur->node_type == PIPE)
		{
			printf("Pipe node %d\n", cur->node_type);
		}
		else if (cur->node_type == BUILT_IN)
		{
			printf("Built-in node %d\n", cur->node_type);
		}
		else if (cur->node_type == EXTERNAL)
		{
			printf("External node %d\n", cur->node_type);
		}
		else
		{
			printf("Unknown node type %d\n", cur->node_type);
		}
		cur = cur->next;
	}
	return (true);
}
