/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 15:48:45 by rafaelherin       #+#    #+#             */
/*   Updated: 2025/07/08 12:22:23 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void sigint_handler_prompt(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

void sigint_handler_exec(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
}

void sigpipe_handler(int sig)
{
	(void)sig;
    return ;
}

void signal_setup_prompt(void)
{
	struct sigaction sa;
    
    sa.sa_handler = sigint_handler_prompt;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
    sa.sa_handler = sigpipe_handler;
    sigaction(SIGQUIT, &sa, NULL);
}

void setup_signals_exec(void)
{
    struct sigaction sa;

    sa.sa_handler = sigint_handler_exec;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
    sa.sa_handler = sigpipe_handler;
    sigaction(SIGQUIT, &sa, NULL);
}
