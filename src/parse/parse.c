/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 17:09:30 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/06/16 17:24:23 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static bool	handle_quotes(t_data *data)
{
	t_token	*cur;
	char	*temp;

	cur = data->token_list;
	while (cur)
	{
		if (cur->type == DOUB_QUOTE || cur->type == SING_QUOTE)
		{
			if (ft_strlen(cur->value) <= 2)
				temp = ft_strdup("");
			else
				temp = ft_substr(cur->value, 1, ft_strlen(cur->value) - 2);
			if (!temp)
				return (false);
			free(cur->value);
			cur->value = temp;
			if (cur->type == SING_QUOTE)
				cur->type = SING_QUOTE;
			else
				cur->type = EXPAND;
		}
		cur = cur->next;
	}
	return (true);
}

static bool	search_heredoc(t_data *data)
{
	t_token	*cur;

	cur = data->token_list;
	while (cur)
	{
		if (ft_strcmp(cur->value, "<<") == 0)
		{
			if (!cur->next)
				return (false);
			else if (cur->next)
				cur->next->type = DOC_WORD;
		}
		cur = cur->next;
	}
	return (true);
}

static bool	validate_tokens(t_data *data)
{
	t_token	*cur;
	t_token	*last;

	cur = data->token_list;
	if (cur->type == PIPE)
		return (false);
	last = ft_last(data->token_list);
	if (last->type == PIPE || last->type == REDIR)
		return (false);
	while (cur)
	{
		if (cur->type == PIPE)
			if (cur->next->type == PIPE || cur->next->type == REDIR)
				return (false);
		if (cur->type == REDIR)
			if (cur->next->type == PIPE || cur->next->type == REDIR)
				return (false);
		cur = cur->next;
	}
	return (true);
}

void	print_command(t_node *print)
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
	t_node	*print;

	print = data->exec_list;
	while (print)
	{
		ft_printf("type = %d, ", print->node_type);
		if (print->redir)
		{
			print_command(print);
			while (print->redir)
			{
				ft_printf("redir = %d - target = %s\n", print->redir->type,
					print->redir->target);
				print->redir = print->redir->next;
			}
		}
		else
			print_command(print);
		print = print->next;
	}
}

// echo hi > hi echo "hello" >> ha < hi > hu > ho >> he > hh 
// >> hc < hh > ll > kk > kd > ds < hi > jj < kk < ll >> jj
bool	parse(t_data *data)
{
	if (!search_heredoc(data))
		return (free_program(data, "Error searching heredoc"));
	if (!handle_quotes(data))
		return (free_program(data, "Quotes not closed"));
	if (!ft_expand(data))
		return (free_program(data, "Error expanding variables"));
	if (!get_new_types(data))
		return (free_program(data, "Error getting new types"));
	if (!validate_tokens(data))
		return (free_program(data, "Invalid tokens"));
	if (!parse_args_list(data))
		return (free_program(data, "Error parsing arguments"));
	data->fd[0] = dup(STDIN_FILENO);
	data->fd[1] = dup(STDOUT_FILENO);
	if (!build_stack(data))
		return (free_program(data, "Error building stack"));
	printf("Stack built successfully\n");
	print_list(data);
	return (true);
}
