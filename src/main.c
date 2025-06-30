/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 15:22:45 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/06/30 14:46:12 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

bool	verify_tokens(t_data *data)
{
	if (data->token_list == NULL)
	{
		free_program(data, "No tokens found");
		return (false);
	}
	return (true);
}

int	main(int ac, char **av, char **env)
{
	t_data	*data;
	int		exit_status;

	(void)ac;
	(void)av;
	exit_status = 0;
	data = (t_data *)ft_calloc(1, sizeof(t_data));
	ft_init_env(data, env);
	while (true)
	{
		data->exit_status = exit_status;
		if (!ft_readline(data))
			continue ;
		create_token(data);
		if (!verify_tokens(data))
			continue ;
		if (!parse(data))
			continue ;
		executor(data);
		exit_status = data->exit_status;
		free_program(data, NULL);
	}
}
