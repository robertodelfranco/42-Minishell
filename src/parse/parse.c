/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 17:09:30 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/06/30 14:21:00 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*ft_get_str_without_quotes(t_token *cur, char *str_quoted)
{
	char	*str_without_quotes;
	char	type;
	int		i;
	size_t	j;

	str_without_quotes = ft_calloc(ft_strlen(str_quoted) - 1, sizeof(char));
	if (!str_without_quotes)
		return (NULL);
	type = ft_findchar(str_quoted, "\'\"");
	cur->type = DOUB_QUOTE;
	if (type == '\'')
		cur->type = SING_QUOTE;
	i = 0;
	j = 0;
	while (str_quoted[i] && j < ft_strlen(str_quoted) - 1)
	{
		while (str_quoted[i] == '\'' && cur->type == SING_QUOTE)
			i++;
		while (str_quoted[i] == '\"' && cur->type == DOUB_QUOTE)
			i++;
		if (str_quoted[i] == '\0')
			break ;
		str_without_quotes[j++] = str_quoted[i++];
	}
	return (str_without_quotes);
}

static bool	handle_quotes(t_data *data)
{
	t_token	*cur;
	char	*temp;

	cur = data->token_list;
	while (cur)
	{
		if (ft_strchr(cur->value, '\'') || ft_strchr(cur->value, '\"'))
		{
			if (ft_strlen(cur->value) <= 2)
				temp = ft_strdup("");
			else
				temp = ft_get_str_without_quotes(cur, cur->value);
			if (!temp)
				return (false);
			free(cur->value);
			cur->value = temp;
			cur->quoted = true;
			if (cur->type == SING_QUOTE)
				cur->type = SING_QUOTE;
			else
				cur->type = EXPAND;
		}
		cur = cur->next;
	}
	return (true);
}

static bool	search_heredoc(t_data *data)
{
	t_token	*cur;

	cur = data->token_list;
	while (cur)
	{
		if (ft_strcmp(cur->value, "<<") == 0)
		{
			if (!cur->next)
				return (false);
			else if (cur->next)
				cur->next->type = DOC_WORD;
		}
		cur = cur->next;
	}
	return (true);
}

static bool	validate_tokens(t_data *data)
{
	t_token	*cur;
	t_token	*last;

	cur = data->token_list;
	if (cur->type == PIPE)
		return (false);
	last = ft_last(data->token_list);
	if (last->type == PIPE || last->type == REDIR)
		return (false);
	while (cur)
	{
		if (cur->type == PIPE)
			if (cur->next->type == PIPE || cur->next->type == REDIR)
				return (false);
		if (cur->type == REDIR)
			if (cur->next->type == PIPE || cur->next->type == REDIR)
				return (false);
		cur = cur->next;
	}
	return (true);
}

// echo hi > hi echo "hello" >> ha < hi > hu > ho >> he > hh 
// >> hc < hh > ll > kk > kd > ds < hi > jj < kk < ll >> jj
bool	parse(t_data *data)
{
	if (!search_heredoc(data))
		return (free_program(data, "Error searching heredoc"));
	if (!handle_quotes(data))
		return (free_program(data, "Quotes not closed"));
	if (!ft_expand(data))
		return (free_program(data, "Error expanding variables"));
	if (!get_new_types(data))
		return (free_program(data, "Error getting new types"));
	if (!validate_tokens(data))
		return (free_program(data, "Invalid tokens"));
	if (!parse_args_list(data))
		return (free_program(data, "Error parsing arguments"));
	data->fd[0] = dup(STDIN_FILENO);
	data->fd[1] = dup(STDOUT_FILENO);
	if (!build_stack(data))
		return (free_program(data, "Error building stack"));
	return (true);
}
