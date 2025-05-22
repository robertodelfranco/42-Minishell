/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 15:10:52 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/05/22 17:13:03 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

bool	free_program(t_data *data)
{
	ft_free_token_list(data);
	free(data->prompt);
	free(data);
	return (false);
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

void	ft_free_matrix(char **ptr_matrix)
{
	int	i;

	i = 0;
	while (ptr_matrix[i])
	{
		free(ptr_matrix[i]);
		i++;
	}
	free(ptr_matrix);
}
