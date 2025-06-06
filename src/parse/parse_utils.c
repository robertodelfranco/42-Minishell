/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 14:09:30 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/05/28 15:57:39 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_type	ft_get_redir_type(char *redir)
{
	if (ft_strcmp(redir, ">") == 0)
		return (OUT_REDIR);
	else if (ft_strcmp(redir, "<") == 0)
		return (IN_REDIR);
	else if (ft_strcmp(redir, ">>") == 0)
		return (APPEND);
	else if (ft_strcmp(redir, "<<") == 0)
		return (HEREDOC);
	else
		return (WORD);
}

t_type	ft_get_cmd_type(char *value)
{
	if (ft_strcmp(value, "echo") == 0)
		return (ECHO);
	else if (ft_strcmp(value, "pwd") == 0)
		return (PWD);
	else if (ft_strcmp(value, "cd") == 0)
		return (CD);
	else if (ft_strcmp(value, "env") == 0)
		return (ENV);
	else if (ft_strcmp(value, "unset") == 0)
		return (UNSET);
	else if (ft_strcmp(value, "export") == 0)
		return (EXPORT);
	else if (ft_strcmp(value, "exit") == 0)
		return (EXIT);
	else
		return (EXTERNAL);
}
