/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 15:19:09 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/07/14 17:45:17 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_dup_and_close(int fd, int dup, int clos)
{
	dup2(fd, dup);
	close(fd);
	if (clos != -1)
		close(clos);
}

bool	execute_built_in(t_data *data, t_node *cur)
{
	t_type	type;

	if (!cur->cmd)
		return (true);
	type = ft_get_cmd_type(cur->cmd[0]);
	if (type == ECHO)
		return (echo(data, cur->cmd));
	data->exit_status = 0;
	if (type == CD)
		return (cd(data, cur->cmd));
	else if (type == PWD)
		return (pwd(data));
	else if (type == ENV)
		return (env(data, cur->cmd));
	else if (type == EXIT)
		return (b_exit(data, cur->cmd));
	else if (type == EXPORT)
		return (b_export(data, cur->cmd));
	else if (type == UNSET)
		return (b_unset(data, cur->cmd));
	return (false);
}

int	get_execve_exit_code(char *cmd, char *full_path)
{
	struct stat	st;

	if (!full_path || !cmd)
	{
		if (cmd)
			ft_printf_fd(2, "minishell: %s: command not found\n", cmd);
		return (127);
	}
	if (access(full_path, F_OK) != 0)
	{
		ft_printf_fd(2, "minishell: %s: No such file or directory\n", cmd);
		return (127);
	}
	if (stat(full_path, &st) == 0 && S_ISDIR(st.st_mode))
	{
		ft_printf_fd(2, "minishell: %s: Is a directory\n", cmd);
		return (126);
	}
	if (access(full_path, X_OK) != 0)
	{
		ft_printf_fd(2, "minishell: %s: Permission denied\n", cmd);
		return (126);
	}
	ft_printf_fd(2, "minishell: %s: command not found\n", cmd);
	return (127);
}
