/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 22:12:21 by marvin            #+#    #+#             */
/*   Updated: 2025/06/16 10:39:46 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/minishell.h"

static void	print_token(const t_token *token)
{
	if (!token)
		return ;
	ft_printf("value = >%s<, type = %d\n", token->value, token->type);
}

void	print_token_list(const t_data *data)
{
	t_token	*cur;

	cur = data->token_list;
	ft_printf("=== Token List ===\n");
	while (cur)
	{
		print_token(cur);
		cur = cur->next;
	}
	ft_printf("===================\n");
}

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
