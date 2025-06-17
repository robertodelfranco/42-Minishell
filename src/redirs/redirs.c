/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 17:14:38 by marvin            #+#    #+#             */
/*   Updated: 2025/06/17 18:50:14 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

bool	execute_redir_in(t_redir *redir, t_data *data)
{
	int	fd;

	fd = open(redir->target, O_RDONLY);
	(void)data;
	if (fd < 0)
	{
		perror("redirection in error: could not open file");
		return (false);
	}
	if (redir->target)
	{
		if (dup2(fd, STDIN_FILENO) < 0)
		{
			close(fd);
			perror("dup2 failed on redir in");
			return (false);
		}
		close(fd);
	}
	else
	{
		perror("redirection in error: no target specified\n");
		return (false);
	}
	return (true);
}

bool	execute_redir_out(t_redir *redir, t_data *data)
{
	int	fd;

	fd = open(redir->target, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	(void)data;
	if (fd < 0)
	{
		perror("redirection out error: could not open file");
		return (false);
	}
	if (redir->target)
	{
		if (dup2(fd, STDOUT_FILENO) < 0)
		{
			close(fd);
			perror("dup2 failed on redir out");
			return (false);
		}
		close(fd);
	}
	else
	{
		perror("redirection out error: no target specified\n");
		return (false);
	}
	return (true);
}

bool	execute_redir_append(t_redir *redir, t_data *data)
{
	int	fd;

	fd = open(redir->target, O_CREAT | O_APPEND | O_WRONLY, 0644);
	(void)data;
	if (fd < 0)
	{
		perror("redirection append error: could not open file");
		return (false);
	}
	if (redir->target)
	{
		if (dup2(fd, STDOUT_FILENO) < 0)
		{
			close(fd);
			perror("dup2 failed on redir append");
			return (false);
		}
		close(fd);
	}
	else
	{
		perror("redirection append error: no target specified\n");
		return (false);
	}
	return (true);
}
/*
bool	execute_redir_heredoc(t_redir **redir, t_data *data)
{
	int		fd;

	fd = open((*redir)->target, O_RDONLY);
	if (fd < 0)
	{
		perror("redirection heredoc error: could not open file");
		*redir = (*redir)->next;
		return (false);
	}
	if ((*redir)->target)
	{
		if (dup2(fd, STDIN_FILENO) < 0)
		{
			close(fd);
			perror("dup2 failed on redir heredoc");
			*redir = (*redir)->next;
			return (false);
		}
		*redir = (*redir)->next;
		close(fd);
	}
	else
	{
		perror("redirection heredoc error: no target specified\n");
		return (false);
	}
	return (true);
}
*/

bool	identify_redirs(t_redir *redir, t_data *data)
{
	t_redir	*cur;

	cur = redir;
	while (cur)
	{
		if (cur->type == IN_REDIR)
		{
			if (!execute_redir_in(cur, data))
				return (false);
		}
		else if (cur->type == OUT_REDIR)
		{
			if (!execute_redir_out(cur, data))
				return (false);
		}
		else if (cur->type == APPEND)
		{
			if (!execute_redir_append(cur, data))
				return (false);
		}
		else if (cur->type == HEREDOC)
			ft_printf("Heredoc-> >%s<\n", cur->target);
		cur = cur->next;
	}
	return (true);
}
