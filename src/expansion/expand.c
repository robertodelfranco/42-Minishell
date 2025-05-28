/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 17:40:55 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/05/28 18:17:47 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

bool    ft_expand(t_data *data)
{
	t_token *cur;

	cur = data->token_list;
	while (cur)
	{
		if (cur->value && cur->type != REDIR && cur->type != PIPE && cur->type != EXPAND)
		{
		}
	}
}
