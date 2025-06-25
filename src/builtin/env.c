/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rheringe <rheringe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 14:32:21 by rheringe          #+#    #+#             */
/*   Updated: 2025/05/19 15:54:58 by rheringe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	env(t_data *data, char **args)
{
	t_env	*cur;

	if (args[1])
	{
		ft_printf("env: %s: No such file or directory\n", args[1]);
		data->exit_status = 127;
		return ;
	}
	cur = data->env_list;
	while (cur)
	{
		ft_printf("%s=%s\n", cur->key, cur->value);
		cur = cur->next;
	}
	data->exit_status = 0;
}
