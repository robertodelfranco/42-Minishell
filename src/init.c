/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 15:20:19 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/04/09 15:55:24 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_redir	*create_redir_node(t_type type, char *target)
{
	t_redir	*redir;

	redir = (t_redir *)ft_calloc(1, sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->target = target;
	return (redir);
}

t_node	*create_pipe_node(t_node *cmd1, t_node *cmd2, bool subshell)
{
	t_node	*pipe;
	
	pipe = (t_node *)ft_calloc(1, sizeof(t_node));
	if (!pipe)
	return (NULL);
	pipe->type = PIPE_NODE;
	pipe->left = cmd1;
	pipe->right = cmd2;
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
	cmd->type = CMD_NODE;
	cmd->command = prompt;
	cmd->left = NULL;
	cmd->right = NULL;
	cmd->subshell = subshell;
	cmd->redir = redir;
	return (cmd);
}
