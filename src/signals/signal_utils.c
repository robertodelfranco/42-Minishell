/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 12:58:06 by rheringe          #+#    #+#             */
/*   Updated: 2025/07/14 12:57:42 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	handle_heredoc(int sig)
{
	(void)sig;
	g_sig = 130;
	ft_putendl_fd("", STDOUT_FILENO);
	rl_replace_line("", 0);
	rl_on_new_line();
	close(STDIN_FILENO);
}

void	handle_sigquit(int sig)
{
	(void)sig;
	signal(SIGQUIT, SIG_IGN);
}
