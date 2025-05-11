/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 15:22:45 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/05/09 17:30:41 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main(void)
{
	t_data	*data;

	while (true)
	{
		data = (t_data *)ft_calloc(1, sizeof(t_data));
		data->prompt = ft_readline();
		if (data->prompt != NULL)
			create_token(data);
		if (data->token_list != NULL)
			parsing(data);
		free(data->prompt);
		free(data);
	}
}
