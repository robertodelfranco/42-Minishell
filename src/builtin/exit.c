/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rheringe <rheringe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:06:06 by rheringe          #+#    #+#             */
/*   Updated: 2025/05/26 17:58:31 by rheringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static bool	is_numeric(const char *str)
{
    int i = 0;

    if (!str)
        return false;
    if (str[i] == '-' || str[i] == '+')
        i++;
    if (str[i] == '\0')
        return false;
    while (str[i])
    {
        if (!ft_isdigit(str[i]))
            return false;
        i++;
    }
    return true;
}

void	b_exit(t_data *data, char **argv, pid_t *pids)
{
	long	code;

	code = data->exit_status;
	if (argv[1] && argv[2])
	{
		ft_putendl_fd("exit: too many arguments", 2);
		data->exit_status = 1;
		return ;
	}
	if (argv[1])
	{
		if (!is_numeric(argv[1]))
		{
			ft_printf_fd(2, "exit: %s: numeric argument required\n", argv[1]);
			code = 2;
		}
		else 
			code = ft_atol(argv[1]) % 256;
	}
	ft_free_pids(pids);
	exit(code);
}
