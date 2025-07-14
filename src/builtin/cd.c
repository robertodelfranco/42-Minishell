/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 17:34:57 by rheringe          #+#    #+#             */
/*   Updated: 2025/07/14 14:59:59 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static bool	cd_to_home(t_data *data, char **argv)
{
	char	*home;
	char	*join;

	home = get_variable_value(data, "HOME");
	if (!home)
	{
		ft_putendl_fd("cd: HOME not set", 2);
		data->exit_status = 1;
		return (false);
	}
	if (argv[1] && argv[1][0] == '~' && argv[1][1] == '/')
	{
		join = ft_strjoin(home, &argv[1][1]);
		free(home);
		home = join;
	}
	if (chdir(home) != 0)
	{
		free(home);
		perror("cd");
		data->exit_status = 1;
		return (false);
	}
	free(home);
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

static t_env	*get_env_node(t_data *data, char *key)
{
	t_env	*env;

	env = data->env_list;
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

static void	update_pwd(t_data *data)
{
	char	*path;
	t_env	*pwd;
	t_env	*oldpwd;

	pwd = get_env_node(data, "PWD");
	oldpwd = get_env_node(data, "OLDPWD");
	path = ft_calloc(1024, sizeof(char));
	if (!pwd || !oldpwd || !path)
	{
		free(pwd);
		free(oldpwd);
		free(path);
		return ;
	}
	if (oldpwd->value)
		free(oldpwd->value);
	oldpwd->value = ft_strdup(pwd->value);
	getcwd(path, 1024);
	if (pwd->value)
		free(pwd->value);
	pwd->value = ft_strdup(path);
	free(path);
}

bool	cd(t_data *data, char **argv)
{
	if (!argv[1] || (argv[1][0] == '~' && argv[1][1] == '\0')
		|| (argv[1][0] == '~' && argv[1][1] == '/'))
		cd_to_home(data, argv);
	else if (argv[2])
	{
		ft_putendl_fd("cd: too many arguments", 2);
		data->exit_status = 1;
		return (false);
	}
	else
		if (!cd_to_path(data, argv[1]))
			return (false);
	update_pwd(data);
	return (true);
}
