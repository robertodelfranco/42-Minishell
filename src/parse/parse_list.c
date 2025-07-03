/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 14:10:39 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/07/03 19:23:39 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	**get_operations(t_token *cur)
{
	char	**args;

	args = ft_calloc(2, sizeof(char *));
	if (!args)
		return (NULL);
	args[0] = ft_strdup(cur->value);
	if (!args[0])
		return (NULL);
	return (args);
}

static t_parse	*add_parse_list(t_data *data, char **args, t_type type)
{
	t_parse	*new_node;
	t_parse	*last;

	new_node = ft_calloc(1, sizeof(t_parse));
	if (!new_node)
		return (NULL);
	new_node->cmd = args;
	new_node->node_type = type;
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

static void	get_redirs(t_parse *node, t_token **cur)
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

bool	parse_args_list(t_data *data)
{
	t_type	type;
	t_parse	*node;
	t_token	*cur;

	cur = data->token_list;
	while (cur)
	{
		if (cur->type == REDIR)
		{
			verify_pipeline(data, &cur);
			if (!cur)
				break ;
		}
		else if (cur->type == BUILT_IN || cur->type == EXTERNAL)
		{
			type = cur->type;
			node = add_parse_list(data, get_arguments(cur), type);
			get_redirs(node, &cur);
		}
		else if (cur->type == PIPE || cur->type == WORD)
		{
			add_parse_list(data, get_operations(cur), cur->type);
			cur = cur->next;
		}
		else
			cur = cur->next;
	}
	return (true);
}

char	**get_arguments(t_token *cur)
{
	t_token	*temp;
	char	**args;
	int		i;

	i = ft_count_tokens(cur);
	args = ft_calloc(i + 1, sizeof(char *));
	if (!args)
		return (NULL);
	i = 0;
	temp = cur;
	while (temp != NULL && temp->type != PIPE)
	{
		if (temp->type == REDIR)
			temp = temp->next->next;
		else
		{
			args[i] = ft_strdup(temp->value);
			if (!args[i])
				return (NULL);
			i++;
			temp = temp->next;
		}
	}
	return (args);
}
