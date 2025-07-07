/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 15:22:19 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/07/03 17:26:54 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	dup_fds(t_node *cur)
{
	if (cur->fd_in != -1)
	{
		if (dup2(cur->fd_in, STDIN_FILENO) < 0)
		{
			close(cur->fd_in);
			perror("dup2 failed on redir");
			return ;
		}
		close(cur->fd_in);
	}
	if (cur->fd_out != -1)
	{
		if (dup2(cur->fd_out, STDOUT_FILENO) < 0)
		{
			close(cur->fd_out);
			perror("dup2 failed on redir");
			return ;
		}
		close(cur->fd_out);
	}
}

int	ft_listsize(t_env *list)
{
	t_env	*ptr;
	int		i;

	i = 0;
	ptr = list;
	while (ptr != NULL)
	{
		ptr = ptr->next;
		i++;
	}
	return (i);
}

bool	fd_restore(t_data *data, t_node *cur)
{
	if (cur->fd_in != -1)
		close(cur->fd_in);
	if (cur->fd_out != -1)
		close(cur->fd_out);
	dup2(data->fd[0], STDIN_FILENO);
	dup2(data->fd[1], STDOUT_FILENO);
	close(data->fd[0]);
	close(data->fd[1]);
	return (true);
}

char	**get_env_array(t_env *env_list)
{
	char	**env_array;
	t_env	*cur;
	int		i;

	env_array = ft_calloc((ft_listsize(env_list) + 1), sizeof(char *));
	if (!env_array)
		return (NULL);
	i = 0;
	cur = env_list;
	while (cur)
	{
		if (cur->key && cur->value)
			env_array[i] = ft_strjoin_three(cur->key, "=",
					cur->value);
		else if (cur->key)
			env_array[i] = ft_strjoin(cur->key, "=");
		cur = cur->next;
		i++;
	}
	return (env_array);
}

char	*ft_get_external_path(char *token_name)
{
	int		i;
	char	*full_cmd;
	char	*full_path;
	char	**new_path;

	new_path = ft_split(getenv("PATH"), ':'); // function to get PATH from env list
	if (new_path == NULL)
		return (0);
	i = 0;
	while (new_path[i])
	{
		full_path = ft_strjoin(new_path[i++], "/");
		full_cmd = ft_strjoin(full_path, token_name);
		if (access(full_cmd, X_OK) == 0)
		{
			free(full_path);
			ft_free_matrix(new_path);
			return (full_cmd);
		}
		free(full_cmd);
		free(full_path);
	}
	ft_free_matrix(new_path);
	return (NULL);
}
