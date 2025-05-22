/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 17:54:41 by rafaelherin       #+#    #+#             */
/*   Updated: 2025/05/22 16:09:50 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	print_cmd(char **args, int n_flag)
{
	int	i;

	i = 1;
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

int	echo(char **args)
{
	int	i;
	int	n_flag;
	int	j;
	int	check_n_flag;

	i = 0;
	n_flag = 0;
	while (i++, args[i] && args[i][0] == '-')
	{
		j = 1;
		check_n_flag = 1;
		while (j++, args[i][j] != '\0')
		{
			if (args[i][j] != 'n')
			{
				check_n_flag = 0;
				break ;
			}
		}
		if (!check_n_flag || j == 1)
			break ;
		n_flag = 1;
	}
	print_cmd(args, n_flag);
	return (0);
}
