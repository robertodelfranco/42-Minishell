/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 17:09:30 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/05/19 10:02:27 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	*parse(t_data *data)
{
	if (!validate_tokens(data))
		return (free_program(data));
	if (!parse_tokens(data))
		return (free_program(data));
	if (!build_tree(data))
		return (free_program(data));
}

bool	validate_tokens(t_data *data)
{
	t_token	*current;

	current = data->token_list;
	if (current->type == PIPE || current->type == AND || current->type == OR)
		return (false);
	while (current)
	{
		if (current->type == PIPE && (current->next->type != BUILT_IN
			&& current->next->type != EXTERNAL) || current->next == NULL)
			return (false);
		if (current->type == AND && (current->next->type != BUILT_IN
			&& current->next->type != EXTERNAL && current->next->type != WORD)
			|| current->next == NULL)
			return (false);
		if (current->type == OR && (current->next->type != BUILT_IN
			&& current->next->type != EXTERNAL && current->next->type != WORD)
			|| current->next == NULL)
			return (false);
		if (current->type == IN_REDIR || current->type == OUT_REDIR
			|| current->type == APPEND || current->type == HEREDOC)
			if (current->next == NULL || current->next->type != WORD)
				return (false);
	}
	return (true);
}

// Tokens inesperados no início/fim: ex: começar ou terminar com | ou && ou redirecionamentos (<, >).

// Tokens duplicados sem conteúdo entre eles: ex: ||, && ou | |.

// Redirecionamentos sem target: ex: ls > ou cat <.

// Subshells mal fechados: parênteses abertos sem fechar ( ou ).


// if (current->type == WORD)
// 	ft_printf("WORD: %s\n", current->value);
// else if (current->type == PIPE)
// 	ft_printf("PIPE: %s\n", current->value);
// else if (current->type == IN_REDIR)
// 	ft_printf("IN_REDIR: %s\n", current->value);
// else if (current->type == OUT_REDIR)
// 	ft_printf("OUT_REDIR: %s\n", current->value);
// else if (current->type == AND)
// 	ft_printf("AND: %s\n", current->value);
// else if (current->type == OR)
// 	ft_printf("OR: %s\n", current->value);
// else if (current->type == HEREDOC)
// 	ft_printf("HEREDOC: %s\n", current->value);
// else if (current->type == APPEND)
// 	ft_printf("APPEND: %s\n", current->value);
// else if (current->type == OPEN_PAR)
// 	ft_printf("OPEN_PAR: %s\n", current->value);
// else if (current->type == CLOSE_PAR)
// 	ft_printf("CLOSE_PAR: %s\n", current->value);
// else if (current->type == SING_QUOTE)
// 	ft_printf("SING_QUOTE: %s\n", current->value);
// else if (current->type == DOUB_QUOTE)
// 	ft_printf("DOUB_QUOTE: %s\n", current->value);
// else if (current->type == EXPAND)
// 	ft_printf("EXPAND: %s\n", current->value);
// else if (current->type == ECHO)
// 	ft_printf("ECHO: %s\n", current->value);
// else if (current->type == CD)
// 	ft_printf("CD: %s\n", current->value);
// else if (current->type == PWD)
// 	ft_printf("PWD: %s\n", current->value);
// else if (current->type == EXPORT)
// 	ft_printf("EXPORT: %s\n", current->value);
// else if (current->type == UNSET)
// 	ft_printf("UNSET: %s\n", current->value);
// else if (current->type == ENV)
// 	ft_printf("ENV: %s\n", current->value);
// else if (current->type == EXIT)
// {
// 	data->exit = 1;
// 	ft_printf("EXIT: %s\n", current->value);
// }
// else if (current->type == EXTERNAL)
// 	ft_printf("EXTERNAL: %s\n", current->value);
// else
// 	ft_printf("UNKNOWN TOKEN: %s\n", current->value);
// current = current->next;