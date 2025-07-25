/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_handle_quotes.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 14:52:08 by rheringe          #+#    #+#             */
/*   Updated: 2025/07/16 15:23:35 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	process_quotes(t_data *data, int i)
{
	char	quote;

	quote = data->prompt[i++];
	while (data->prompt[i] && data->prompt[i] != quote)
		i++;
	if (data->prompt[i] == '\0')
	{
		data->unclosed_quote = true;
		return (i);
	}
	if (data->prompt[i])
		i++;
	return (i);
}

int	handle_word_token(t_data *data, int i)
{
	int		start;
	char	*token;

	start = i;
	while (data->prompt[i] && !ft_strchr(NOPRINTABLE, data->prompt[i])
		&& data->prompt[i] != '>' && data->prompt[i] != '<'
		&& data->prompt[i] != '|')
	{
		if (data->prompt[i] == '\'' || data->prompt[i] == '\"')
			i = process_quotes(data, i);
		else
			i++;
	}
	token = ft_substr(data->prompt, start, i - start);
	add_token_list(data, token, give_id_token(token));
	free(token);
	return (i);
}
