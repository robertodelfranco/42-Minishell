/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 10:48:03 by rafaelherin       #+#    #+#             */
/*   Updated: 2025/07/08 18:51:33 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	handle_operator_token(t_data *data, int i)
{
	int		start;
	int		len;
	char	*token;

	start = i;
	len = 1;
	if ((data->prompt[i] == '>' || data->prompt[i] == '<')
		&& data->prompt[i + 1] == data->prompt[i])
		len = 2;
	token = ft_substr(data->prompt, start, len);
	add_token_list(data, token, give_id_token(token));
	free(token);
	return (i + len);
}

int	create_token(t_data *data)
{
	int	i;

	i = 0;
	if (!data->prompt)
		return (0);
	while (data->prompt[i] && ft_strchr(NOPRINTABLE, data->prompt[i]))
		i++;
	while (data->prompt[i])
	{
		if (data->prompt[i] == '>' || data->prompt[i] == '<'
			|| data->prompt[i] == '|')
			i = handle_operator_token(data, i);
		else if (!ft_strchr(NOPRINTABLE, data->prompt[i]))
			i = handle_word_token(data, i);
		else
			i++;
	}
	return (i);
}

void	add_token_list(t_data *data, char *value, t_type id_token)
{
	t_token	*new_token;
	t_token	*last;

	new_token = (t_token *)malloc(sizeof(t_token));
	if (!new_token)
		return ;
	new_token->value = ft_strdup(value);
	if (!new_token->value)
	{
		free(new_token);
		return ;
	}
	new_token->type = id_token;
	new_token->quoted = false;
	new_token->next = NULL;
	if (!data->token_list)
		data->token_list = new_token;
	else
	{
		last = ft_last(data->token_list);
		last->next = new_token;
	}
}
