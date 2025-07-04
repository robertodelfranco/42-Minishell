/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_stack.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 15:04:39 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/06/18 15:51:33 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

bool	build_stack(t_data *data)
{
	t_parse	*cur;
	t_node	*node;

	cur = data->parse_list;
	while (cur)
	{
		if (cur->node_type != PIPE)
		{
			node = create_cmd_node(cur->cmd, cur->redir, cur->node_type);
			add_stack_node(data, node);
		}
		else if (cur->node_type == PIPE)
		{
			node = create_pipe_node(cur->node_type);
			add_stack_node(data, node);
		}
		cur = cur->next;
	}
	return (true);
}

void	add_stack_node(t_data *data, t_node *node)
{
	t_node	*last;

	if (data->exec_list == NULL)
		data->exec_list = node;
	else
	{
		last = data->exec_list;
		while (last->next != NULL)
			last = last->next;
		last->next = node;
		node->prev = last;
	}
}

t_node	*create_pipe_node(t_type type)
{
	t_node	*pipe;

	pipe = (t_node *)ft_calloc(1, sizeof(t_node));
	if (!pipe)
		return (NULL);
	pipe->node_type = type;
	return (pipe);
}

t_node	*create_cmd_node(char **prompt, t_redir *redir, t_type type)
{
	t_node	*cmd;

	cmd = (t_node *)ft_calloc(1, sizeof(t_node));
	if (!cmd)
		return (NULL);
	cmd->cmd = prompt;
	cmd->node_type = type;
	cmd->redir = redir;
	cmd->fd_in = -1;
	cmd->fd_out = -1;
	return (cmd);
}
