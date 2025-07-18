/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 15:10:52 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/07/18 10:15:14 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

bool	free_program(t_data *data, char *message)
{
	if (message)
		error_message(message);
	ft_free_token_list(data);
	ft_free_parse_list(data);
	ft_free_node_list(data);
	if (data->fd[0] != -1)
		close(data->fd[0]);
	if (data->fd[1] != -1)
		close(data->fd[1]);
	free(data->prompt);
	data->prompt = NULL;
	return (false);
}

bool	shutdown_program(t_data *data)
{
	free_program(data, NULL);
	ft_free_env_list(data->env_list);
	if (data->env_copy)
		ft_free_env_list(data->env_copy);
	free(data);
	data = NULL;
	return (true);
}

void	error_message(char *message)
{
	ft_putstr_fd("Error: ", 2);
	ft_putstr_fd(message, 2);
	ft_putstr_fd("\n", 2);
}

void	ft_free_matrix(char **ptr_matrix)
{
	int	i;

	i = 0;
	if (!ptr_matrix)
		return ;
	while (ptr_matrix[i] != NULL)
	{
		free(ptr_matrix[i]);
		i++;
	}
	free(ptr_matrix);
	ptr_matrix = NULL;
}

void	exit_not_found(t_data *data, char **env)
{
	if (env)
		ft_free_matrix(env);
	shutdown_program(data);
	exit(CMD_NOT_FOUND);
}
