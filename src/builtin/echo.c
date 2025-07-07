/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafaelheringer <rafaelheringer@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 17:54:41 by rafaelherin       #+#    #+#             */
/*   Updated: 2025/07/07 13:37:35 by rafaelherin      ###   ########.fr       */
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

bool	echo(char **args)
{
	int	n_flag;

	n_flag = 0;
	if (args[1] != NULL)
	{
		if (ft_strcmp(args[1], "-n") == 0)
			n_flag = 1;
	}
	print_cmd(args, n_flag);
	return (true);
}
