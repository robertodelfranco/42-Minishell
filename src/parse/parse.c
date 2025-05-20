/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 17:09:30 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/05/20 14:21:12 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	print_list(t_data *data)
{
	t_parse	*print;

	print = data->parse_list;
	while (print)
	{
		ft_printf("type = %d, ", print->node_type);
		if (print->redir)
		{
			ft_printf("cmd = %s, ", print->cmd[0]);
			ft_printf("target = %s\n", print->redir->target);
		}
		else
			ft_printf("cmd = %s\n", print->cmd[0]);
		print = print->next;
	}
}

void	*parse(t_data *data)
{
	// if (data->double_quotes % 2 != 0 || data->single_quotes % 2 != 0)
	// 	return (free_program(data));
	// if (data->open_parentheses != data->close_parentheses)
	// 	return (free_program(data));
	if (!validate_tokens(data))
		return (free_program(data));
	if (!parse_args(data))
		return (free_program(data));
	print_list(data);
	// if (!build_tree(data))
	// 	return (free_program(data));
	return (NULL);
}
//ls -l | grep txt | wc -l > total.txt && echo Tem txt > log.txt && cat total.txt

bool	validate_tokens(t_data *data)
{
	t_token	*cur;
	t_token	*last;

	cur = data->token_list;
	if (cur->type == PIPE || cur->type == AND || cur->type == OR)
		return (false);
	last = ft_last(data->token_list);
	if (last->type == PIPE || last->type == AND || last->type == OR
		|| last->type == REDIR)
		return (false);
	// transform every if into a function
	while (cur)
	{
		if ((cur->type == PIPE && cur->next->type != BUILT_IN
				&& cur->next->type != EXTERNAL) || (cur->type == PIPE
				&& cur->next == NULL))
		{
			printf("Error: pipe with invalid target\n");
			return (false);
		}
		if ((cur->type == AND && cur->next->type != BUILT_IN
				&& cur->next->type != EXTERNAL && cur->next->type != WORD)
			|| (cur->type == AND && cur->next == NULL))
			return (false);
		if ((cur->type == OR && cur->next->type != BUILT_IN
				&& cur->next->type != EXTERNAL && cur->next->type != WORD)
			|| (cur->type == OR && cur->next == NULL))
			return (false);
		if (cur->type == REDIR)
		{
			if (cur->next == NULL || (cur->next->type != WORD && cur
					->next->type != BUILT_IN && cur->next->type != EXTERNAL))
			{
				printf("Error: redirection with invalid target\n");
				return (false);
			}
		}
		cur = cur->next;
	}
	return (true);
}

t_parse	*add_parse_list(t_data *data, char **args, t_type type)
{
	t_parse	*new_node;
	t_parse	*last;

	new_node = ft_calloc(1, sizeof(t_parse));
	if (!new_node)
		return (NULL);
	new_node->cmd = args;
	if (type == 1 || type == 3)
		new_node->node_type = AND;
	else
		new_node->node_type = type;
	new_node->redir = NULL;
	new_node->next = NULL;
	if (data->parse_list == NULL)
		data->parse_list = new_node;
	else
	{
		last = data->parse_list;
		while (last->next != NULL)
			last = last->next;
		last->next = new_node;
	}
	return (new_node);
}

bool	parse_args(t_data *data)
{
	char	**args;
	t_parse	*node;
	t_token	*cur;

	cur = data->token_list;
	while (cur)
	{
		if (cur->type == PIPE || cur->type == AND || cur->type == OR)
			add_parse_list(data, get_operations(cur), cur->type);
		if ((cur->type == BUILT_IN || cur->type == EXTERNAL))
		{
			cur->type = ft_get_type(cur->value);
			args = get_arguments(cur);
			node = add_parse_list(data, args, cur->type);
		}
		if (cur->type == REDIR)
			if (cur->next && cur->next->type == WORD)
				node->redir = create_redir(cur->value, cur->next->value);
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