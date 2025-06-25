/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 14:10:39 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/06/19 15:32:28 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

bool	parse_args_list(t_data *data)
{
	t_type	type;
	t_parse	*node;
	t_token	*cur;

	cur = data->token_list;
	if (cur->type == REDIR)
		verify_pipeline(data, &cur);
	while (cur)
	{
		if (cur->type == PIPE || (cur->type == EXPAND
				|| cur->type == WORD))
			add_parse_list(data, get_operations(cur), cur->type);
		if ((cur->type == BUILT_IN || cur->type == EXTERNAL))
		{
			type = cur->type;
			node = add_parse_list(data, get_arguments(cur), type);
			get_redirs(node, &cur);
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

void	append_redir(t_redir **redir_list, t_token *cur)
{
	t_redir	*new_redir;
	t_redir	*last_redir;

	new_redir = ft_calloc(1, sizeof(t_redir));
	if (!new_redir)
		return ;
	new_redir->target = ft_strdup(cur->next->value);
	new_redir->type = ft_get_redir_type(cur->value);
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
	args[i++] = ft_strdup(cur->value);
	temp = cur->next;
	while (temp != NULL && temp->type != PIPE)
	{
		if (temp->type == REDIR)
			temp = temp->next->next;
		else
		{
			args[i++] = ft_strdup(temp->value);
			if (!args[i - 1])
				return (NULL);
			temp = temp->next;
		}
	}
	return (args);
}
