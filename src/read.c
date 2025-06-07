/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 15:31:14 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/05/23 14:29:35 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*ft_readline(t_data *data, char **env)
{
	data->prompt = readline(COLOR "Minihell $ " RESET);
	if (data->prompt[0] == '\0')
	{
		free_program(data, NULL);
		return (NULL);
	}
	add_history(data->prompt);
	ft_init_env(data, env);
	return (data->prompt);
}
