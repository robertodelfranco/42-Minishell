/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 17:34:57 by rheringe          #+#    #+#             */
/*   Updated: 2025/05/26 15:55:10 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static bool	cd_to_home(t_data *data)
{
	char	*home;

	home = getenv("HOME");
	if (!home)
	{
		ft_putendl_fd("cd: HOME not set", 2);
		data->exit_status = 1;
		return (false);
	}
	if (chdir(home) != 0)
	{
		perror("cd");
		data->exit_status = 1;
		return (false);
	}
	return (true);
}

static bool	cd_to_path(t_data *data, char *path)
{
	struct stat	file_info;

	if (stat(path, &file_info) != 0)
	{
		ft_printf_fd(2, "cd: %s: No such file or directory\n", path);
		data->exit_status = 1;
		return (false);
	}
	if (chdir(path) != 0)
	{
		perror("cd");
		data->exit_status = 1;
		return (false);
	}
	return (true);
}

void	cd(t_data *data, char **argv)
{
	if (!argv[1] || (argv[1][0] == '~' && argv[1][1] == '\0'))
		cd_to_home(data);
	else if (argv[2])
	{
		ft_putendl_fd("cd: too many arguments", 2);
		data->exit_status = 1;
	}
	else
		cd_to_path(data, argv[1]);
}
