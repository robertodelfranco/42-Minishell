/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 15:22:45 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/07/08 13:28:00 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

bool	verify_tokens(t_data *data)
{
	if (data->token_list == NULL)
	{
		free_program(data, NULL);
		return (false);
	}
	return (true);
}

int	main(int ac, char **av, char **env)
{
	t_data	*data;

	(void)ac;
	(void)av;
	data = (t_data *)ft_calloc(1, sizeof(t_data));
	ft_init_env(data, env);
	while (true)
	{
		signal_setup_prompt();
		if (!ft_readline(data))
			continue ;
		create_token(data);
		if (!verify_tokens(data))
			continue ;
		if (!parse(data))
			continue ;
		setup_signals_exec();
		executor(data);
		free_program(data, NULL);
	}
}
