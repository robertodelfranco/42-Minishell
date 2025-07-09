/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rheringe <rheringe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 12:14:15 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/07/09 16:37:57 by rheringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

bool	init_heredoc(t_redir *redir, t_node *node, t_data *data)
{
	char		*filename;
	int			pid;
	int			fd;
	static int	id = 0;
	int			status;

	g_sig = 0;
	filename = ft_strjoin_free("heredoc_", ft_itoa(id));
	fd = open(filename, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (fd < 0)
		return (free_program(data, "Heredoc file creation failed"));
	pid = fork();
	if (pid < 0)
		return (free_program(data, "Fork failed"));
	if (pid == 0)
		read_heredoc(redir, redir->target, data, fd);
	waitpid(pid, &status, 0);
	signal_setup_prompt();
	if (WIFSIGNALED(status) || g_sig == SIGINT)
	{
		g_sig = 0;
		unlink(filename);
		free(filename);
		return (false);
	}
	fd = open(filename, O_RDONLY);
	unlink(filename);
	free(filename);
	if (node->fd_in != -1)
		close (node->fd_in);
	node->fd_in = fd;
	id++;
	return (true);
}

void	read_heredoc(t_redir *redir, char *delimiter, t_data *data, int fd)
{
	char		*new_str;
	char		*expand;
	char		*input;

	g_sig = 0;
	heredoc_signal();
	while (true)
	{
		if (g_sig == SIGINT)
		{
			close(fd);
			exit(1);
		}
		input = readline(COLOR "> " RESET);
		if (g_sig == SIGINT)
		{
			if (input)
				free(input);
			close(fd);
			exit(1);
		}
		if (!input)
		{
			fprintf(stderr,
				"minishell: warning: here-document delimited by end-of-file "
				"(wanted `%s')\n", delimiter);
			close(fd);
			exit(0);
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
		ft_putstr_fd(input, fd);
		ft_putstr_fd("\n", fd);
		free(input);
	}
	close(fd);
	exit(0);
}
