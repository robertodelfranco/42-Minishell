/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 15:53:47 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/07/14 15:26:18 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	ft_jump_single_quote(char *quoted, int *i)
{
	int	count;

	(*i)++;
	count = 0;
	while (quoted[*i] && quoted[*i] != '\'')
	{
		count++;
		(*i)++;
	}
	if (quoted[*i] == '\'')
		(*i)++;
	return (count);
}

static int	ft_jump_double_quote(char *quoted, int *i)
{
	int	count;

	(*i)++;
	count = 0;
	while (quoted[*i] && quoted[*i] != '\"')
	{
		count++;
		(*i)++;
	}
	if (quoted[*i] == '\"')
		(*i)++;
	return (count);
}

static int	ft_count_str_without_quotes(char *quoted)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (quoted[i])
	{
		if (quoted[i] == '\'')
			count += ft_jump_single_quote(quoted, &i);
		else if (quoted[i] == '\"')
			count += ft_jump_double_quote(quoted, &i);
		else
		{
			count++;
			i++;
		}
	}
	return (count);
}

static int	ft_copy_single_quote(char *quoted, char *unquoted, int *i, int *j)
{
	(*i)++;
	while (quoted[*i] && quoted[*i] != '\'')
	{
		unquoted[*j] = quoted[*i];
		(*j)++;
		(*i)++;
	}
	if (quoted[*i] == '\'')
		(*i)++;
	return (*i);
}

char	*ft_get_str_without_quotes(char *quoted)
{
	char	*unquoted;
	int		i;
	int		j;

	i = 0;
	j = 0;
	unquoted = ft_calloc(ft_count_str_without_quotes(quoted) + 1, 1);
	while (quoted[i])
	{
		if (quoted[i] == '\'')
			ft_copy_single_quote(quoted, unquoted, &i, &j);
		else if (quoted[i] == '\"')
		{
			i++;
			while (quoted[i] && quoted[i] != '\"')
				unquoted[j++] = quoted[i++];
			if (quoted[i] == '\"')
				i++;
		}
		else
			unquoted[j++] = quoted[i++];
	}
	return (unquoted);
}
