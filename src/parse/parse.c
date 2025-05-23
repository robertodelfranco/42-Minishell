/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 17:09:30 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/05/23 16:03:24 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	print_command(t_parse *print)
{
	int	i;

	i = 0;
	ft_printf("cmd = ");
	while (print->cmd[i])
	{
		ft_printf("%s ", print->cmd[i]);
		i++;
	}
	ft_printf("\n");
}
// echo hi > texiste | cat texiste | ls -l | grep ".c"

void	print_list(t_data *data)
{
	t_parse	*print;

	print = data->parse_list;
	while (print)
	{
		ft_printf("type = %d, ", print->node_type);
		if (print->redir)
		{
			print_command(print);
			ft_printf("redir = %d - target = %s\n", print->redir->type,
				print->redir->target);
		}
		else
			print_command(print);
		print = print->next;
	}
}

bool	parse(t_data *data)
{
	if (data->double_quotes % 2 != 0 || data->single_quotes % 2 != 0)
		return (free_program(data, "Quotes not closed"));
	if (!validate_tokens(data))
		return (free_program(data, "Invalid tokens"));
	if (!parse_args(data))
		return (free_program(data, "Error parsing arguments"));
	print_list(data);
	if (!build_stack(data))
		return (free_program(data, "Error building stack"));
	return (true);
}

bool	validate_tokens(t_data *data)
{
	t_token	*cur;
	t_token	*last;

	cur = data->token_list;
	if (cur->type == PIPE || cur->type == WORD)
		return (false);
	last = ft_last(data->token_list);
	if (last->type == PIPE || last->type == REDIR)
		return (false);
	while (cur)
	{
		if ((cur->type == PIPE && cur->next->type != BUILT_IN
				&& cur->next->type != EXTERNAL) || (cur->type == PIPE
				&& cur->next == NULL))
			return (false);
		if (cur->type == REDIR)
		{
			if (cur->next == NULL || (cur->next->type != WORD && cur
					->next->type != BUILT_IN && cur->next->type != EXTERNAL))
				return (false);
		}
		cur = cur->next;
	}
	return (true);
}

// ft_free_token_list(data);
// echo -n hello world | ls -l && echo hi again > get.txt || cat get.txt
// ls -l | grep src && echo ok > log.txt

// Tokens inesperados no início/fim: ex: começar ou terminar com 
// | ou && ou redirecionamentos (<, >).
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
// else if (current->type == BUILT_IN)
// 	ft_printf("BUILT_IN: %s\n", current->value);
// else if (current->type == EXTERNAL)
// 	ft_printf("EXTERNAL: %s\n", current->value);
// else
// 	ft_printf("UNKNOWN TOKEN: %s\n", current->value);
// current = current->next;