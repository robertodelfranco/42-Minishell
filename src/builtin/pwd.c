/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 15:38:52 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/07/09 14:33:06 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

bool	pwd(t_data *data)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		ft_printf_fd(STDOUT_FILENO, "%s\n", cwd);
		free(cwd);
		data->exit_status = 0;
		return (true);
	}
	data->exit_status = 1;
	return (false);
}
