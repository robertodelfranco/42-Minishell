/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 15:22:45 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/05/26 16:01:17 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main(int ac, char **av, char **env)
{
	t_data	*data;

	(void)ac;
	(void)av;
	while (true)
	{
		data = (t_data *)ft_calloc(1, sizeof(t_data));
		data->prompt = ft_readline();
		if (data->prompt[0] == '\0')
			continue ;
		add_history(data->prompt);
		ft_init_env(data, env);
		create_token(data);
		if (data->token_list == NULL)
			continue ;
		if (!parse(data))
			continue ;
		if (!executor(data))
			continue ;
		if (data->exit)
			return (free_program(data, NULL));
		free_program(data, NULL);
		data = NULL;
	}
}
