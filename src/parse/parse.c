/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 17:09:30 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/07/03 16:06:14 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
				if (!handle_quotes(cur->next))
					return (false);
		}
		cur = cur->next;
	}
	return (true);
}

static bool	validate_tokens(t_data *data)
{
	t_token	*cur;
	t_token	*last;

	if (data->unclosed_quote == true)
		return (false);
	cur = data->token_list;
	if (cur->type == PIPE)
		return (false);
	last = ft_last(data->token_list);
	if (last->type == PIPE || last->type == REDIR)
		return (false);
	while (cur)
	{
		if (cur->type == PIPE)
			if (cur->next->type == PIPE)
				return (false);
		if (cur->type == REDIR)
			if (cur->next->type == PIPE || cur->next->type == REDIR)
				return (false);
		cur = cur->next;
	}
	return (true);
}

// echo hi > hi echo "hello" >> ha < hi > hu > ho >> he > hh 
// >> hc < hh > ll > kk > kd > ds < hi > jj < kk < ll >> jj
bool	parse(t_data *data)
{
	if (!validate_tokens(data))
		return (free_program(data, "Invalid tokens"));
	if (!search_heredoc(data))
		return (free_program(data, "Error searching heredoc"));
	if (!ft_expand(data))
		return (free_program(data, "Error expanding variables"));
	if (!get_new_types(data))
		return (free_program(data, "Error getting new types"));
	if (!parse_args_list(data))
		return (free_program(data, "Error parsing arguments"));
	data->fd[0] = dup(STDIN_FILENO);
	data->fd[1] = dup(STDOUT_FILENO);
	if (!build_stack(data))
		return (free_program(data, "Error building stack"));
	return (true);
}
