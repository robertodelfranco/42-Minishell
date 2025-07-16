/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rheringe <rheringe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 15:19:09 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/07/16 10:36:04 by rheringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	handle_exec_status(t_data *data, int status)
{
	if (WIFEXITED(status))
		data->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		data->exit_status = 128 + WTERMSIG(status);
}

char	*get_command_path(t_data *data, t_node *cur)
{
	char	*full_path;

	if (cur->cmd[0][0] == '/' || ft_strncmp(cur->cmd[0], "./", 2) == 0
		|| ft_strncmp(cur->cmd[0], "../", 3) == 0)
		full_path = ft_strdup(cur->cmd[0]);
	else
		full_path = ft_get_external_path(data, cur->cmd[0]);
	return (full_path);
}

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
