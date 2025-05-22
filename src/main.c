/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 15:22:45 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/05/22 17:16:50 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// fix empty prompt and prompt with only spaces
int	main(void)
{
	t_data	*data;

	while (true)
	{
		data = (t_data *)ft_calloc(1, sizeof(t_data));
		data->prompt = ft_readline();
		if (data->prompt == NULL)
			continue ;
		add_history(data->prompt);
		create_token(data);
		if (data->token_list == NULL)
			continue ;
		if (!parse(data))
			continue ;
		if (!executor(data))
			continue ;
		ft_free_node_list(data);
		ft_free_parse_list(data);
		ft_free_token_list(data);
		free(data->prompt);
		if (data->exit)
			return (free(data), 0);
		free(data);
		data = NULL;
	}
}
