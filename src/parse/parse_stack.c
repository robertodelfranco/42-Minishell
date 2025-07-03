/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_stack.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 15:04:39 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/07/03 19:28:05 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	add_stack_node(t_data *data, t_node *node)
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

static t_node	*create_pipe_node(char **args, t_type type)
{
	t_node	*pipe;

	pipe = (t_node *)ft_calloc(1, sizeof(t_node));
	if (!pipe)
		return (NULL);
	pipe->cmd = args;
	pipe->node_type = type;
	return (pipe);
}

static t_node	*create_cmd_node(char **prompt, t_redir *redir, t_type type)
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
			cur->cmd = NULL; // Mark as transferred to avoid double free
			cur->redir = NULL; // Mark as transferred to avoid double free
		}
		else if (cur->node_type == PIPE)
		{
			node = create_pipe_node(cur->cmd, cur->node_type);
			add_stack_node(data, node);
			cur->cmd = NULL; // Mark as transferred to avoid double free
		}
		cur = cur->next;
	}
	return (true);
}
