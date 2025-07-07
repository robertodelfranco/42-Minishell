/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_stack.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 15:04:39 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/07/03 20:09:46 by rdel-fra         ###   ########.fr       */
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

static char	**ft_strdup_matrix(char **matrix)
{
	char	**new_matrix;
	int		i;

	if (!matrix)
		return (NULL);
	new_matrix = (char **)ft_calloc(ft_ptrlen(matrix) + 1, sizeof(char *));
	if (!new_matrix)
		return (NULL);
	i = 0;
	while (matrix[i])
	{
		new_matrix[i] = ft_strdup(matrix[i]);
		if (!new_matrix[i])
		{
			ft_free_matrix(new_matrix);
			return (NULL);
		}
		i++;
	}
	return (new_matrix);
}

static t_node	*create_pipe_node(char **args, t_type type)
{
	t_node	*pipe;

	pipe = (t_node *)ft_calloc(1, sizeof(t_node));
	if (!pipe)
		return (NULL);
	pipe->cmd = ft_strdup_matrix(args);
	pipe->node_type = type;
	return (pipe);
}

static t_node	*create_cmd_node(char **prompt, t_redir *redir, t_type type)
{
	t_node	*command;

	command = (t_node *)ft_calloc(1, sizeof(t_node));
	if (!command)
		return (NULL);
	command->cmd = ft_strdup_matrix(prompt);
	command->node_type = type;
	command->redir = redir;
	command->fd_in = -1;
	command->fd_out = -1;
	return (command);
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
		}
		else if (cur->node_type == PIPE)
		{
			node = create_pipe_node(cur->cmd, cur->node_type);
			add_stack_node(data, node);
		}
		cur = cur->next;
	}
	return (true);
}
