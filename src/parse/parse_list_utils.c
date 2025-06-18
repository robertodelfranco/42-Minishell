/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_list_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 20:53:57 by marvin            #+#    #+#             */
/*   Updated: 2025/06/17 20:53:57 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static bool	has_command(t_token *cur)
{
	while (cur)
	{
		if (cur->type == PIPE)
			return (false);
		if (cur->type != REDIR)
			return (true);
		cur = cur->next->next;
	}
	return (false);
}

static t_parse	*get_command_node(t_data *data, t_token *cur)
{
	t_parse	*node;

	while (cur && cur->type == REDIR)
		cur = cur->next->next;
	if (!cur)
		return (NULL);
	node = ft_calloc(1, sizeof(t_parse));
	if (!node)
		return (NULL);
	node->cmd = get_arguments(cur);
	node->node_type = cur->type;
	data->parse_list = node;
	return (node);
}

void	verify_pipeline(t_data *data, t_token **cur)
{
	t_parse	*node;

	if (!has_command(*cur))
		get_redir_node(data, cur);
	else
	{
		node = get_command_node(data, *cur);
		while (*cur)
		{
			if ((*cur)->type == REDIR)
			{
				append_redir(&node->redir, *cur);
				*cur = (*cur)->next->next;
			}
			else
				*cur = (*cur)->next;
		}
	}
}

void	get_redirs(t_parse *node, t_token **cur)
{
	while (*cur && (*cur)->type != PIPE)
	{
		if ((*cur)->type == REDIR)
		{
			append_redir(&node->redir, *cur);
			*cur = (*cur)->next->next;
		}
		else
			*cur = (*cur)->next;
	}
}

void	get_redir_node(t_data *data, t_token **tokens)
{
	t_parse	*first_node;

	first_node = ft_calloc(1, sizeof(t_parse));
	if (!first_node)
		return ;
	first_node->node_type = BUILT_IN;
	while (*tokens && ((*tokens)->type == REDIR))
	{
		if ((*tokens)->type == REDIR)
		{
			append_redir(&first_node->redir, *tokens);
			*tokens = (*tokens)->next->next;
		}
		else
			*tokens = (*tokens)->next;
	}
	data->parse_list = first_node;
}
