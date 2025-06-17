/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 14:09:30 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/06/17 18:48:28 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_type	ft_get_redir_type(char *redir)
{
	if (ft_strcmp(redir, ">") == 0)
		return (OUT_REDIR);
	else if (ft_strcmp(redir, "<") == 0)
		return (IN_REDIR);
	else if (ft_strcmp(redir, ">>") == 0)
		return (APPEND);
	else if (ft_strcmp(redir, "<<") == 0)
		return (HEREDOC);
	else
		return (WORD);
}

t_type	ft_get_cmd_type(char *value)
{
	if (value == NULL)
	{
		printf("Entrei\n");
		return (REDIR);
	}
	if (ft_strcmp(value, "echo") == 0)
		return (ECHO);
	else if (ft_strcmp(value, "pwd") == 0)
		return (PWD);
	else if (ft_strcmp(value, "cd") == 0)
		return (CD);
	else if (ft_strcmp(value, "env") == 0)
		return (ENV);
	else if (ft_strcmp(value, "unset") == 0)
		return (UNSET);
	else if (ft_strcmp(value, "export") == 0)
		return (EXPORT);
	else if (ft_strcmp(value, "exit") == 0)
		return (EXIT);
	else
		return (EXTERNAL);
}

int	ft_count_tokens(t_token *cur)
{
	int	count;

	count = 0;
	while (cur && cur->type != PIPE)
	{
		if (cur->type == REDIR)
		{
			cur = cur->next->next;
			continue ;
		}
		count++;
		cur = cur->next;
	}
	return (count);
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
