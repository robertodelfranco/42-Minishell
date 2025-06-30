/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 12:14:15 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/06/30 14:37:02 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

bool	read_heredoc(t_redir *redir, t_node *node, t_data *data)
{
	char		*delimiter;
	char		*filename;
	char		*new_str;
	char		*expand;
	char		*input;
	int			pid;
	int			fd;
	static int	id = 0;

	filename = ft_strjoin("heredoc_", ft_itoa(id));
	fd = open(filename, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	pid = fork();
	if (pid < 0)
		return (free_program(data, "Fork failed"));
	if (pid == 0)
	{
		delimiter = redir->target;
		while (true)
		{
			input = readline(COLOR "> " RESET);
			if (ft_strcmp(input, delimiter) == 0)
			{
				free(input);
				break ;
			}
			if (redir->quoted == false)
			{
				expand = ft_calloc(get_expand_size(data, input) + 1, 1);
				new_str = get_str_expanded(data, input, expand);
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
	waitpid(pid, NULL, 0);
	fd = open(filename, O_RDONLY);
	free(filename);
	if (node->fd_in != -1)
		close (node->fd_in);
	node->fd_in = fd;
	id++;
	return (true);
}
