/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 17:14:38 by marvin            #+#    #+#             */
/*   Updated: 2025/07/10 15:03:04 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static bool	execute_redir_in(t_redir *redir, t_node *cur)
{
	int	fd;

	fd = open(redir->target, O_RDONLY);
	if (fd < 0)
	{
		perror(redir->target);
		return (false);
	}
	if (cur->fd_in != -1)
		close(cur->fd_in);
	cur->fd_in = fd;
	return (true);
}

static bool	execute_redir_out(t_redir *redir, t_node *cur)
{
	int	fd;

	fd = open(redir->target, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (fd < 0)
	{
		perror(redir->target);
		return (false);
	}
	if (cur->fd_out != -1)
		close(cur->fd_out);
	cur->fd_out = fd;
	return (true);
}

static bool	execute_redir_append(t_redir *redir, t_node *cur)
{
	int	fd;

	fd = open(redir->target, O_CREAT | O_APPEND | O_WRONLY, 0644);
	if (fd < 0)
	{
		perror(redir->target);
		return (false);
	}
	if (cur->fd_out != -1)
		close(cur->fd_out);
	cur->fd_out = fd;
	return (true);
}

static bool	execute_redir_heredoc(t_redir *redir, t_node *cur)
{
	int	fd;

	fd = open(redir->target, O_RDONLY);
	if (fd < 0)
	{
		perror(redir->target);
		return (false);
	}
	unlink(redir->target);
	if (cur->fd_in != -1)
		close(cur->fd_in);
	cur->fd_in = fd;
	return (true);
}

bool	identify_redirs(t_redir *redir, t_node *node)
{
	while (redir)
	{
		if (redir->type == IN_REDIR)
		{
			if (!execute_redir_in(redir, node))
				return (false);
		}
		else if (redir->type == OUT_REDIR)
		{
			if (!execute_redir_out(redir, node))
				return (false);
		}
		else if (redir->type == APPEND)
		{
			if (!execute_redir_append(redir, node))
				return (false);
		}
		else if (redir->type == HEREDOC)
			if (!execute_redir_heredoc(redir, node))
				return (false);
		redir = redir->next;
	}
	return (true);
}
