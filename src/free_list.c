/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_list.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 14:27:17 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/05/27 14:51:11 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
		if (current->redir)
			free(current->redir);
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

void	ft_free_env_list(t_data *data)
{
	t_env	*current;
	t_env	*temp;

	if (!data->env_list)
		return ;
	current = data->env_list;
	while (current)
	{
		temp = current->next;
		free(current->key);
		if (current->value)
			free(current->value);
		free(current);
		current = temp;
	}
	data->env_list = NULL;
}
// void	ft_free_pids(pid_t *pids)
// {
//     int i;

//     if (!pids)
//         return;

//     for (i = 0; pids[i] != -1; ++i)
//         waitpid(pids[i], NULL, 0);

//     free(pids);
// }
