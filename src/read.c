/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rheringe <rheringe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 15:31:14 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/07/09 15:06:09 by rheringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*ft_readline(t_data *data)
{
	data->prompt = readline(COLOR "Minihell $ " RESET);
	data->fd[0] = -1;
	data->fd[1] = -1;
	if (!data->prompt)
	{
		ft_putendl_fd("exit", STDOUT_FILENO);
		shutdown_program(data);
		exit(0);
	}
	if (data->prompt[0] == '\0')
	{
		free_program(data, NULL);
		return (NULL);
	}
	add_history(data->prompt);
	return (data->prompt);
}
