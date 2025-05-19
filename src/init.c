/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 15:20:19 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/05/19 18:24:52 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_redir	*create_redir_node(t_type type, char *target, bool subshell)
{
	t_redir	*redir;

	redir = (t_redir *)ft_calloc(1, sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->target = target;
	redir->subshell = subshell;
	return (redir);
}

t_node	*create_pipe_node(t_node *node_left, t_node *node_right, bool subshell)
{
	t_node	*pipe;
	
	pipe = (t_node *)ft_calloc(1, sizeof(t_node));
	if (!pipe)
	return (NULL);
	pipe->node_type = PIPE_NODE;
	pipe->left = node_left;
	pipe->right = node_right;
	pipe->subshell = subshell;
	pipe->redir = NULL;
	return (pipe);
}

t_node	*create_cmd_node(char **prompt, t_redir *redir, bool subshell)
{
	t_node	*cmd;

	cmd = (t_node *)ft_calloc(1, sizeof(t_node));
	if (!cmd)
		return (NULL);
	cmd->node_type = CMD_NODE;
	cmd->cmd = prompt;
	cmd->left = NULL;
	cmd->right = NULL;
	cmd->subshell = subshell;
	cmd->redir = redir;
	return (cmd);
}
