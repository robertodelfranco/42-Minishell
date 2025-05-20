/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 14:09:30 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/05/20 17:47:04 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	**get_operations(t_token *cur)
{
	char	**args;
	int		i;

	i = 1;
	args = ft_calloc(i + 1, sizeof(char *));
	if (!args)
		return (NULL);
	args[0] = ft_strdup(cur->value);
	if (!args[0])
		return (NULL);
	return (args);
}

char	**get_arguments(t_token *cur)
{
	t_token	*count;
	char	**args;
	int		i;

	i = 1;
	count = cur->next;
	while (count && count->type == WORD)
	{
		i++;
		count = count->next;
	}
	args = ft_calloc(i + 1, sizeof(char *));
	if (!args)
		return (NULL);
	i = 0;
	args[i++] = ft_strdup(cur->value);
	cur = cur->next;
	while (cur && cur->type == WORD)
	{
		args[i++] = ft_strdup(cur->value);
		if (!args[i - 1])
			return (NULL);
		cur = cur->next;
	}
	return (args);
}

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

t_type	ft_get_type(char *value)
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

t_token	*ft_last(t_token *lst)
{
	t_token	*nav;

	nav = lst;
	while (nav->next)
		nav = (*nav).next;
	return (nav);
}
