/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 17:09:30 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/05/30 16:52:21 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static bool	get_new_types(t_data *data)
{
	t_token	*cur;

	cur = data->token_list;
	while (cur)
	{
		if (!cur->value)
			return (false);
		else
			cur->type = give_id_token(cur->value);
		cur = cur->next;
	}
	return (true);
}

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

bool	parse(t_data *data)
{
	if (!handle_quotes(data))
		return (free_program(data, "Quotes not closed"));
	if (!ft_expand(data))
		return (free_program(data, "Error expanding variables"));
	if (!get_new_types(data))
		return (free_program(data, "Error getting new types"));
	if (!validate_tokens(data))
		return (free_program(data, "Invalid tokens"));
	if (!parse_args(data))
		return (free_program(data, "Error parsing arguments"));
	data->fd[0] = dup(STDIN_FILENO);
	data->fd[1] = dup(STDOUT_FILENO);
	if (!build_stack(data))
		return (free_program(data, "Error building stack"));
	return (true);
}

bool	validate_tokens(t_data *data)
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
		if ((cur->type == PIPE && cur->next->type != BUILT_IN
				&& cur->next->type != EXTERNAL && cur->next->type != EXPAND)
			|| (cur->type == PIPE && cur->next == NULL))
			return (false);
		if (cur->type == REDIR)
		{
			if (cur->next == NULL || (cur->next->type != WORD && cur
					->next->type != BUILT_IN && cur->next->type != EXTERNAL
					&& cur->next->type != EXPAND))
				return (false);
		}
		cur = cur->next;
	}
	return (true);
}
