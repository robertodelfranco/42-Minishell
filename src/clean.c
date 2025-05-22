/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 15:10:52 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/05/20 14:10:45 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	*free_program(t_data *data)
{
	ft_free_token_list(data);
	free(data->prompt);
	free(data);
	return (NULL);
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
