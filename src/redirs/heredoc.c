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
	int			fd;
	static int	id = 0;

	filename = ft_strjoin_free("/tmp/here_doc", ft_itoa(id));
	fd = open(filename, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	signal(SIGINT, handle_heredoc);
	read_heredoc(redir, redir->target, data, fd);
	close(fd);
	signal_setup_prompt();
	dup2(data->fd[0], STDIN_FILENO);
	free(redir->target);
	redir->target = filename;
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
		input = readline(COLOR "> " RESET);
		if (!input && g_sig != 0)
			return ;
		if (!input)
		{
			ft_printf_fd(2, "minishell: warning: here-document delimited by "
				"end-of-file (wanted `%s')\n", delimiter);
			return ;
		}
		if (ft_strcmp(input, delimiter) == 0)
		{
			free(input);
			return ;
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
}
