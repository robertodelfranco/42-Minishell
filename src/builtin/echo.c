/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafaelheringer <rafaelheringer@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 17:54:41 by rafaelherin       #+#    #+#             */
/*   Updated: 2025/03/19 15:47:33 by rafaelherin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	echo(char **args)
{
	int	i;
	int n_flag;


	i = 0;
	n_flag = 0;
	
	while (i++, args[i] && args[i][0] == '-')
	{
		int j;
		int	check_n_flag;

		j = 1;
		check_n_flag = 1;
		while (args[i][j] != '\0')
		{
			if (args[i][j] != 'n')
            {
                check_n_flag = 0;
                break;
            }
            j++;
		}
		if (!check_n_flag || j == 1)
			break ;
		n_flag = 1;
	}
	while (args[i])
    {
        printf("%s", args[i]);
        if (args[i + 1])
            printf(" ");
        i++;
    }
	if (!n_flag)
        printf("\n");
    
    return (0);
}

// int main(int argc, char **argv)
// {
// 	if (argc < 2) 
// 		return (1); // Nenhum argumento foi passado além do próprio programa
	
// 	echo(argv + 1); // Passamos `argv + 1` para ignorar "echo"
// 	return (0);
// }