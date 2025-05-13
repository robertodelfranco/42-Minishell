/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 17:09:30 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/05/13 14:11:39 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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

void	parsing(t_data *data)
{
	t_token	*current;

	current = data->token_list;
	while (current)
	{
		if (current->type == WORD)
			ft_printf("WORD: %s\n", current->value);
		else if (current->type == PIPE)
			ft_printf("PIPE: %s\n", current->value);
		else if (current->type == IN_REDIR)
			ft_printf("IN_REDIR: %s\n", current->value);
		else if (current->type == OUT_REDIR)
			ft_printf("OUT_REDIR: %s\n", current->value);
		else if (current->type == AND)
			ft_printf("AND: %s\n", current->value);
		else if (current->type == OR)
			ft_printf("OR: %s\n", current->value);
		else if (current->type == HEREDOC)
			ft_printf("HEREDOC: %s\n", current->value);
		else if (current->type == APPEND)
			ft_printf("APPEND: %s\n", current->value);
		else if (current->type == OPEN_PAR)
			ft_printf("OPEN_PAR: %s\n", current->value);
		else if (current->type == CLOSE_PAR)
			ft_printf("CLOSE_PAR: %s\n", current->value);
		else if (current->type == SING_QUOTE)
			ft_printf("SING_QUOTE: %s\n", current->value);
		else if (current->type == DOUB_QUOTE)
			ft_printf("DOUB_QUOTE: %s\n", current->value);
		else if (current->type == EXPAND)
			ft_printf("EXPAND: %s\n", current->value);
		else if (current->type == ECHO)
			ft_printf("ECHO: %s\n", current->value);
		else if (current->type == CD)
			ft_printf("CD: %s\n", current->value);
		else if (current->type == PWD)
			ft_printf("PWD: %s\n", current->value);
		else if (current->type == EXPORT)
			ft_printf("EXPORT: %s\n", current->value);
		else if (current->type == UNSET)
			ft_printf("UNSET: %s\n", current->value);
		else if (current->type == ENV)
			ft_printf("ENV: %s\n", current->value);
		else if (current->type == EXIT)
		{
			data->exit = 1;
			ft_printf("EXIT: %s\n", current->value);
		}
		else if (current->type == NOT_BUILT_IN)
			ft_printf("NOT_BUILT_IN: %s\n", current->value);
		else
			ft_printf("UNKNOWN TOKEN: %s\n", current->value);
		current = current->next;
	}
}

// void	identify_command(t_data *data, t_token *current)
// {
// 	// if (current->value == ECHO)
// 	// {
// 	// 	while (current && current->type != WORD)
// 	// 		current = current->next;
// 	// 	if (current)
// 	// 		ft_printf("ECHO command detected with value: %s\n", current->value);
// 	// }

// }
