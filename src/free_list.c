/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_list.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 14:27:17 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/07/03 19:28:06 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_free_redir_list(t_redir *redir_list)
{
	t_redir	*cur;
	t_redir	*temp;

	if (!redir_list)
		return ;
	cur = redir_list;
	while (cur)
	{
		temp = cur->next;
		free(cur->target);
		free(cur);
		cur = temp;
	}
	redir_list = NULL;
}

void	ft_free_node_list(t_data *data)
{
	t_node	*current;
	t_node	*temp;

	if (!data->exec_list)
		return ;
	current = data->exec_list;
	while (current)
	{
		temp = current->next;
		if (current->cmd)
			ft_free_matrix(current->cmd);
		if (current->redir)
			ft_free_redir_list(current->redir);
		free(current);
		current = temp;
	}
	data->exec_list = NULL;
}

void	ft_free_parse_list(t_data *data)
{
	t_parse	*current;
	t_parse	*temp;

	if (!data->parse_list)
		return ;
	current = data->parse_list;
	while (current)
	{
		temp = current->next;
		if (current->cmd)
			ft_free_matrix(current->cmd);
		free(current);
		current = temp;
	}
	data->parse_list = NULL;
}

void	ft_free_token_list(t_data *data)
{
	t_token	*current;
	t_token	*temp;

	if (!data->token_list)
		return ;
	current = data->token_list;
	while (current)
	{
		temp = current->next;
		free(current->value);
		current->value = NULL;
		free(current);
		current = temp;
	}
	data->token_list = NULL;
}

void	ft_free_env_list(t_env *env_list)
{
	t_env	*current;
	t_env	*temp;

	if (!env_list)
		return ;
	current = env_list;
	while (current)
	{
		temp = current->next;
		free(current->key);
		if (current->value)
			free(current->value);
		free(current);
		current = temp;
	}
	env_list = NULL;
}
