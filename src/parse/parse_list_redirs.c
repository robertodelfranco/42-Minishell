/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_list_redirs.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 20:53:57 by marvin            #+#    #+#             */
/*   Updated: 2025/07/03 19:13:15 by rdel-fra         ###   ########.fr       */
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
	t_parse	*last;

	while (cur && cur->type == REDIR)
		cur = cur->next->next;
	if (!cur)
		return (NULL);
	node = ft_calloc(1, sizeof(t_parse));
	if (!node)
		return (NULL);
	node->cmd = get_arguments(cur);
	node->node_type = cur->type;
	if (data->parse_list == NULL)
		data->parse_list = node;
	else
	{
		last = data->parse_list;
		while (last->next != NULL)
			last = last->next;
		last->next = node;
	}
	return (node);
}

static void	get_redir_node(t_data *data, t_token **tokens)
{
	t_parse	*first_node;
	t_parse	*last;

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
	if (data->parse_list == NULL)
		data->parse_list = first_node;
	else
	{
		last = ft_last_parse(data->parse_list);
		last->next = first_node;
	}
}

void	append_redir(t_redir **redir_list, t_token *cur)
{
	t_redir	*new_redir;
	t_redir	*last_redir;

	new_redir = ft_calloc(1, sizeof(t_redir));
	if (!new_redir)
		return ;
	new_redir->target = ft_strdup(cur->next->value);
	new_redir->type = ft_get_redir_type(cur->value);
	new_redir->quoted = cur->next->quoted;
	new_redir->next = NULL;
	if (!*redir_list)
		*redir_list = new_redir;
	else
	{
		last_redir = *redir_list;
		while (last_redir->next)
			last_redir = last_redir->next;
		last_redir->next = new_redir;
	}
}

void	verify_pipeline(t_data *data, t_token **cur)
{
	t_parse	*node;

	if (!has_command(*cur))
		get_redir_node(data, cur);
	else
	{
		node = get_command_node(data, *cur);
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
}
