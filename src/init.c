/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 15:20:19 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/05/20 10:48:31 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_redir	*create_redir(char *redir, char *target)
{
	t_redir	*new_redir;

	new_redir = ft_calloc(1, sizeof(t_redir));
	if (!new_redir)
		return (NULL);
	new_redir->target = target;
	new_redir->type = ft_get_redir_type(redir);
	new_redir->subshell = false;
	return (new_redir);
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
