/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 17:09:30 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/07/08 18:50:37 by rdel-fra         ###   ########.fr       */
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

static t_token	*validate_tokens(t_data *data)
{
	t_token	*cur;
	t_token	*last;

	cur = data->token_list;
	if (data->unclosed_quote == true)
		return (cur);
	if (cur->type == PIPE)
		return (cur);
	last = ft_last(data->token_list);
	if (last->type == PIPE || last->type == REDIR)
		return (cur);
	while (cur)
	{
		if (cur->type == PIPE)
			if (cur->next->type == PIPE)
				return (cur);
		if (cur->type == REDIR)
			if (cur->next->type == PIPE || cur->next->type == REDIR)
				return (cur);
		cur = cur->next;
	}
	return (NULL);
}

bool	parse(t_data *data)
{
	t_token	*cur;

	cur = validate_tokens(data);
	if (cur)
	{
		data->exit_status = 2;
		ft_printf_fd(2, "minishell: syntax error near unexpected token\n");
		return (free_program(data, NULL));
	}
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
