/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 15:31:14 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/07/17 12:01:39 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*ft_readline(t_data *data)
{
	data->prompt = readline(COLOR "minihell$ " RESET);
	data->fd[0] = -1;
	data->fd[1] = -1;
	if (!data->prompt)
	{
		ft_putendl_fd("exit", STDOUT_FILENO);
		shutdown_program(data);
		exit(0);
	}
	if (g_sig != 0)
	{
		data->exit_status = g_sig;
		g_sig = 0;
	}
	if (data->prompt[0] == '\0')
	{
		free_program(data, NULL);
		return (NULL);
	}
	add_history(data->prompt);
	return (data->prompt);
}
