/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 15:10:52 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/05/26 11:19:55 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

bool	free_program(t_data *data, char *message)
{
	if (message)
		error_message(message);
	ft_free_env_list(data);
	ft_free_node_list(data);
	ft_free_parse_list(data);
	ft_free_token_list(data);
	free(data->prompt);
	free(data);
	return (false);
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
	while (ptr_matrix[i] != NULL)
	{
		free(ptr_matrix[i]);
		i++;
	}
	free(ptr_matrix);
	ptr_matrix = NULL;
}
