/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 15:04:39 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/05/20 14:23:39 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

bool	build_tree(t_data *data)
{
	t_parse	*cur;
	t_node	*node;

	cur = data->parse_list;
	while (cur)
	{
		if (cur->node_type == CMD_NODE)
		{
			node = create_cmd_node(cur->cmd, cur->redir, false);
			add_node_on_stack(node);
		}
		else if (cur->node_type == PIPE_NODE)
		cur = cur->next;
	}
	return (true);
}
