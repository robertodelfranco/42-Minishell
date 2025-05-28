/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 14:10:39 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/05/28 15:55:47 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

bool	parse_args(t_data *data)
{
	t_type	type;
	t_parse	*node;
	t_token	*cur;

	cur = data->token_list;
	while (cur)
	{
		if (cur->type == PIPE || cur->type == EXPAND)
			add_parse_list(data, get_operations(cur), cur->type);
		if ((cur->type == BUILT_IN || cur->type == EXTERNAL))
		{
			type = cur->type;
			node = add_parse_list(data, get_arguments(&cur), type);
			if (cur == NULL)
				break ;
			if (cur->type == REDIR)
			{
				node->redir = create_redir(cur->value, cur->next->value);
				cur = cur->next->next;
			}
		}
		else
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

t_redir	*create_redir(char *redir, char *target)
{
	t_redir	*new_redir;

	new_redir = ft_calloc(1, sizeof(t_redir));
	if (!new_redir)
		return (NULL);
	new_redir->target = target;
	new_redir->type = ft_get_redir_type(redir);
	return (new_redir);
}

char	**get_operations(t_token *cur)
{
	char	**args;
	int		i;

	i = 1;
	args = ft_calloc(i + 1, sizeof(char *));
	if (!args)
		return (NULL);
	args[0] = ft_strdup(cur->value);
	if (!args[0])
		return (NULL);
	return (args);
}

char	**get_arguments(t_token **cur)
{
	t_token	*count;
	char	**args;
	int		i;

	i = 1;
	count = (*cur)->next;
	while (count && count->type != PIPE && count->type != REDIR)
	{
		i++;
		count = count->next;
	}
	args = ft_calloc(i + 1, sizeof(char *));
	if (!args)
		return (NULL);
	i = 0;
	args[i++] = ft_strdup((*cur)->value);
	(*cur) = (*cur)->next;
	while ((*cur) != NULL && (*cur)->type != PIPE && (*cur)->type != REDIR)
	{
		args[i++] = ft_strdup((*cur)->value);
		if (!args[i - 1])
			return (NULL);
		*cur = (*cur)->next;
	}
	return (args);
}
