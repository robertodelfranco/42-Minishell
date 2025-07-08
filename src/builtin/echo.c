/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 17:54:41 by rafaelherin       #+#    #+#             */
/*   Updated: 2025/07/08 16:28:30 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	print_cmd(char **args, int n_flag)
{
	int	i;

	i = 1;
	if (n_flag)
		i = 2;
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (!n_flag)
		printf("\n");
}

bool	echo(t_data *data, char **args)
{
	int	n_flag;

	n_flag = 0;
	if (args[1] != NULL)
	{
		if (ft_strcmp(args[1], "-n") == 0)
			n_flag = 1;
	}
	print_cmd(args, n_flag);
	data->exit_status = 0;
	return (true);
}
