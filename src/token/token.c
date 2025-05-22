/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 10:48:03 by rafaelherin       #+#    #+#             */
/*   Updated: 2025/05/22 14:44:43 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// if (data->prompt[i] == '"')
// 	get_quotes(data, i);
int	create_token(t_data *data)
{
	int	i;

	i = 0;
	while (ft_strchr(NOPRINTABLE, data->prompt[i]) != NULL
		&& data->prompt[i] != '\0')
		i++;
	while (data->prompt[i] != '\0')
	{
		if (!ft_strchr(NOPRINTABLE, data->prompt[i]))
		{
			get_token(data, i);
			while (data->prompt[i] && !ft_strchr(NOPRINTABLE, data->prompt[i]))
				i++;
		}
		else
			i++;
	}
	return (i);
}

void	get_token(t_data *data, int start)
{
	int		end;
	char	*token_name;
	t_type	id_token;

	end = start;
	while (data->prompt[end] && !ft_strchr(NOPRINTABLE, data->prompt[end]))
		end++;
	token_name = ft_substr(data->prompt, start, end - start);
	id_token = give_id_token(token_name);
	add_token_list(data, token_name, id_token);
	free(token_name);
}

void	add_token_list(t_data *data, char *token_name, t_type id_token)
{
	t_token	*new_token;
	t_token	*last;

	new_token = ft_calloc(1, sizeof(t_token));
	if (!new_token)
		return ;
	if (token_name)
	{
		new_token->value = ft_strdup(token_name);
		if (!new_token->value)
			return (free(new_token));
	}
	else
		new_token->value = NULL;
	new_token->type = id_token;
	new_token->next = NULL;
	if (data->token_list == NULL)
		data->token_list = new_token;
	else
	{
		last = data->token_list;
		while (last->next != NULL)
			last = last->next;
		last->next = new_token;
	}
}
