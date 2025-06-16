/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-fra <rdel-fra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 12:14:15 by rdel-fra          #+#    #+#             */
/*   Updated: 2025/06/16 15:03:25 by rdel-fra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	read_heredoc(t_redir *redir, t_data *data)
{
	char	*delimiter;

	if (!redir->target)
		return ;
	delimiter = redir->target;
}
