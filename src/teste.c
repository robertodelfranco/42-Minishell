/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   teste.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 18:52:50 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/05/09 17:35:13 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int main(void)
{
	t_data *data;

	data = (t_data *)ft_calloc(1, sizeof(t_data));
	while (true)
	{
		data->prompt = ft_readline();
		if (data->prompt == NULL)
			continue ;
		external_command(data->prompt);
	}
}