/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 14:09:30 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/05/22 14:12:12 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_type	ft_get_redir_type(char *redir)
{
	if (strcmp(redir, ">") == 0)
		return (IN_REDIR);
	else if (strcmp(redir, "<") == 0)
		return (OUT_REDIR);
	else if (strcmp(redir, ">>") == 0)
		return (APPEND);
	else
		return (HEREDOC);
}

t_type	ft_get_cmd_type(char *value)
{
	if (strcmp(value, "echo") == 0)
		return (ECHO);
	else if (strcmp(value, "pwd") == 0)
		return (PWD);
	else if (strcmp(value, "cd") == 0)
		return (CD);
	else if (strcmp(value, "env") == 0)
		return (ENV);
	else if (strcmp(value, "unset") == 0)
		return (UNSET);
	else if (strcmp(value, "export") == 0)
		return (EXPORT);
	else if (strcmp(value, "exit") == 0)
		return (EXIT);
	else
		return (EXTERNAL);
}
