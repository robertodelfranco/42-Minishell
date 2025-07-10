/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 12:58:06 by rheringe          #+#    #+#             */
/*   Updated: 2025/07/10 15:27:27 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	handle_heredoc(int sig)
{
	(void)sig;
	g_sig = 130;
	write(STDOUT_FILENO, "\n", 1);
	exit(130);
}

void	handle_sigquit(int sig)
{
	(void)sig;
	signal(SIGQUIT, SIG_IGN);
}
