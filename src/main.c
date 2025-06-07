/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rheringe <rheringe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 15:22:45 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/06/02 13:01:42 by rheringe         ###   ########.fr       */
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

// #include <fcntl.h>
// #include <stdio.h>
// int main(int ac, char **av, char **env)
// {
// 	int fd;
// 	int fd2;

// 	fd = open("hi", O_RDONLY);
// 	fd2 = open("ho", O_RDONLY);
// 	printf("fd: %d, fd2: %d\n", fd, fd2);
// }

int	main(int ac, char **av, char **env)
{
	t_data	*data;
	int		exit_status;

	(void)ac;
	(void)av;
	exit_status = 0;
	while (true)
	{
		data = (t_data *)ft_calloc(1, sizeof(t_data));
		data->exit_status = exit_status;
		if (!ft_readline(data, env))
			continue ;
		create_token(data);
		if (!verify_tokens(data))
			continue ;
		if (!parse(data))
			continue ;
		executor(data);
		exit_status = data->exit_status;
		free_program(data, NULL);
		data = NULL;
	}
}
