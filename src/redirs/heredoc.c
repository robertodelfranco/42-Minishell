/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 12:14:15 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/07/11 17:04:01 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

bool	init_heredoc(t_redir *redir, t_data *data)
{
	char		*filename;
	int			pid;
	int			fd;
	static int	id = 0;
	int			status;

	filename = ft_strjoin_free("/tmp/here_doc", ft_itoa(id));
	fd = open(filename, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (fd < 0)
		return (free_program(data, "Heredoc file creation failed"));
	pid = fork();
	if (pid == 0)
		read_heredoc(redir, redir->target, data, fd);
	else
	{
		signal(SIGINT, SIG_IGN);
		waitpid(pid, &status, 0);
		data->exit_status = WEXITSTATUS(status);
	}
	free(redir->target);
	redir->target = filename;
	close(fd);
	id++;
	return (true);
}

void	read_heredoc(t_redir *redir, char *delimiter, t_data *data, int fd)
{
	char		*new_str;
	char		*expand;
	char		*input;

	while (true)
	{
		signal(SIGINT, handle_heredoc);
		input = readline(COLOR "> " RESET);
		if (!input)
		{
			ft_printf_fd(2, "minishell: warning: here-document delimited by "
				"end-of-file (wanted `%s')\n", delimiter);
			break ;
		}
		if (ft_strcmp(input, delimiter) == 0)
		{
			free(input);
			break ;
		}
		if (redir->quoted == false)
		{
			expand = ft_calloc(get_exp_size(data, input, true) + 1, 1);
			new_str = get_str_expand(data, input, expand, true);
			free(input);
			input = new_str;
		}
		ft_putendl_fd(input, fd);
		free(input);
	}
	close(fd);
	shutdown_program(data);
	exit(0);
}
