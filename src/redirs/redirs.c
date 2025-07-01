/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 17:14:38 by marvin            #+#    #+#             */
/*   Updated: 2025/07/01 20:24:24 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

bool	execute_redir_in(t_redir *redir, t_node *cur)
{
	int	fd;

	fd = open(redir->target, O_RDONLY);
	if (fd < 0)
	{
		perror("redirection in error: could not open file");
		return (false);
	}
	if (cur->fd_in != -1)
		close(cur->fd_in);
	cur->fd_in = fd;
	return (true);
}

bool	execute_redir_out(t_redir *redir, t_node *cur)
{
	int	fd;

	fd = open(redir->target, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (fd < 0)
	{
		perror("redirection out error: could not open file");
		return (false);
	}
	if (cur->fd_out != -1)
		close(cur->fd_out);
	cur->fd_out = fd;
	return (true);
}

bool	execute_redir_append(t_redir *redir, t_node *cur)
{
	int	fd;

	fd = open(redir->target, O_CREAT | O_APPEND | O_WRONLY, 0644);
	if (fd < 0)
	{
		perror("redirection append error: could not open file");
		return (false);
	}
	if (cur->fd_out != -1)
		close(cur->fd_out);
	cur->fd_out = fd;
	return (true);
}

bool	identify_redirs(t_redir *redir, t_node *node, t_data *data)
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
			if (!init_heredoc(redir, node, data))
				return (false);
		redir = redir->next;
	}
	return (true);
}

bool	open_redirs(t_data *data)
{
	t_node	*cur;

	cur = data->exec_list;
	while (cur)
	{
		if (cur->redir)
		{
			if (!identify_redirs(cur->redir, cur, data))
			{
				data->exit_status = 1;
				return (false);
			}
		}
		cur = cur->next;
	}
	return (true);
}
