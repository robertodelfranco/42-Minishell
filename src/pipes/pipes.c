/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 09:55:48 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/05/26 11:28:00 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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

char	**get_env_array(t_env *env_list)
{
	char	**env_array;
	int		i;

	env_array = ft_calloc((ft_listsize(env_list) + 1), sizeof(char *));
	if (!env_array)
		return (NULL);
	i = 0;
	while (i < ft_listsize(env_list))
	{
		if (env_list->key && env_list->value)
			env_array[i] = ft_strjoin_three(env_list->key, "=", env_list->value);
		else if (env_list->key)
			env_array[i] = ft_strjoin(env_list->key, "=");
		else
			env_array[i] = NULL;
		env_list = env_list->next;
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

void	ft_pipes(t_data *data)
{
	pid_t	pid;
	pid_t	pid_two;
	int		fd[2];
	char	**env_array;
	char	*full_path;

	if (pipe(fd) == -1)
	{
		ft_printf("Error: Pipe creation failed\n");
		return ;
	}
	env_array = get_env_array(data->env_list);
	pid = fork();
	if (pid < 0)
	{
		ft_printf("Error: Fork failed\n");
		close(fd[0]);
		close(fd[1]);
		return ;
	}
	else if (pid == 0)
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
		full_path = ft_get_external_path(data->exec_list->cmd[0]);
		if (execve(full_path, data->exec_list->cmd, env_array) == -1)
		{
			perror("execve");
			exit(free_program(data, "Command execution failed"));
		}
	}
	close(fd[1]); // Close the write end of the pipe in the parent process
	pid_two = fork();
	if (pid_two < 0)
	{
		ft_printf("Error: Fork failed\n");
		close(fd[0]);
		return ;
	}
	else if (pid_two == 0)
	{
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		full_path = ft_get_external_path(data->exec_list->next->cmd[0]);
		if (execve(full_path, data->exec_list->next->cmd, env_array) == -1)
		{
			perror("execve");
			exit(free_program(data, "Command execution failed"));
		}
	}
	close(fd[0]); // Close the read end of the pipe in the parent process
	waitpid(pid, NULL, 0); // Wait for the first child process to finish
	waitpid(pid_two, NULL, 0); // Wait for the second child process to finish
	ft_free_matrix(env_array);
}
