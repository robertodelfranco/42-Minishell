/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 15:31:14 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/06/30 17:15:40 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*ft_readline(t_data *data)
{
	data->prompt = readline(COLOR "Minihell $ " RESET);
	data->fd[0] = -1;
	data->fd[1] = -1;
	if (data->prompt[0] == '\0')
	{
		free_program(data, NULL);
		return (NULL);
	}
	add_history(data->prompt);
	return (data->prompt);
}
