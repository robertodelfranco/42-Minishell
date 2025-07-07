/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 14:26:34 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/07/03 19:12:09 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_type	give_id_token(char *str)
{
	if (ft_strcmp(str, "|") == 0)
		return (PIPE);
	else if (ft_strcmp(str, "<<") == 0)
		return (REDIR);
	else if (ft_strcmp(str, ">>") == 0)
		return (REDIR);
	else if (ft_strcmp(str, "<") == 0)
		return (REDIR);
	else if (ft_strcmp(str, ">") == 0)
		return (REDIR);
	return (get_command(str));
}

t_type	get_command(char *token_name)
{
	if (ft_strcmp(token_name, "echo") == 0)
		return (BUILT_IN);
	else if (ft_strcmp(token_name, "cd") == 0)
		return (BUILT_IN);
	else if (ft_strcmp(token_name, "pwd") == 0)
		return (BUILT_IN);
	else if (ft_strcmp(token_name, "export") == 0)
		return (BUILT_IN);
	else if (ft_strcmp(token_name, "unset") == 0)
		return (BUILT_IN);
	else if (ft_strcmp(token_name, "env") == 0)
		return (BUILT_IN);
	else if (ft_strcmp(token_name, "exit") == 0)
		return (BUILT_IN);
	else if (token_name[0] == '\0')
		return (WORD);
	else if (token_name[0] == '/' || ft_strncmp(token_name, "./", 2) == 0
		|| ft_strncmp(token_name, "../", 3) == 0)
		return (EXTERNAL);
	else if (external_command(token_name) == 1)
		return (EXTERNAL);
	else
		return (WORD);
}

t_type	external_command(char *token_name)
{
	int		i;
	char	*full_cmd;
	char	*full_path;
	char	**new_path;

	new_path = ft_split(getenv("PATH"), ':');
	if (new_path == NULL)
		return (0);
	i = 0;
	while (new_path[i])
	{
		full_path = ft_strjoin(new_path[i++], "/");
		full_cmd = ft_strjoin(full_path, token_name);
		if (access(full_cmd, X_OK) == 0)
		{
			free(full_cmd);
			free(full_path);
			ft_free_matrix(new_path);
			return (1);
		}
		free(full_cmd);
		free(full_path);
	}
	ft_free_matrix(new_path);
	return (0);
}

t_token	*ft_last(t_token *lst)
{
	t_token	*nav;

	nav = lst;
	while (nav->next)
		nav = (*nav).next;
	return (nav);
}
