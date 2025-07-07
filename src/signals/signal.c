/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rafaelheringer <rafaelheringer@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 15:48:45 by rafaelherin       #+#    #+#             */
/*   Updated: 2025/07/07 17:14:28 by rafaelherin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char *rl_replace_line(const char *text, int clear_undo);
void rl_redisplay(void);

void sigint_handler_prompt(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	write(STDOUT_FILENO, COLOR "Minihell $ " RESET, 18);
}

void sigint_handler_exec(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
}

void sigpipe_handler(int sig)
{
	(void)sig;
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
